#ifndef OLLAMA_API_CPP
#define OLLAMA_API_CPP

#include "ollama_api.h"

using namespace std;

// ------------------------------------------------------------------------- //

void OLLAMA_API::on_receive_response(const ollama::response& response) 
{
  if (response.as_json()["done"] == true) 
  {
    OLLAMA_MUTEX.add_to_response(response);
    OLLAMA_MUTEX.set_done(OLLAMA_API_RESPONSE_DONE);
    OLLAMA_MUTEX.set_complete_response_after_done(response);
  }
  else
  {
    OLLAMA_MUTEX.add_to_response(response);
  }
}

void OLLAMA_API::proc_render_thread() 
{
  OLLAMA_MUTEX.set_done(OLLAMA_API_RESPONS_GENERATING);

  // Set the callback to the member function
  std::function<void(const ollama::response&)> response_callback = [this](const ollama::response& res) 
  {
    this->on_receive_response(res);
  };

  //1 - ollama::response generate(ollama::request &request)

  //2 - bool generate(ollama::request &request, 
                  //  std::function<void (const ollama::response &)> on_receive_response)

  //3 - ollama::response generate(const std::string &model, 
                  //  const std::string &prompt, 
                  //  const ollama::json &options = nullptr, 
                  //  const std::vector<std::string> &images = std::vector<std::string>())

  //4 - bool generate(const std::string &model, 
                  //  const std::string &prompt, 
                  //  std::function<void (const ollama::response &)> on_receive_response, 
                  //  const ollama::json &options = nullptr, 
                  //  const std::vector<std::string> &images = std::vector<std::string>())

  //5 - ollama::response generate(const std::string &model, 
                  //  const std::string &prompt, 
                  //  const ollama::response &context, 
                  //  const ollama::json &options = nullptr, 
                  //  const std::vector<std::string> &images = std::vector<std::string>())

  //6 - bool generate(const std::string &model, 
                  //  const std::string &prompt, 
                  //  ollama::response &context, 
                  //  std::function<void (const ollama::response &)> on_receive_response, 
                  //  const ollama::json &options = nullptr, 
                  //  const std::vector<std::string> &images = std::vector<std::string>())

  //cout << "-------------" << endl;

  //cout << "Response JSON: " << CONTEXT.as_json().dump() << endl << endl;
  //cout << "Response STRING: " << CONTEXT.as_json_string() << endl  << endl;

  //OLLAMA_MUTEX.set_context(CONTEXT);

  //ollama::request request(ollama::message_type::generation);
  //request["model"]=     "llama3.1:8b";
  //request["prompt"]=    OLLAMA_MUTEX.request();
  //request["context"] =  OLLAMA_MUTEX.context();

  //std::cout << OLLAMA_MUTEX.context().as_simple_string() << std::endl;

  ollama::generate("llama3:8b", REQUEST, CONTEXT, response_callback, OPTIONS);
}

// ------------------------------------------------------------------------- //

bool OLLAMA_API::create(TTY_OUTPUT &Output_Container, TTY_OUTPUT_FOCUS &Output_Focus)
{
  bool ret_running = false;

  // Optional. By default, the server URL is set to http://localhost:11434. 
  // Use this function if your server resides at a different URL.
  OLLAMA.setServerURL("http://localhost:11434");
  //OLLAMA.setServerURL("http://127.0.0.1:11434");
  
  // Optional. Set the read and write timeouts in seconds for server interactions.
  // If you have a large model with a long response time you may need to increase these.
  OLLAMA.setReadTimeout(120);
  OLLAMA.setWriteTimeout(120);

  if (OLLAMA.is_running())
  {
    string thing = "";

    Output_Container.add_to("Version: " + OLLAMA.get_version() + "\n", Output_Focus);
    //cout << "Version: " << OLLAMA.get_version() << endl;

    // This can optionally be used to deliberately load a model into memory prior to use. 
    // This occurs automatically when a request is made for an unloaded model, but can be 
    // useful for loading a model in advance.
    // bool model_loaded = OLLAMA.load_model("llama3:8b");

    // Pull a model by specifying a model name.
    // bool model_pulled = OLLAMA.pull_model("llama3:8b");

    // Copy a model by specifying a source model and destination model name.
    // bool model_copied = OLLAMA.copy_model("llama3:8b", "llama3_copy");

    // Delete a model by specifying a model name.
    // bool model_deleted = OLLAMA.delete_model("llama3_copy");

    // Request model info from the OLLAMA server.
    nlohmann::json model_info = OLLAMA.show_model_info("llama3:8b");
    //cout << "Model family is " << model_info["details"]["family"] << endl;
    thing = model_info["details"]["family"];
    Output_Container.add_to( "Model family is " + thing + "\n", Output_Focus);

    // List the models available locally in the OLLAMA server
    OLLAMA_MODELS_LIST = OLLAMA.list_models();
    //print_vector_with_title("Available models", OLLAMA_MODELS_LIST);
    thing = return_vector_as_string(OLLAMA_MODELS_LIST, true);
    Output_Container.add_to( "Available models \n" + thing + "\n", Output_Focus);

    // List the models available locally in the OLLAMA server
    OLLAMA_MODELS_RUNNING = OLLAMA.list_running_models();
    //print_vector_with_title("Running models", OLLAMA_MODELS_RUNNING);
    thing = return_vector_as_string(OLLAMA_MODELS_RUNNING, true);
    Output_Container.add_to( "Running models \n" + thing + "\n", Output_Focus);

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


    // -------------------------------------------------
    
    Output_Container.add_to( " -----\n\n", Output_Focus);

    ret_running = true;
  }

  return ret_running;
}

int OLLAMA_API::get_status()
{
  return OLLAMA_MUTEX.done();
}

void OLLAMA_API::set_status(int Status)
{
  OLLAMA_MUTEX.set_done(Status);
}

void OLLAMA_API::set_request(const string& Request)
{
  REQUEST = Request;
  OLLAMA_MUTEX.set_done(OLLAMA_API_REQUEST_SUBMITTED);
}

int OLLAMA_API::check_response()
{
  OLLAMA_MUTEX.get_response_to_vector(RESPONSE_STRING_VECTOR);
  return RESPONSE_STRING_VECTOR.size();
}

void OLLAMA_API::check_response_done()
{
  if (OLLAMA_MUTEX.complete_respoonse_ready_after_done())
  {
    RESPONSE = OLLAMA_MUTEX.get_complete_response_after_done();
    CONTEXT = RESPONSE;
  }
}

// ------------------------------------------------------------------------- //

#endif  // OLLAMA_API_CPP