import requests
import json

# URL of the API endpoint
api_url = "http://localhost:11434/api/generate"

# Initial empty context
context = []

def send_message(message):
    global context
    # Prepare the payload with the message and the current context
    payload = {
        "model": "llama3.2:latest",
        "prompt": message,
        "context": context
    }

    # Print the payload for debugging
    print(json.dumps(payload, indent=2))
    
    # Send the request to the server
    response = requests.post(api_url, json=payload)
    
    # Print the raw response text for debugging
    print("Raw server response:", response.text)
    
    # Attempt to parse the response as JSON
    try:
        response_data = response.json()
    except requests.exceptions.JSONDecodeError as e:
        print("JSON decode error:", e)
        print("Response content:", response.text)
        return None  # Handle the error appropriately
    
    # Update the context with the new context vectors
    context = response_data.get("context", [])
    
    # Return the server's response message
    return response_data.get("message", {}).get("content", "")

# Example usage
user_input = "Hello, how can you assist me today?"
response = send_message(user_input)
print("Server Response:", response)
