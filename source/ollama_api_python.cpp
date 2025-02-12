#ifndef OLLAMA_API_PYTHON_CPP
#define OLLAMA_API_PYTHON_CPP

#include "ollama_api_python.h"

using namespace std;

// ------------------------------------------------------------------------- //

void OLLAMA_API_MUTEX_PYTHON::set_command_line(string Command)
{
  lock_guard<mutex> lock(MUTEX_);
  //COMPLETE_RESPONSE = "";
  COMMAND_LINE = Command;
}

string OLLAMA_API_MUTEX_PYTHON::command_line() const
{
  lock_guard<mutex> lock(MUTEX_);
  return COMMAND_LINE;
}

string OLLAMA_API_MUTEX_PYTHON::get_complete_response() const
{
  lock_guard<mutex> lock(MUTEX_);

  return COMPLETE_RESPONSE;
}

void OLLAMA_API_MUTEX_PYTHON::set_complete_response_after_done(nlohmann::json Response)
{
  lock_guard<mutex> lock(MUTEX_);

  COMPLETE_RESPONSE_AFTER_DONE = Response;
  COMPLETE_RESPONSE_READY_AFTER_DONE = true;
}

nlohmann::json OLLAMA_API_MUTEX_PYTHON::get_complete_response_after_done()
{
  lock_guard<mutex> lock(MUTEX_);

  COMPLETE_RESPONSE_READY_AFTER_DONE = false;
  CHANGED = false;

  return COMPLETE_RESPONSE_AFTER_DONE;
}

bool OLLAMA_API_MUTEX_PYTHON::complete_response_ready_after_done() const
{
  lock_guard<mutex> lock(MUTEX_);

  if (CHANGED)
  {
    return COMPLETE_RESPONSE_READY_AFTER_DONE;
  }
  else
  {
    return false;
  }
}

int OLLAMA_API_MUTEX_PYTHON::done() const
{
  lock_guard<mutex> lock(MUTEX_);
  return DONE;
}

void OLLAMA_API_MUTEX_PYTHON::set_done(int Done)
{
  lock_guard<mutex> lock(MUTEX_);
  DONE = Done;

  if (DONE == OLLAMA_API_RESPONSE_DONE)
  {
    CHANGED = true;
  }
}

void OLLAMA_API_MUTEX_PYTHON::add_to_response(const string& value) 
{
  lock_guard<mutex> lock(MUTEX_);
  RESPONSE_VECTOR.push_back(value);
}

int OLLAMA_API_MUTEX_PYTHON::response_size() const
{
  lock_guard<mutex> lock(MUTEX_);
  return RESPONSE_VECTOR.size();
}

void OLLAMA_API_MUTEX_PYTHON::get_response_to_vector(vector<string> &Receiving_Vector)
{
  lock_guard<mutex> lock(MUTEX_);

  if (RESPONSE_VECTOR.size() > 0)
  {
    for (int i = 0; i < (int)RESPONSE_VECTOR.size(); ++i) 
    {
      Receiving_Vector.push_back(RESPONSE_VECTOR[i]);
    }
  }
  
  RESPONSE_VECTOR.clear();
}

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //


// ------------------------------------------------------------------------- //

string OLLAMA_API_PYTHON::exec(const char* cmd) 
{
  array<char, 128> buffer;
  string result;
  
  // Use a lambda to avoid the warning
  unique_ptr<FILE, function<int(FILE*)>> pipe(popen(cmd, "r"), [](FILE* fp) { return pclose(fp); });
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

void OLLAMA_API_PYTHON::exec_thread_question() 
{
  array<char, 128> buffer;

  // Use a lambda to avoid the warning
  unique_ptr<FILE, function<int(FILE*)>> pipe(popen(OLLAMA_MUTEX.command_line().c_str(), "r"), [](FILE* fp) { return pclose(fp); });

  OLLAMA_MUTEX.set_done(OLLAMA_API_RESPONS_GENERATING);

  if (!pipe) 
  {
    //OLLAMA_MUTEX.add_to_response("PROBLEM WITH: " + OLLAMA_MUTEX.command_line() + "\n");
    OLLAMA_MUTEX.set_done(OLLAMA_API_READY_FOR_REQUEST);
    throw runtime_error("popen() failed!");
  }

  // does not generate error if file not found

  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) 
  {
    OLLAMA_MUTEX.add_to_response(buffer.data());
  }

  OLLAMA_MUTEX.set_complete_response_after_done(nlohmann::json::parse(file_to_string(EXCHANGE_DIRECTORY + PROPS.RESPONSE_JSON_FILENAME)));
  //
  OLLAMA_MUTEX.set_done(OLLAMA_API_RESPONSE_DONE);
}

void OLLAMA_API_PYTHON::thread()
{
  //if (PYTHON_QUESTION_RESPONSE_MUTEX.done() == VECTORDB_API_READY_FOR_REQUEST || 
  //    PYTHON_QUESTION_RESPONSE_MUTEX.done() == VECTORDB_API_WRITING_REQUEST)
  if (OLLAMA_MUTEX.done() == OLLAMA_API_READY_FOR_REQUEST)
  {
    //exec_thread_question();

    // Be careful with this because it looks like black magic to me.
    PYTHON_QUESTION_RESPONSE_THREAD.start_render_thread([&]() 
                  {  exec_thread_question();  });
  }
}

// ------------------------------------------------------------------------- //

void OLLAMA_API_PYTHON::clean_chat_conversation()
{
  if (!CONVERSATION.empty())
  {
    // Remove entries with "role": "tool"
    CONVERSATION.erase(std::remove_if(CONVERSATION.begin(), CONVERSATION.end(),
                                      [](const nlohmann::json& entry) 
                                      {
                                          return entry["role"] == "tool";
                                      }),
                        CONVERSATION.end());

  }
}

nlohmann::json OLLAMA_API_PYTHON::build_generate_request(string Request)
{
  nlohmann::json request;

  // build header
  {
    request["model"] = PROPS.MODEL; 
  }

  // add messages
  {
    // method generate.
    request["prompt"] = Request;
    if (!CONVERSATION_CONTEXT.empty())
    {
      request["context"] = CONVERSATION_CONTEXT;
    }
  }

  // options
  {
    request["stream"] = true;
    //request["format"] = "json";
    request["tool_choice"] = "auto";    // Default auto, any, none // Not seeing this do anything
    request["function_call"] = "auto";  // Not seeing this do anything
    request["keep_alive"] = 3600;

    // Additional Options
    request["options"] = {{"num_ctx", 4096}};
    
    //request["stop_sequences"] = "";
    //request["max_tokens"] = 2048;
    //request["temperature"] = 1.0;
    //request["top_p"] = 1.0;
    //request["presence_penalty"] = 0.0;
    //request["frequency_penalty"] = 0.0;
    //request["logit_bias"] = {};
  }

  return request; // Return the built request
}

