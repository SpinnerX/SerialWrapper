cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME serialWrapper)

project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES C CXX)

include_directories(/opt/homebrew/opt/curl)
set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)

add_executable(${PROJECT_NAME} main.cpp gps.h)

include(cmake/boost.cmake)
include(cmake/curl.cmake)