import requests
import json
import sys

def get_current_weather(location, format):
    # Implement your weather fetching logic here
    # For demonstration, we'll use a placeholder response
    print("---\nget_current_weather\n---\n")
    return f"The current weather in {location} has a temperature of 25°C."

def print_json(json_data, name):
    if (1 == 0):
      print("---")
      print(name)
      print(json.dumps(json_data, indent=2))  # Print full JSON 
      print("---")

def ask_ollama(question):
    payload = {
        "model": "llama3.2",
        "messages": [{"role": "user", "content": question}],
        "stream": False,
        "keep_alive": 0,
        "tools": [
            {
                "type": "function",
                "function": {
                    "name": "get_current_weather",
                    "description": "Get the current weather for a location",
                    "parameters": {
                        "type": "object",
                        "properties": {
                            "location": {
                                "type": "string",
                                "description": "The location to get the weather for, e.g. San Francisco, CA"
                            },
                            "format": {
                                "type": "string",
                                "description": "The format to return the weather in, e.g. 'celsius' or 'fahrenheit'",
                                "enum": ["celsius", "fahrenheit"]
                            }
                        },
                        "required": ["location", "format"]
                    }
                }
            }
        ]
    }

    try:
        print_json(payload, "First Payload")
        response = requests.post("http://localhost:11434/api/chat", json=payload)
        response.raise_for_status()  # Check for HTTP errors
        
        response_json = response.json()
        print_json(response_json, "First Response")

        if "tool_calls" in response_json["message"]:
            for tool_call in response_json["message"]["tool_calls"]:
                if tool_call["function"]["name"] == "get_current_weather":
                    location = tool_call["function"]["arguments"]["location"]
                    format = tool_call["function"]["arguments"]["format"]
                    weather_info = get_current_weather(location, format)

                    # Update the payload with the weather information
                    message_tool = {"role": "tool", "content": weather_info}

                    payload["messages"].append(message_tool)

                    print_json(payload, "Second Payload")

                    # Resubmit the request with the updated payload
                    response = requests.post("http://localhost:11434/api/chat", json=payload)
                    response.raise_for_status()
                    response_json = response.json()
                    print_json(response_json, "Second Response")
                    
                    print(response_json["message"]["content"])

        else:
            # Handle the standard request
            message_user = {"role": "user", "content": question}

            payload = {
                "model": "llama3.2",
                "messages": [message_user],
                "stream": False
            }

            print_json(payload, "Payload with no tool function")

            # Resubmit the request with the updated payload
            response = requests.post("http://localhost:11434/api/chat", json=payload)
            response.raise_for_status()
            response_json = response.json()
            print_json(response_json, "Second Response")
            
            print(response_json["message"]["content"])

    except requests.exceptions.RequestException as e:
        print(f"Request failed: {e}")

    return

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 generate_request.py <question>")
        sys.exit(1)
    
    question = " ".join(sys.argv[1:])
    response = ask_ollama(question)