nlohmann::json OLLAMA_API_PYTHON::build_chat_request(string Role_1, string Name_1, string Content_1, 
                                                string Role_2, string Name_2, string Content_2, 
                                                bool Enable_Tool_Function, nlohmann::json Message_Append)
{
  nlohmann::json request;

  // testing
  Enable_Tool_Function = true;

  // modify with names
  {
    if (!Name_1.empty()) 
    {
      Content_1 = Name_1 + ": " + Content_1; // Prefix the content with the name
    }
    if (!Name_2.empty()) 
    {
      Content_2 = Name_2 + ": " + Content_2; // Prefix the content with the name
    }
  }

  // build header
  {
    request["model"] = PROPS.MODEL; 
  }

  // tools
  if (Enable_Tool_Function) 
  {
    nlohmann::json tools_list;

    if (TOOLS.WEATHER_TOOL_SUBMIT.submit())
    {
      tools_list.push_back(TOOLS.weather_tool());
    }

    if (TOOLS.CLOCK_TOOL_SUBMIT.submit())
    {
      tools_list.push_back(TOOLS.clock_tool());
    }

    if (TOOLS.DATE_TOOL_SUBMIT.submit())
    {
      tools_list.push_back(TOOLS.date_tool());
    }


    if (!tools_list.empty())
    {
      request["tools"] = tools_list;
    }
  }

  // add messages
  {

    nlohmann::json message_1;
    nlohmann::json message_2;

    int append_position = 1;

    // Before
    if (append_position == 0)
    {
      if (!Message_Append.empty())
      {
        for (const auto& message : Message_Append)
        {
          CONVERSATION.push_back(message);
        }
      }
    }

    // Rebuild conversation.
    message_1 = {{"role", Role_1}, {"content", Content_1}};
    message_2 = {{"role", Role_2}, {"content", Content_2}};

    if (Role_1 != "")
    {
      CONVERSATION.push_back(message_1);
    }

    if (Role_2 != "") // single message
    {
      CONVERSATION.push_back(message_2);
    }

    // After
    if (append_position == 1)
    {
      if (!Message_Append.empty())
      {
        for (const auto& message : Message_Append)
        {
          CONVERSATION.push_back(message);
        }
      }
    }

    request["messages"] = CONVERSATION;
  }

  // options
  {
    request["stream"] = true;
    //request["format"] = "json";
    request["tool_choice"] = "auto";    // Default auto, any, none // Not seeing this do anything
    request["function_call"] = "auto";  // Not seeing this do anything
    request["keep_alive"] = 3600;

    // Additional Options
    request["options"] = {{"num_ctx", 4096}};
    
    //request["stop_sequences"] = "";
    //request["max_tokens"] = 2048;
    //request["temperature"] = 1.0;
    //request["top_p"] = 1.0;
    //request["presence_penalty"] = 0.0;
    //request["frequency_penalty"] = 0.0;
    //request["logit_bias"] = {};
  }

  return request; // Return the built request
}

void OLLAMA_API_PYTHON::submit_generate_request(string Request, 
                                        bool Output_To_Response, bool Consider_Context, 
                                        bool Remember_Context)
{
  if (CONNECTION_STATUS == OLLAMA_SERVER_CONNECTED)
  {
    if (OLLAMA_MUTEX.done() == OLLAMA_API_READY_FOR_REQUEST)
    {
      ALLOW_OUTPUT = Output_To_Response;
      CONSIDER_CONTEXT = Consider_Context;
      REMEMBER_CONTEXT = Remember_Context;

      // Take a snapshot of the conversation, in case it needs to be undone by the tool function.
      //CONVERSATION_SNAP_SHOT = CONVERSATION;
      //ROLE_1_SNAP_SHOT = Role_1;
      //NAME_1_SNAP_SHOT = Name_1;
      //QUESTION_1_SNAP_SHOT = Question_1;
      //ROLE_2_SNAP_SHOT = Role_2;
      //NAME_2_SNAP_SHOT = Name_2;
      //QUESTION_2_SNAP_SHOT = Question_2;

      REQUEST = build_generate_request(Request);

      string_to_file((EXCHANGE_DIRECTORY + PROPS.REQUEST_JSON_FILENAME), REQUEST.dump(2), false);
      
      RESPONSE_FULL = "";

      //string bcommand = PROPS.ENVIRONMENT + aa + PROPS.TEST + sp;
      string bcommand = PROPS.ENVIRONMENT + aa + PROPS.REQUEST_GENERATE + sp + 
                                    PROPS.MODEL + sp + 
                                    (EXCHANGE_DIRECTORY + PROPS.REQUEST_JSON_FILENAME) + sp + 
                                    (EXCHANGE_DIRECTORY + PROPS.RESPONSE_JSON_FILENAME);

      if (PROPS.BASH_SHELL.size() > 0)
      {
        bcommand = PROPS.BASH_SHELL + bcommand + "'";
      }

      OLLAMA_MUTEX.set_command_line (bcommand);

      dump_string(DUMP_DIRECTORY, "pycall.txt", bcommand);

      // Remove any unnecessary items from the conversation 
      clean_chat_conversation();

      thread();
    }
  }
}

