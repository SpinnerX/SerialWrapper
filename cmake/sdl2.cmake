find_package(SDL2 REQUIRED)

if(__APPLE__)
    # This is making sure we find the SDL libraries on mac
    include_directories(${SDL2_INCLUDE_DIRS})

endif(__APPLE__)

target_link_libraries(
    ${PROJECT_NAME}
    ${SDL2_LIBRARIES}
)