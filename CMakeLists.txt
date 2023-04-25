cmake_minimum_required(VERSION 3.10)

project(licenta)

set(CMAKE_CXX_STANDARD 17)

# This way I can set the output folder of the executable 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)

# Set the paths to a couple of variables for better readability
set(PATH_TO_CPP ${CMAKE_SOURCE_DIR}/res/cpp/src)
set(PATH_TO_HEADERS ${CMAKE_SOURCE_DIR}/res/cpp/headers)

# Set the sources to a specific variable
set(SOURCE_FILES_LIBRARY
    ${PATH_TO_CPP}/constants.cpp
    ${PATH_TO_CPP}/logger.cpp
    ${PATH_TO_CPP}/server.cpp
    ${PATH_TO_CPP}/main.cpp
    ${PATH_TO_CPP}/chatbot.cpp
)

# Include the headers' folder in the CMake build
include_directories(${PATH_TO_HEADERS})

# Creates an executable that has as translation units the sources
# defined in the SOURCE_FILES_LIBRARY variable
add_executable(${PROJECT_NAME}_EXECUTABLE
    ${SOURCE_FILES_LIBRARY}
)

# Find the required packages
find_package(Poco REQUIRED Foundation Net Util)
find_package(google_cloud_cpp_dialogflow_es REQUIRED)

# Linking the required libraries for my executable from the POCO library and google-cloud-cpp
target_link_libraries(${PROJECT_NAME}_EXECUTABLE Poco::Foundation Poco::Net Poco::Util google-cloud-cpp::dialogflow_es)