void OLLAMA_API_PYTHON::submit_chat_request(string Role_1, string Name_1, string Question_1, 
                                        string Role_2, string Name_2, string Question_2, 
                                        bool Output_To_Response, bool Consider_Context, 
                                        bool Remember_Context, bool Enable_Tool_Function, 
                                        nlohmann::json Message_Append)
{
  if (CONNECTION_STATUS == OLLAMA_SERVER_CONNECTED)
  {
    if (OLLAMA_MUTEX.done() == OLLAMA_API_READY_FOR_REQUEST)
    {
      ALLOW_OUTPUT = Output_To_Response;
      CONSIDER_CONTEXT = Consider_Context;
      REMEMBER_CONTEXT = Remember_Context;

      // Take a snapshot of the conversation, in case it needs to be undone by the tool function.
      CONVERSATION_SNAP_SHOT = CONVERSATION;
      ROLE_1_SNAP_SHOT = Role_1;
      NAME_1_SNAP_SHOT = Name_1;
      QUESTION_1_SNAP_SHOT = Question_1;
      ROLE_2_SNAP_SHOT = Role_2;
      NAME_2_SNAP_SHOT = Name_2;
      QUESTION_2_SNAP_SHOT = Question_2;

      REQUEST = build_chat_request(Role_1, Name_1, Question_1, Role_2, Name_2, Question_2, Enable_Tool_Function, Message_Append);

      string_to_file((EXCHANGE_DIRECTORY + PROPS.REQUEST_JSON_FILENAME), REQUEST.dump(2), false);
      
      RESPONSE_FULL = "";

      //string bcommand = PROPS.ENVIRONMENT + aa + PROPS.TEST + sp;
      string bcommand = PROPS.ENVIRONMENT + aa + PROPS.REQUEST_CHAT + sp + 
                                    PROPS.MODEL + sp + 
                                    (EXCHANGE_DIRECTORY + PROPS.REQUEST_JSON_FILENAME) + sp + 
                                    (EXCHANGE_DIRECTORY + PROPS.RESPONSE_JSON_FILENAME);

      if (PROPS.BASH_SHELL.size() > 0)
      {
        bcommand = PROPS.BASH_SHELL + bcommand + "'";
      }

      OLLAMA_MUTEX.set_command_line (bcommand);

      dump_string(DUMP_DIRECTORY, "pycall.txt", bcommand);

      // Remove any unnecessary items from the conversation 
      clean_chat_conversation();

      thread();
    }
  }
}

