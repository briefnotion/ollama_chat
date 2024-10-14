# Minimum required version of CMake
cmake_minimum_required(VERSION 3.18.0)

set(BUILD_RELEASE ON)
set(BUILD_STRICT OFF)

if(BUILD_RELEASE)
      set(CMAKE_BUILD_TYPE Release)
else()
      set(CMAKE_BUILD_TYPE Debug)
endif()

# Project name and description
project(chatapi)

# find_package(glfw3 REQUIRED)
# find_package(OpenGL REQUIRED)
# find_package(Boost COMPONENTS system thread filesystem REQUIRED)

# find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
# find_library(WS2811_LIBRARIES NAMES ws2811)
# find_library(PTHREAD_LIBRARIES NAMES pthread)
# find_library(GLFW_LIBRARIES NAMES glfw3)
# find_library(LIBRT rt)

add_executable(chatapi)

target_include_directories(chatapi PRIVATE 
  ../../ollama-hpp/include
)

target_sources(chatapi PRIVATE
  ../../ollama-hpp/include/ollama.hpp
  ../../ollama-hpp/include/json.hpp
  helper.cpp
  stringthings.cpp
  fled_time.cpp
  threading.cpp
  ollama_helper.cpp
  ollama_api.cpp
  system.cpp
  main.cpp
)

# target_compile_definitions(chatapi PRIVATE BOOST_BIND_GLOBAL_PLACEHOLDERS)

# target_link_libraries(chatapi 
#   ${WIRINGPI_LIBRARIES} 
#   ${WS2811_LIBRARIES}
#   glfw
#   ${OPENGL_LIBRARIES}
#   ${PTHREAD_LIBRARIES}
#   ${LIBRT}
#   ${Boost_LIBRARIES}
# )

# Specify the source files for the project
# set(CMAKE_CXX_STANDARD 14)
# set(SOURCES main.cpp)

# Add executable target to build the program
# add_executable(${PROJECT_NAME} ${SOURCES})

if(BUILD_STRICT)
  # Strict
  if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(chatapi PRIVATE -Wall -Wextra -Werror)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(chatapi PRIVATE -Weverything -Werror)
  endif()
else()
  # Not Strict
  if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(chatapi PRIVATE -Wall -Wextra)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(chatapi PRIVATE -Weverything)
  endif()
endif()
