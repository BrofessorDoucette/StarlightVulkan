cmake_minimum_required(VERSION 3.27)
project(VulkanFromScratch)

set(CMAKE_CXX_STANDARD 17)

add_executable(VulkanFromScratch
        src/main.cpp
        src/engine.cpp
        include/engine.hpp
        include/config.hpp
        include/config.hpp
        include/instance.hpp)

find_package(Vulkan REQUIRED)

add_subdirectory(external/glfw)
add_subdirectory(external/glm)

target_include_directories(${PROJECT_NAME}
        PUBLIC ${VULKAN_SDK}/Include
        PUBLIC external/glfw/include
        PUBLIC external/glm/glm
        PUBLIC include)

target_link_directories(${PROJECT_NAME}
        PRIVATE ${Vulkan_LIBRARIES}
        PRIVATE external/glfw/src
        PRIVATE external/glm/glm)

target_link_libraries(${PROJECT_NAME}
        PRIVATE Vulkan::Vulkan
        PRIVATE glfw
        PRIVATE glm::glm)