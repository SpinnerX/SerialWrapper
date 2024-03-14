# Cmake for linking and including the libcurl library
include_directories(/opt/homebrew/opt/curl)
set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)

include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES})