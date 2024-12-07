import sys
import ollama
import chromadb
import numpy as np
from sklearn.metrics.pairwise import cosine_similarity
from utilities import getconfig

def calculate_similarity(query_embed, doc_embed):
    return cosine_similarity([query_embed], [doc_embed])[0][0]

# Constants for embedding and main models
EMBED_MODEL = "nomic-embed-text"
MAIN_MODEL = "llama3.2:latest"

def main():
    # Get the query from command-line arguments
    collection_name = sys.argv[1]
    query = " ".join(sys.argv[2:])

    # Initialize ChromaDB client and collection
    chroma = chromadb.HttpClient(host="localhost", port=8000)
    collection = chroma.get_or_create_collection(collection_name)

    # Ensure the query is not empty
    if not query.strip():
        print("Error: Query is empty or invalid")
        sys.exit(1)

    # Get the embeddings for the query
    query_embed = ollama.embeddings(model=EMBED_MODEL, prompt=query)['embedding']

    # Retrieve relevant documents from the collection
    relevant_docs = collection.query(query_embeddings=[query_embed], n_results=5)["documents"][0]

    # Relevant Document Count
    doc_count = 0

    # Calculate similarity scores for each document
    for doc in relevant_docs:
        doc_embed = ollama.embeddings(model=EMBED_MODEL, prompt=doc)['embedding']
        similarity_score = calculate_similarity(query_embed, doc_embed)

        if similarity_score > .7:
            print(f"{doc}\n")
            doc_count = doc_count + 1

    # If nothing relevant found, relay the info
    if doc_count == 0:
        print("!No Relevant Documents")

if __name__ == "__main__":
    main()
