cmake_minimum_required(VERSION 3.18.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Assign build type
set(BUILD_RELEASE ON)
set(BUILD_STRICT OFF)

if(BUILD_RELEASE)
    set(CMAKE_BUILD_TYPE Release)
else()
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Project name and description
project(chatapi)

# Find JSON
#find_package(nlohmann_json REQUIRED)

# Find the OpenSSL package
find_package(OpenSSL REQUIRED)

# Find ChromaDB
if (WIN32)
    set(CMAKE_PREFIX_PATH "C:/Program Files (x86)/ChromaDB")
    find_library(ChromaDB_LIB NAMES chromadb HINTS ${CMAKE_PREFIX_PATH}/lib)
    find_path(ChromaDB_INCLUDE_DIR NAMES ChromaDB/ChromaDB.h HINTS ${CMAKE_PREFIX_PATH}/include)
else()
    find_library(ChromaDB_LIB NAMES chromadb HINTS /usr/local/lib /usr/lib /lib)
    find_path(ChromaDB_INCLUDE_DIR NAMES ChromaDB/ChromaDB.h HINTS /usr/local/include /include)
endif()

# Check ChromaDB_CPP Errors
if(NOT ChromaDB_LIB)
    message(FATAL_ERROR "ChromaDB library not found")
endif()
if(NOT ChromaDB_INCLUDE_DIR)
    message(FATAL_ERROR "ChromaDB include directory not found")
endif()

# Inclues

#include_directories(${nlohmann_json_INCLUDE_DIRS})
include_directories(
    ${ChromaDB_INCLUDE_DIR} 
    ../../ollama-hpp/include
    )

#link_directories(${ChromaDB_LIB})

# Add the executable
add_executable(chatapi
    #../../ollama-hpp/include/ollama.hpp
    helper.cpp
    stringthings.cpp
    tty_io.cpp
    fled_time.cpp
    threading.cpp
    ollama_helper.cpp
    ollama_api.cpp
    chromadb_api.cpp
    system.cpp
    main.cpp
)

# Include directories
#target_include_directories(chatapi PRIVATE
#    ../../ollama-hpp/include
#    ${ChromaDB_INCLUDE_DIR}
#    ${OpenSSL_INCLUDE_DIR}
#)

target_link_libraries(chatapi 
    #../../ollama-hpp/include
    ${ChromaDB_LIB} 
    OpenSSL::SSL 
    OpenSSL::Crypto
    atomic
    pthread
)

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
