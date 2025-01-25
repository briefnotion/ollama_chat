import requests
import json
import sys
import os

def get_current_weather(location, format):
    # Implement your weather fetching logic here
    # For demonstration, we'll use a placeholder response
    print("---\nget_current_weather\n---\n")
    return f"The current weather in {location} has a temperature of 25°C."

def print_json(json_data, name, show):
    if show:
        print("---")
        print(name)
        print(json.dumps(json_data, indent=2))
        print("---")

def ask_ollama(input_filename, output_filename):

    try:
        # Load the existing context from the input file
        if os.path.exists(input_filename):
            with open(input_filename, "r") as f:
                payload = json.load(f)
        else:
            payload = {
                "model": "llama3.2",
                "messages": [],
                "stream": True,
                "keep_alive": 0,
            }

        print_json(payload, "Second Payload", False)

        full_content = ""

        # Submit the request with the payload and stream response
        with requests.post("http://localhost:11434/api/chat", json=payload, stream=True) as response:
            response.raise_for_status()
            
            for line in response.iter_lines(delimiter=b'\n'):
                if line:
                    try:
                        response_json = json.loads(line.decode('utf-8').strip())
                        if 'message' in response_json and 'content' in response_json['message']:
                            print(response_json['message']['content'], end="", flush=True)
                            full_content += response_json['message']['content']
                    except json.JSONDecodeError as e:
                        print(f"Decoding JSON has failed: {e}")

        # Append the collected content to the messages
        payload["messages"].append({"role": "assistant", "content": full_content})

        # Save the updated context to the output file
        with open(output_filename, "w") as outfile:
            json.dump(payload, outfile, indent=2)
            # print(f"Context saved to {output_filename}")

    except requests.exceptions.RequestException as e:
        print(f"Request failed: {e}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python3 generate_request.py <input_filename> <output_filename>")
        sys.exit(1)
    
    input_filename = sys.argv[1]
    output_filename = sys.argv[2]

    ask_ollama(input_filename, output_filename)
