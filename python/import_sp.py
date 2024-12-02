import sys
import os
import ollama
import chromadb
import time
import nltk
from utilities import readtext, getconfig
from mattsollamatools import chunker, chunk_text_by_sentences

# Download NLTK data
nltk.download('punkt')

# Constants
COLLECTION_NAME = "buildragwithpython"
EMBED_MODEL = "nomic-embed-text"

def main(filenames):
    # Initialize ChromaDB client
    chroma = chromadb.HttpClient(host="localhost", port=8000)
    
    # Print existing collections
    print("Existing collections:", chroma.list_collections())
    
    # Get or create the collection
    collection = chroma.get_or_create_collection(name=COLLECTION_NAME, metadata={"hnsw:space": "cosine"})

    starttime = time.time()

    # Process each file
    for filename in filenames:
        process_file(filename, collection)

    print("--- %s seconds ---" % (time.time() - starttime))

def process_file(filename, collection):
    """Process a single file and add chunks to the collection"""
    print(f"Processing file: {filename}")
    
    text = readtext(filename)
    chunks = chunk_text_by_sentences(source_text=text, sentences_per_chunk=7, overlap=0, language="english")
    print(f"File contains {len(chunks)} chunks")

    for index, chunk in enumerate(chunks):
        embed = ollama.embeddings(model=EMBED_MODEL, prompt=chunk)['embedding']
        print(f"---   {chunk}   ---")
        collection.add([f"{filename}{index}"], [embed], documents=[chunk], metadatas={"source": filename})

if __name__ == "__main__":
    filenames = sys.argv[1:]
    if not filenames:
        print("No files provided. Please provide filenames as command-line arguments.")
    else:
        # Process each file
        for filename in filenames:
            if os.path.isfile(filename):
                main(filenames)
            else:
                print(f"File not found: {filename}")


        
