#ifndef VECTORDB_API_CPP
#define VECTORDB_API_CPP


#include "chromadb_api.h"

using namespace std;

// ------------------------------------------------------------------------- //


int CHROMADB_API::embedding_test_1()
{
  /*

  #embedmodel=nomic-embed-text
  #mainmodel=llama3.1:8b

  import ollama, sys, chromadb
  from utilities import getconfig

  embedmodel = getconfig()["embedmodel"]
  mainmodel = getconfig()["mainmodel"]
  chroma = chromadb.HttpClient(host="localhost", port=8000)
  collection = chroma.get_or_create_collection("buildragwithpython")

  query = input("Enter your question: ")
  if not query.strip():
          print("Query is empty or invalid")
          #return

  #query = " ".join(sys.argv[1:])

  queryembed = ollama.embeddings(model=embedmodel, prompt=query)['embedding']


  relevantdocs = collection.query(query_embeddings=[queryembed], n_results=5)["documents"][0]
  docs = "\n\n".join(relevantdocs)
  modelquery = f"{query} - Answer that question using the following text as a resource: {docs}"

  stream = ollama.generate(model=mainmodel, prompt=modelquery, stream=True)

  for chunk in stream:
    if chunk["response"]:
      print(chunk['response'], end='', flush=True)
*/


  cout << "Varable Assigment" << endl;
  std::string embedModel = "nomic-embed-text";
  std::string mainModel = "llama3.1:8b";

  // Initialize ChromaDB client
  cout << "Initialize Assigment" << endl;
  chromadb::Client client("http", "localhost", "8000");

  cout << "Print Info" << endl;
	std::cout << client.GetVersion() << std::endl;
	std::cout << client.GetHeartbeat() << std::endl;

/*
  chromadb::Collection collection = client.CreateCollection("buildragwithpython");

  // Ask for the query input
  std::string query;
  std::cout << "Enter your question: ";
  std::getline(std::cin, query);

  if (query.empty()) {
      std::cerr << "Query is empty or invalid" << std::endl;
      return 1;
  }

  // Generate embeddings
  chromadb::Embedding queryEmbedding = client.generate_embedding(embedModel, query);
  if (queryEmbedding.empty()) {
      std::cerr << "Generated embedding is empty" << std::endl;
      return 1;
  }

  // Query ChromaDB
  auto results = collection.query(queryEmbedding, 5);
  if (results.empty()) {
      std::cerr << "No relevant documents found" << std::endl;
      return 1;
  }

  std::string docs;
  for (const auto& doc : results) {
      docs += doc + "\n\n";
  }

  std::cout << "Retrieved documents: " << docs << std::endl;

  // Prepare model query
  std::string modelQuery = query + " - Answer that question using the following text as a resource: " + docs;

  // Generate response using the main model
  auto responseStream = client.generate_response(mainModel, modelQuery);
  for (const auto& chunk : responseStream) {
      if (!chunk.response.empty()) {
          std::cout << chunk.response << std::flush;
      }
  }
*/

  return 1;
}


// ------------------------------------------------------------------------- //

#endif