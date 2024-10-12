#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <vector>
#include <string>
//#include <future>
#include <thread>
#include <atomic>

/* 
    nlohmnann JSON is a feature-rich header-only C++ JSON implementation.
    This library was created by Niels Lohmann and is available under the MIT License.
    For more details visit: https://github.com/nlohmann/json
*/
#include "json.hpp"

#include "helper.h"
#include "stringthings.h"
#include "system.h"

void on_receive_response(const ollama::response& response);

void send_to_ollama(string Text);

#endif //MAIN_H