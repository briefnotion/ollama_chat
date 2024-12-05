cmake_minimum_required(VERSION 3.18.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Assign build type
set(BUILD_RELEASE ON)
set(BUILD_STRICT ON)

if(BUILD_RELEASE)
    set(CMAKE_BUILD_TYPE Release)
else()
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Project name and description
project(chatapi)

# Find JSON
find_package(nlohmann_json REQUIRED)

# Find the OpenSSL package
find_package(OpenSSL REQUIRED)

# Find the OllamaHPP package
find_path(OllamaHPP_INCLUDE_DIR NAMES ollama.hpp HINTS ../../ollama-hpp/singleheader)

include_directories(
    ${OllamaHPP_INCLUDE_DIR}
    ${HTTPLIB_INCLUDE_DIR} 
)

# Create a target for files that include ollama.hpp with non-strict options
add_library(ollama_files STATIC
    ollama_api.cpp
    vectordb_python_api.cpp
    system.cpp
    main.cpp
    thoughts.cpp
)

target_include_directories(ollama_files PRIVATE ${OllamaHPP_INCLUDE_DIR})

# Always set compile options for ollama_files to strict off
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(ollama_files PRIVATE -Wall -Wextra)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(ollama_files PRIVATE -Weverything)
endif()

# Add the executable for remaining files
add_executable(chatapi
    helper.cpp
    stringthings.cpp
    tty_io.cpp
    fled_time.cpp
    threading.cpp
)

target_include_directories(chatapi PRIVATE
    ${OllamaHPP_INCLUDE_DIR}
)

# Set compile options for chatapi based on BUILD_STRICT variable
if(BUILD_STRICT)
    if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        target_compile_options(chatapi PRIVATE -Wall -Wextra -Werror)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(chatapi PRIVATE -Weverything -Werror)
    endif()
else()
    if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        target_compile_options(chatapi PRIVATE -Wall -Wextra)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(chatapi PRIVATE -Weverything)
    endif()
endif()

target_link_libraries(chatapi PRIVATE 
    OpenSSL::SSL 
    OpenSSL::Crypto
    ollama_files
)