void OLLAMA_API_PYTHON::create() // ↑ ↓ → ←
{
  
  CONNECTION_STATUS =  true;
  CONNECTION_STATUS_CHANGED = true;

  /*
  bool connection_status = CONNECTION_STATUS;

  // Optional. By default, the server URL is set to http://localhost:11434. 
  // Use this function if your server resides at a different URL.
  OLLAMA.setServerURL(PROPS.URL);
  //OLLAMA.setServerURL("http://127.0.0.1:11434");
  
  // Optional. Set the read and write timeouts in seconds for server interactions.
  // If you have a large model with a long response time you may need to increase these.
  OLLAMA.setReadTimeout(120);
  OLLAMA.setWriteTimeout(120);

  if (OLLAMA.is_running())
  {
    bool success = true;  // Not implemented.

    string thing = "";

    RESPONSE_STRING_VECTOR.push_back("Version: " + OLLAMA.get_version() + "\n");
    //cout << "Version: " << OLLAMA.get_version() << endl;

    // This can optionally be used to deliberately load a model into memory prior to use. 
    // This occurs automatically when a request is made for an unloaded model, but can be 
    // useful for loading a model in advance.

    success = OLLAMA.load_model(PROPS.MODEL);
    //Output_Container.add_to( "l:" + to_string(success) + "\n", Output_Focus);

    //string em = "claude-3-5-sonnet-20240620";
    //string em = "nomic-embed-text";
    //success = OLLAMA.load_model(em);
    //OLLAMA.load_model(em);
    //Output_Container.add_to( "l:" + to_string(success) + "\n", Output_Focus);

    // Pull a model by specifying a model name.
    // bool model_pulled = OLLAMA.pull_model("llama3:8b");

    // Copy a model by specifying a source model and destination model name.
    // bool model_copied = OLLAMA.copy_model("llama3:8b", "llama3_copy");

    // Delete a model by specifying a model name.
    // bool model_deleted = OLLAMA.delete_model("llama3_copy");

    // Request model info from the OLLAMA server.
    nlohmann::json model_info = OLLAMA.show_model_info(PROPS.MODEL);

    // Ensure "details" and "family" exist and are not null
    if (model_info.contains("details") && 
        !model_info["details"].is_null() && 
        model_info["details"].contains("family") && 
        !model_info["details"]["family"].is_null()) 
    {
      //cout << "Model family is " << model_info["details"]["family"] << endl;
      thing = model_info["details"]["family"];
      RESPONSE_STRING_VECTOR.push_back( "Model family is " + thing + "\n");

      // List the models available locally in the OLLAMA server
      OLLAMA_MODELS_LIST = OLLAMA.list_models();
      //print_vector_with_title("Available models", OLLAMA_MODELS_LIST);
      thing = return_vector_as_string(OLLAMA_MODELS_LIST, true);
      RESPONSE_STRING_VECTOR.push_back( "Available models \n" + thing + "\n");

      // List the models available locally in the OLLAMA server
      OLLAMA_MODELS_RUNNING = OLLAMA.list_running_models();
      //print_vector_with_title("Running models", OLLAMA_MODELS_RUNNING);
      thing = return_vector_as_string(OLLAMA_MODELS_RUNNING, true);
      RESPONSE_STRING_VECTOR.push_back( "Running models \n" + thing + "\n");

      // Most calls will throw ollama::exception in the event of an error, with details on the exception that has occurred. Exceptions are enabled by default.    
      //try { 
      //    OLLAMA.generate("Non-existent-model", "Requesting this model will throw an error"); 
      //} 
      //catch(OLLAMA.exception e) { cout << e.what() << endl; }

      // You can also dynamically enable and disable exceptions. If exceptions are disabled, 
      // functions will return an empty OLLAMA.response or false where appropriate instead 
      // of throwing OLLAMA.exception.
      //OLLAMA.allow_exceptions(false);

      // -------------------------------------------------

      // Set Options
      //  https://github.com/ollama/ollama/blob/main/docs/api.md#generate-request-with-options

      //options["seed"] = 1;
      // This sets the random number seed for generation1
      //  Using the same seed with the same prompt will always produce the same response, making the model's output deterministic

      //options["temperature"] = 0;
      // Controls the "wildness" or creativity of the model's responses2
      //  A higher temperature encourages more creative and varied responses, while a lower temperature (like 0) makes the model's output more deterministic and consistent

      //options["num_predict"] = 18;
      // Specifies the maximum number of tokens the model is allowed to generate in a single response

      // ollama
      // Controls the use of an OLL (Orientation of the Last Layer) algorithm.
      // Value: 0-4 (default: 2)
      // Description: Use an OLL algorithm to orient the last layer.

      // penalize_new_line
      // Penalizes the solver when it needs to move multiple pieces across a new line.
      // Value: 0-4 (default: 1)
      // Description: Make the solver try harder to minimize moves on new lines.

      // max_depth
      // Sets the maximum depth (number of moves) for the solver to explore.
      // Value: integer (default: 1000)
      // Description: Adjust the maximum number of moves considered by the solver.

      // speed
      // Controls the generation speed of the solver.
      // Value: float (default: 1.0)
      // Description: Fine-tune the speed at which the solver generates responses.

      // temperature
      // Adjusts the "human-like" quality of generated text.
      // Value: float (default: 0.5)
      // Description: Control how conversational or creative the generated text is.

      // top_p
      // Sets the top-p probability value for generating text.
      // Value: float (default: 0.9)
      // Description: Favor longer, more coherent responses by adjusting the probability threshold.

      // repetition_penalty
      // Controls the penalty for repeated words or phrases in generated text.
      // Value: float (default: 1.0)
      // Description: Reduce repetition and create more varied outputs by increasing this value.

      // stop_sequencing
      // Stops the solver from generating text once it reaches a certain point.
      // Value: integer (default: 1000)
      // Description: Limit the length of generated responses using this option.

      // stop_at_end_of_input
      // Ends the solver's response when it reaches the end of the input prompt.
      // Value: boolean (default: false)
      // Description: Stop generating text once the solver has processed the entire input prompt.

      // show_tokens
      // Shows the tokens used by the solver to generate its response.
      // Value: boolean (default: false)
      // Description: Visualize the tokens used by the solver for debugging or educational purposes.

      // prune_low_confidence_tokens
      // Prunes tokens with low confidence scores from generated text.
      // Value: boolean (default: true)
      // Description: Remove tokens with low confidence scores to improve response quality.

      // prune_high_entropy_tokens
      // Prunes tokens with high entropy scores from generated text.
      // Value: boolean (default: false)
      // Description: Remove tokens with high entropy scores to reduce ambiguity in responses.

      // prune_long_sequences
      // Prunes long sequences of tokens from generated text.
      // Value: integer (default: 10)
      // Description: Limit the length of token sequences to improve response quality.

      // prune_short_sequences
      // Prunes short sequences of tokens from generated text.
      // Value: integer (default: 3)
      // Description: Remove very short token sequences to reduce noise in responses.

      // use_contextualized_embeddings
      // Enables contextualized embeddings for improved language understanding.
      // Value: boolean (default: false)
      // Description: Use contextualized embeddings to capture subtle semantic relationships in text.

      // use_pretrained_models
      // Uses pre-trained models for improved performance and accuracy.
      // Value: boolean (default: true)
      // Description: Leverage pre-trained models to enhance the solver's language understanding capabilities.

      // num_beams
      // Sets the number of beams used by the solver for parallel generation.
      // Value: integer (default: 4)
      // Description: Control the number of parallel generations performed by the solver.

      // max_new_tokens_per_sentence
      // Limits the number of new tokens introduced in each generated sentence.
      // Value: integer (default: 2)
      // Description: Regulate the introduction of new tokens to prevent overwhelming responses.

      // num_sentences_to_generate
      // Specifies the number of sentences to generate for a given input prompt.
      // Value: integer (default: 5)
      // Description: Control the number of generated sentences to produce more varied and coherent responses.

      OPTIONS["num_ctx"] = 8192; 
      // increases the context window size to 8192 tokens. This means the model can consider up to 8192 tokens from the input context when generating responses, which can help improve the coherence and relevance of the output, especially for longer inputs.

      (void)success;
      
      connection_status = OLLAMA_SERVER_CONNECTED;
    } 
    else 
    {
      cout << "Model family is unknown" << endl;
      RESPONSE_STRING_VECTOR.push_back("Model family is unknown\n");
      connection_status = OLLAMA_SERVER_CONNECTION_FAILED;
    }
  }

  // -------------------------------------------------
  
  CONNECTION_STATUS =  connection_status;
  CONNECTION_STATUS_CHANGED = true;
  */
}

