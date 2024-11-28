import sys
import ollama
import chromadb
from utilities import getconfig

# Constants for embedding and main models
EMBED_MODEL = "nomic-embed-text"
MAIN_MODEL = "llama3.2:latest"

# Initialize ChromaDB client and collection
chroma = chromadb.HttpClient(host="localhost", port=8000)
collection = chroma.get_or_create_collection("buildragwithpython")

# Get the query from command-line arguments
query = " ".join(sys.argv[1:])

# Ensure the query is not empty
if not query.strip():
    print("Error: Query is empty or invalid")
    sys.exit(1)

# Get the embeddings for the query
query_embed = ollama.embeddings(model=EMBED_MODEL, prompt=query)['embedding']

# Retrieve relevant documents from the collection
relevant_docs = collection.query(query_embeddings=[query_embed], n_results=5)["documents"][0]

# Join the relevant documents into a single string
docs = "\n\n".join(relevant_docs)

# Print the relevant documents
print(docs)
