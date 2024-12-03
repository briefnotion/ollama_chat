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

# Description
print("""
This program processes text files in a specified directory or a single file. 
It reads the content, splits the text into chunks, and adds these chunks to a ChromaDB collection.
The embedding model 'nomic-embed-text' is used to generate embeddings for each text chunk.
If a directory is provided, the program processes each file in the directory.
If a single file is provided, it processes only that file.
Finally, the program prints the time taken to process all files.
""")

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
        #print(f"---   {chunk}   ---")
        collection.add([f"{filename}{index}"], [embed], documents=[chunk], metadatas={"source": filename})

if __name__ == "__main__":
    input_path = sys.argv[1]
    if not input_path:
        print("No input provided. Please provide a directory or filename as a command-line argument.")
    elif os.path.isdir(input_path):
        # If input is a directory, process each file in the directory
        for filename in os.listdir(input_path):
            file_path = os.path.join(input_path, filename)
            if os.path.isfile(file_path):
                main([file_path])
    elif os.path.isfile(input_path):
        # If input is a filename, process the single file
        main([input_path])
    else:
        print(f"Input not found: {input_path}")
