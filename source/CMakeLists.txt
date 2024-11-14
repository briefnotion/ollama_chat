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
find_package(nlohmann_json REQUIRED)

# Find the OpenSSL package
find_package(OpenSSL REQUIRED)

# Find the ChromaDB package
find_library(ChromaDB_LIB NAMES chromadb HINTS  /usr/local/lib /usr/lib /lib)
find_path(ChromaDB_INCLUDE_DIR NAMES ChromaDB/ChromaDB.h HINTS ../../chromadb-cpp/include)

# Find the OllamaHPP package
find_path(OllamaHPP_INCLUDE_DIR NAMES ollama.hpp HINTS ../../ollama-hpp/singleheader)

include_directories(
    ${ChromaDB_INCLUDE_DIR} 
    ${OllamaHPP_INCLUDE_DIR} 
    )

#link_directories(${ChromaDB_LIB})

# Add the executable
add_executable(chatapi
    helper.cpp
    stringthings.cpp
    tty_io.cpp
    fled_time.cpp
    threading.cpp
    ollama_helper.cpp
    ollama_api.cpp
    chromadb_api.cpp
    #vectordb_api.cpp
    system.cpp
    main.cpp
)

target_include_directories(chatapi PRIVATE
    ${ChromaDB_INCLUDE_DIR}
    ${OllamaHPP_INCLUDE_DIR}

)

target_link_libraries(chatapi 
    ${ChromaDB_LIB} 
    #${OllamaHPP_LIB} 
    OpenSSL::SSL 
    OpenSSL::Crypto
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
