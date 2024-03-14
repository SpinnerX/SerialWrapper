# This is the link to learn Vulkan API: https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Shader_modules
# In Mac: To install vulkan using Homebrew do: brew install vulkan-tools
if(APPLE)
    find_package(Vulkan REQUIRED FATAL_ERROR)

    if (VULKAN_FOUND)
        message(STATUS "Found Vulkan. Including and linking...")
        target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
    endif(VULKAN_FOUND)
endif(APPLE)