#ifndef VECTORDB_API_CPP
#define VECTORDB_API_CPP


#include "vectordb_api.h"

//using namespace std;

using json = nlohmann::json;


// ------------------------------------------------------------------------- //

/*

import ollama, chromadb, time
from utilities import readtext, getconfig
from mattsollamatools import chunker, chunk_text_by_sentences
import nltk
nltk.download('punkt_tab')

collectionname="buildragwithpython"

chroma = chromadb.HttpClient(host="localhost", port=8000)
print(chroma.list_collections())
if any(collection.name == collectionname for collection in chroma.list_collections()):
  print('deleting collection')
  chroma.delete_collection("buildragwithpython")
collection = chroma.get_or_create_collection(name="buildragwithpython", metadata={"hnsw:space": "cosine"})

embedmodel = getconfig()["embedmodel"]
starttime = time.time()
with open('sourcedocs.txt') as f:
  lines = f.readlines()
  for filename in lines:
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

*/


int VECTORDB_API::embedding_test_1()
{
  // chromadb

  /*
  cout << "Varable Assigment" << endl;
  std::string embedModel = "nomic-embed-text";
  std::string mainModel = "llama3.1:8b";

  // Initialize ChromaDB client
  cout << "Initialize Assigment" << endl;
  chromadb::Client client("http", "localhost", "8000");

  cout << "Print Info" << endl;
	std::cout << client.GetVersion() << std::endl;
	std::cout << client.GetHeartbeat() << std::endl;
  */
  return 0;

      

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

/*
  cout << "Varable Assigment" << endl;
  std::string embedModel = "nomic-embed-text";
  std::string mainModel = "llama3.1:8b";

  // Initialize ChromaDB client
  cout << "Initialize Assigment" << endl;
  chromadb::Client client("http", "localhost", "8000");

  cout << "Print Info" << endl;
	std::cout << client.GetVersion() << std::endl;
	std::cout << client.GetHeartbeat() << std::endl;
*/

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
}

// ------------------------------------------------------------------------- //

/*
json post_request(const std::string& url, const json& jsonData) 
{
  httplib::Client cli("http://localhost:8000");

  //cout << "JSONDUMP: " << jsonData.dump() << endl;
  //std::cout<< std::setw(2) << jsonData << endl;

  json data1 = jsonData["embeddings"][0];

  cout << "EMB: \n" << data1.dump() << endl;

  //exit(0);

  //auto res = cli.Post(url.c_str(), jsonData.dump(), "application/json");
  auto res = cli.Post(url.c_str(), data1.dump(), "application/json");



  json jsonResponse;

  if (res && res->status == 200) 
  {
    jsonResponse = json::parse(res->body);
  } else 
  {
    std::cerr << "POST request failed with status: " << res->status << std::endl;
  }

  return jsonResponse;
}
*/

