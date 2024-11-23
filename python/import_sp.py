import sys, ollama, chromadb, time
from utilities import readtext, getconfig
from mattsollamatools import chunker, chunk_text_by_sentences
import nltk
nltk.download('punkt_tab')

collectionname="buildragwithpython"

chroma = chromadb.HttpClient(host="localhost", port=8000)
print(chroma.list_collections())
#if any(collection.name == collectionname for collection in chroma.list_collections()):
#  print('deleting collection')
#  chroma.delete_collection("buildragwithpython")
collection = chroma.get_or_create_collection(name="buildragwithpython", metadata={"hnsw:space": "cosine"})

#embedmodel = getconfig()["embedmodel"]
embedmodel = "nomic-embed-text"

starttime = time.time()
#with open('sourcedocs.txt') as f:
#  lines = f.readlines()
#  for filename in lines:

filenames = sys.argv[1:]

for filename in filenames:
  print(filename)
  text = readtext(filename)
  chunks = chunk_text_by_sentences(source_text=text, sentences_per_chunk=7, overlap=0, language = "english" )
  print(f"with {len(chunks)} chunks")
  for index, chunk in enumerate(chunks):
    embed = ollama.embeddings(model=embedmodel, prompt=chunk)['embedding']
    #print(".", end="", flush=True)
    #print("---   " + chunk + "EMBED: " + embed + "   ---")
    print("---   " + chunk + "   ---")
    collection.add([filename+str(index)], [embed], documents=[chunk], metadatas={"source": filename})

  print("--- %s seconds ---" % (time.time() - starttime))

