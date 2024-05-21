cmake_minimum_required(VERSION 3.28)
project(Starlight)

set(CMAKE_CXX_STANDARD 20)
add_compile_definitions(Starlight
						PRIVATE "BUILD_DEBUG=$<CONFIG:Debug>")

option(ENABLE_SWAP_CHAIN "ENABLES SWAP CHAIN EXTENSION" ON)

add_compile_definitions(Starlight
	PRIVATE "ENABLE_SWAP_CHAIN=$<CONFIG:ENABLE_SWAP_CHAIN>")

#Create executable
add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC ${CMAKE_SOURCE_DIR})

#Find external packages and build subdirectories for them.
find_package(Vulkan REQUIRED)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glm)

set(CORE_SOURCES
	${CMAKE_SOURCE_DIR}/core/engine.cpp
	${CMAKE_SOURCE_DIR}/core/engine.hpp
	${CMAKE_SOURCE_DIR}/core/logger.hpp
)

add_library(core ${CORE_SOURCES})

target_include_directories(
	core
	PUBLIC ${CMAKE_SOURCE_DIR})

set(RENDERING_SOURCES
	${CMAKE_SOURCE_DIR}/rendering/renderer.cpp
	${CMAKE_SOURCE_DIR}/rendering/renderer.hpp
	${CMAKE_SOURCE_DIR}/rendering/window.cpp
	${CMAKE_SOURCE_DIR}/rendering/window.hpp
	${CMAKE_SOURCE_DIR}/rendering/context.hpp
	${CMAKE_SOURCE_DIR}/rendering/context.cpp
	${CMAKE_SOURCE_DIR}/rendering/device.cpp
	${CMAKE_SOURCE_DIR}/rendering/device.hpp
	${CMAKE_SOURCE_DIR}/rendering/surface.cpp
	${CMAKE_SOURCE_DIR}/rendering/surface.hpp
	${CMAKE_SOURCE_DIR}/rendering/swap_chain.cpp
	${CMAKE_SOURCE_DIR}/rendering/swap_chain.hpp
)

add_library(rendering ${RENDERING_SOURCES})

target_include_directories(
	rendering
	PUBLIC ${CMAKE_SOURCE_DIR}
	PUBLIC ${VULKAN_SDK}/Include
)

target_link_directories(
	${PROJECT_NAME}
	PRIVATE ${Vulkan_LIBRARIES}
	PRIVATE ${CMAKE_SOURCE_DIR}/external/glfw/src
	PRIVATE ${CMAKE_SOURCE_DIR}/external/glm/glm
)

target_link_libraries(
	core
	PRIVATE Vulkan::Vulkan
	PRIVATE glfw
	PRIVATE glm::glm
)

target_link_libraries(
	rendering
	PRIVATE Vulkan::Vulkan
	PRIVATE glfw
	PRIVATE glm::glm
	PRIVATE core
)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE Vulkan::Vulkan
	PRIVATE glfw
	PRIVATE glm::glm
	PRIVATE core
	PRIVATE rendering
)