import ollama, sys, chromadb
from utilities import getconfig

#embedmodel = getconfig()["embedmodel"]
#mainmodel = getconfig()["mainmodel"]

embedmodel = "nomic-embed-text"
mainmodel = "llama3.1:8b"

chroma = chromadb.HttpClient(host="localhost", port=8000)
collection = chroma.get_or_create_collection("buildragwithpython")

#query = input("Enter your question: ")
#if not query.strip():
        #print("Query is empty or invalid")
        #return

query = " ".join(sys.argv[1:])

queryembed = ollama.embeddings(model=embedmodel, prompt=query)['embedding']

#print(queryembed)

relevantdocs = collection.query(query_embeddings=[queryembed], n_results=5)["documents"][0]

#print("----------------------")
#print(relevantdocs)
#print("----------------------")


docs = "\n\n".join(relevantdocs)
modelquery = f"{query} - Answer that question using the following text as a resource: {docs}"

#print("----------------------")
#print(modelquery)
#print("----------------------")

stream = ollama.generate(model=mainmodel, prompt=modelquery, stream=True)

for chunk in stream:
  if chunk["response"]:
    print(chunk['response'], end='', flush=True)
