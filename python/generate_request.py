import sys
from ollama import chat
# from ollama import ChatResponse

def ask_ollama(question):
  stream = chat(
      model='llama3.2',
      messages=[{'role': 'user', 'content': question}],
      stream=True,
  )

  for chunk in stream:
    print(chunk['message']['content'], end='', flush=True)

  return

if __name__ == "__main__":
  if len(sys.argv) < 2:
    print("Usage: python3 generate_request.py <question>")
    sys.exit(1)
  
  question = " ".join(sys.argv[1:])
  response = ask_ollama(question)