int VECTORDB_API::embedding_test_2()
{
  /*
  // Fetch configuration values
  std::string embedModel = "nomic-embed-text";
  std::string mainModel = "llama3.1:8b";


  Ollama ollama;
  ollama.setServerURL("http://localhost:11434");
  ollama.setReadTimeout(120);
  ollama.setWriteTimeout(120);

  // Ask for the query input
  std::string query;
  std::cout << "Enter your question: ";
  std::getline(std::cin, query);

  if (query.empty())
  {
    std::cerr << "Query is empty or invalid" << std::endl;
    return 1;
  }

  // Generate embeddings
  json embedData;
  embedData["model"] = embedModel;
  embedData["prompt"] = query;

  cout << "Embedding request: " << embedData.dump() << endl;

  cout << "A" << endl;
  ollama::response response = ollama::generate_embeddings("nomic-embed-text", "Why is the sky blue?");
  cout << "B" << endl;
  //cout << "GENERATED: " << response.as_json_string() << endl;
  cout << "C" << endl;

  //json embedResponse = post_request("/embeddings", embedData);
  //json embedResponse = post_request("/api/v1/collections/", embedData);
  //json embedResponse = post_request("/api/v1/collections/b224e957-026a-45dd-a65c-7d90c15d005f/query", embedData);
  //json embedResponse = post_request("/api/v1/collections/", response);
  json embedResponse = post_request("/api/v1/collections/b224e957-026a-45dd-a65c-7d90c15d005f/query", response.as_json());

  if (!embedResponse.contains("embedding") || embedResponse["embedding"].empty()) 
  {
    std::cerr << "Failed to generate embedding" << std::endl;
    return 1;
  }

  
  std::vector<float> queryEmbedding;
  for (const auto& val : embedResponse["embedding"]) 
  {
    queryEmbedding.push_back(val.get<float>());
  }
  if (queryEmbedding.empty()) 
  {
    std::cerr << "Generated embedding is empty" << std::endl;
    return 1;
  }

  // Simulated local vector database
  std::vector<std::vector<float>> localVectorDatabase; // Add your vectors here
  std::vector<std::string> documents; // Corresponding documents

  // Query the local vector database
  std::vector<std::pair<int, float>> distances;
  for (size_t i = 0; i < localVectorDatabase.size(); ++i)
  {
    float distance = 0;
    for (size_t j = 0; j < queryEmbedding.size(); ++j)
    {
      distance += (queryEmbedding[j] - localVectorDatabase[i][j]) * (queryEmbedding[j] - localVectorDatabase[i][j]);
    }
    distances.emplace_back(i, distance);
  }

  // Sort by distance
  std::sort(distances.begin(), distances.end(), [](auto& left, auto& right)
            {
              return left.second < right.second;
            });

  // Retrieve the top 5 closest documents
  std::vector<std::string> relevantDocs;
  for (int i = 0; i < 5 && i < distances.size(); ++i)
  {
    relevantDocs.push_back(documents[distances[i].first]);
  }

  // Prepare documents for model query
  std::string docs;
  for (const auto& doc : relevantDocs)
  {
    docs += doc + "\n\n";
  }

  std::cout << "Retrieved documents: " << docs << std::endl;

  // Prepare model query
  std::string modelQuery = query + " - Answer that question using the following text as a resource: " + docs;

  // Generate response using the main model
  json modelData;
  modelData["model"] = mainModel;
  modelData["prompt"] = modelQuery;

  json responseStream = post_request("/generate", modelData);
  for (const auto& chunk : responseStream["chunks"])
  {
    if (chunk.contains("response"))
    {
      std::cout << chunk["response"].get<std::string>();
    }
  }

  exit(0);
  */

  return 0;
}


int VECTORDB_API::embedding_test_3()
{

  //exit(0);

  return 0;

}

// ------------------------------------------------------------------------- //

/*
int VECTORDB_API::embedding_test_cjson_1()
{
  const char *baseUrl = "http://localhost:8000";

  // Test Connection to the chromadb host
  testConnection(baseUrl);
  const char *collectionName = "TestCollection";

  // Create a Collection
  printf("\n\nCreate Collection\n");
  int result = createCollection(baseUrl, collectionName);
  if (result) {
    printf("Collection created successfully.\n");
  } else {
    printf("Failed to create collection.\n");
  }

  // Get Collection
  MemoryStruct response = getCollection(baseUrl, collectionName);
  printf("\n\nGet Collection\n");
  if (response.size > 0) {
    Collection collection = parseCollectionResponse(response.memory);
    if (collection.id != NULL && collection.name != NULL) {
      printf("Collection ID: %s\n", collection.id);
      printf("Collection Name: %s\n", collection.name);
    }
    freeCollection(&collection);
  } else {
    printf("Collection not found or an error occurred.\n");
  }

  free(response.memory);



  return 0;
}
*/

// ------------------------------------------------------------------------- //
string VECTORDB_API::callPythonScript() 
{
  // Call the Python script
  system("python3 ../python/script.py");

  // Read the result from the file
  ifstream file("output.txt");
  string result;

  if (file.is_open()) {
    getline(file, result);
    file.close();
  } else {
    result = "Error: unable to open file";
  }

  return result;
}

int VECTORDB_API::embedding_test_python_1()
{
  string result = callPythonScript();
  cout << "Result from Python script: " << result << endl;

  return 1;
}


string VECTORDB_API::exec(const char* cmd) 
{
  array<char, 128> buffer;
  string result;
  unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe) 
  {
    throw runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) 
  {
    result += buffer.data();
  }
  return result;
}

int VECTORDB_API::embedding_test_python_2() 
{
  string command = "python3 ../python/script.py";
  string result = exec(command.c_str());
  cout << "Result from Python script: " << result << endl;
    
  return 1;
}

int VECTORDB_API::embedding_test_python_3() 
{
  string andand = "&& ";

  string bash = "bash -c '";

  string environment = "source /home/briefn/py/venv/bin/activate ";
  string script = "python3 ../python/search.py ";
  string question = "what happened in tiawan?";

  string enviornement_stop = "deactivate";

  string command = environment + andand + script + question;
  //string command = script + question;
  //string command = environment;

  string bcommand = bash + command + "'";

  cout << bcommand << endl;

  string result = exec(bcommand.c_str());
  cout << "Result from Python script: " << result << endl;
    
  return 1;
}



// ------------------------------------------------------------------------- //

#endif // CHROMADB_API_CPP