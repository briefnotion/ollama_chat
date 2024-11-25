import sys, chromadb

# Initialize the client
chroma = chromadb.HttpClient(host="localhost", port=8000)

# List all collections
collections = chroma.list_collections()

print("Listing Database")

# Print all collection names
for collection in collections:
    print(f'Collection name: {collection.name}')

print("")