/*
string OLLAMA_API_PYTHON::tools()
{
  nlohmann::json tools_list_full;
  nlohmann::json tools_list;

  if (TOOLS.WEATHER_TOOL_SUBMIT.submit())
  {
    tools_list.push_back(TOOLS.weather_tool());
  }

  if (TOOLS.CLOCK_TOOL_SUBMIT.submit())
  {
    tools_list.push_back(TOOLS.clock_tool());
  }

  if (!tools_list.empty())
  {
    tools_list_full["tools"] = tools_list;
  }

  return tools_list_full.dump();
}
*/

void OLLAMA_API_PYTHON::set_status(int Status)
{
  OLLAMA_MUTEX.set_done(Status);
}

int OLLAMA_API_PYTHON::get_status()
{
  return OLLAMA_MUTEX.done();
}


void OLLAMA_API_PYTHON::check()
{
  if (CONNECTION_STATUS_CHANGED == true)
  {
    if (CONNECTION_STATUS == OLLAMA_SERVER_NOT_CONNECTED)
    {
      create();
    }
    else if (CONNECTION_STATUS == OLLAMA_SERVER_CONNECTED)
    {
    }
    else  // OLLAMA_SERVER_CONNECTION_FAILED
    {
    }
    
    CONNECTION_STATUS_CHANGED = false;
  }
}

