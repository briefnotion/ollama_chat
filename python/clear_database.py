import sys, ollama, chromadb, time

# Initialize the client
chroma = chromadb.HttpClient(host="localhost", port=8000)

# List all collections
collections = chroma.list_collections()

print("Clearing Database")

# Print all collection names
for collection in collections:
    print(f'Collection name: {collection.name}')

# Delete all collections
for collection in collections:
    print(f'Deleting collection: {collection.name}')
    chroma.delete_collection(collection.name)

print("")
