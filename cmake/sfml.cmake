# Finding SFML 2
find_package(SFML 2 REQUIRED COMPONENTS network audio graphics window system)

target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARIES}
    sfml-system sfml-window sfml-graphics
)