void OLLAMA_API_PYTHON::submit_request(string Role_1, string Name_1, string Question_1, 
  string Role_2, string Name_2, string Question_2, 
  bool Output_To_Response, bool Consider_Context, 
  bool Remember_Context, bool Enable_Tool_Function, 
  nlohmann::json Message_Append)
{
  if (PROPS.REQUEST_MODE == 0)
  {
    submit_chat_request( Role_1, Name_1, Question_1, Role_2, Name_2, Question_2, 
      Output_To_Response, Consider_Context, Remember_Context, Enable_Tool_Function, 
      Message_Append);
  }
  else if (PROPS.REQUEST_MODE == 1)
  {
    if (!Question_2.empty())
    {
      //say something because this isnt allowed.
    }

    submit_generate_request(Question_1, 
      Output_To_Response, Consider_Context, Remember_Context);
  }
}

void OLLAMA_API_PYTHON::submit_request(string Role_1, string Name_1, string Question_1, 
  string Role_2, string Name_2, string Question_2, 
  bool Output_To_Response, bool Consider_Context, 
  bool Remember_Context, bool Enable_Tool_Function)
{
  nlohmann::json empty_json;
  submit_request(Role_1, Name_1, Question_1, Role_2, Name_2, Question_2, 
                  Output_To_Response, Consider_Context, 
                  Remember_Context, Enable_Tool_Function, empty_json);
}
  
void OLLAMA_API_PYTHON::submit_request(string Role, string Name, string Question, 
  bool Output_To_Response, bool Consider_Context, 
  bool Remember_Context, bool Enable_Tool_Function)
{
  nlohmann::json empty_json;
  submit_request(Role, Name, Question, "", "", "", 
    Output_To_Response, Consider_Context, 
    Remember_Context, Enable_Tool_Function, empty_json);
}

int OLLAMA_API_PYTHON::check_response()
{
  OLLAMA_MUTEX.get_response_to_vector(RESPONSE_STRING_VECTOR);
  return RESPONSE_STRING_VECTOR.size();
}

void OLLAMA_API_PYTHON::check_response_done()
{
  if (OLLAMA_MUTEX.done() == OLLAMA_API_READY_TO_GATHER)
  {
    RESPONSE = OLLAMA_MUTEX.get_complete_response_after_done();
    
    bool tool_calls_found = false;
    bool tool_calls_submittted = false;

    nlohmann::json tool_reply;

    if (RESPONSE.contains("messages"))
    {
      for (const auto& message : RESPONSE["messages"]) 
      {
        // tack on tool_call info if found.
        if (message.contains("tool_calls"))
        {
          // UNDO to Resubmit
          OLLAMA_MUTEX.set_done(OLLAMA_API_READY_FOR_REQUEST);
          CONVERSATION = CONVERSATION_SNAP_SHOT;
          tool_calls_found = true;
  
          dump_string(DUMP_DIRECTORY, "tool_response.json", RESPONSE.dump(2));
  
          if (message["tool_calls"].contains("function"))
          {
            dump_string(DUMP_DIRECTORY, "item_1.txt", message["tool_calls"]["function"].dump(2));
            if (message["tool_calls"]["function"].contains("name"))
            {
  
              // get current weather
              if (message["tool_calls"]["function"]["name"] == "get_current_weather")
              {
                if (message["tool_calls"]["function"].contains("arguments"))
                {
                  if (message["tool_calls"]["function"]["arguments"].contains("format") &&
                      message["tool_calls"]["function"]["arguments"].contains("location"))
                  {
                    TOOLS.WEATHER_TOOL_PARAM_FORMAT = message["tool_calls"]["function"]["arguments"]["format"];
                    TOOLS.WEATHER_TOOL_PARAM_LOCATION = message["tool_calls"]["function"]["arguments"]["location"];
  
                    tool_calls_submittted = true;
                    tool_reply.push_back(TOOLS.weather_tool_reply());
                  }
                }
              }
  
              // get current time
              if (message["tool_calls"]["function"]["name"] == "get_current_time")
              {
                tool_calls_submittted = true;
                tool_reply.push_back(TOOLS.clock_tool_reply());
              }
  
              // get current time
              if (message["tool_calls"]["function"]["name"] == "get_current_date")
              {
                tool_calls_submittted = true;
                tool_reply.push_back(TOOLS.date_tool_reply());
              }
  
            }
          }
        }
        else if (message.contains("role"))
        {
          //dump_string(DUMP_DIRECTORY, "tool_respons.json", RESPONSE.dump(2));
          // If no tool call found, store the response.
          if (message["content"] != "")
          {
            if (REMEMBER_CONTEXT)
            {
              // resubmit without tool info
              CONVERSATION.push_back(message);
            }
          }
        }
        else if (message.contains("role"))
        {
          //dump_string(DUMP_DIRECTORY, "tool_respons.json", RESPONSE.dump(2));
          // If no tool call found, store the response.
          if (message["content"] != "")
          {
            if (REMEMBER_CONTEXT)
            {
              // resubmit without tool info
              CONVERSATION.push_back(message);
            }
          }
        }
  
      }

      // Context
      if (RESPONSE.contains("context"))
      {
        dump_string(DUMP_DIRECTORY, "context_response.json", RESPONSE.dump(2));
        CONVERSATION_CONTEXT = RESPONSE["context"];
      }
    
      // Tool Calls
      if (tool_calls_found)
      {
        if (tool_calls_submittted)
        {
          // Resubmit question with tool reply
          submit_request(ROLE_1_SNAP_SHOT, NAME_1_SNAP_SHOT, QUESTION_1_SNAP_SHOT, 
                          ROLE_2_SNAP_SHOT, NAME_2_SNAP_SHOT, QUESTION_2_SNAP_SHOT, 
                          ALLOW_OUTPUT, CONSIDER_CONTEXT, 
                          REMEMBER_CONTEXT, false, tool_reply);
        }
        else
        {
          // Resubmit question without tool reply
          //  Likely called by a tool request that doesnt exist or exist elsewhere.
          submit_request(ROLE_1_SNAP_SHOT, NAME_1_SNAP_SHOT, QUESTION_1_SNAP_SHOT, 
                          ROLE_2_SNAP_SHOT, NAME_2_SNAP_SHOT, QUESTION_2_SNAP_SHOT, 
                          ALLOW_OUTPUT, CONSIDER_CONTEXT, 
                          REMEMBER_CONTEXT, false);
        }
      }
    }
  }
}

string OLLAMA_API_PYTHON::get_complete_text_response()
{
  return RESPONSE_FULL;
}

void OLLAMA_API_PYTHON::context_pause()
{
  CONTEXT_PAUSED.push_back(CONVERSATION);
}

void OLLAMA_API_PYTHON::context_unpause()
{
  if (CONTEXT_PAUSED.size() > 0)
  {
    CONVERSATION = CONTEXT_PAUSED.back();
    CONTEXT_PAUSED.pop_back();
  }
}

void OLLAMA_API_PYTHON::process(TTY_OUTPUT &Output, TTY_OUTPUT_FOCUS &Focus)
{
  // Print Responses that may arrive.
  if (check_response() > 0)
  {
    string tmp_response = return_vector_as_string(RESPONSE_STRING_VECTOR);
    
    if (ALLOW_OUTPUT)
    {
      Output.add_to(tmp_response, Focus);
    }

    RESPONSE_FULL += tmp_response;
  }

  // Get to next line if response found and ask another question
  if (get_status() == OLLAMA_API_RESPONSE_DONE)
  {
    if (ALLOW_OUTPUT)
    {
      // Closing
      Output.seperater(Focus);
    }
    set_status(OLLAMA_API_READY_TO_GATHER);
  }

  check_response_done();
}

#endif  // OLLAMA_API_CPP