cmake_minimum_required(VERSION 3.28)
project(Starlight)

set(CMAKE_CXX_STANDARD 20)

#Create executable
add_executable(${PROJECT_NAME} main.cpp)

#Find external packages and build subdirectories for them.
find_package(Vulkan REQUIRED)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glm)

set(CORE_SOURCES
	${CMAKE_SOURCE_DIR}/core/engine.cpp
	${CMAKE_SOURCE_DIR}/core/engine.hpp
	${CMAKE_SOURCE_DIR}/core/external.hpp
)

add_library(core ${CORE_SOURCES})

set(RENDERING_SOURCES
	${CMAKE_SOURCE_DIR}/rendering/instance.cpp
	${CMAKE_SOURCE_DIR}/rendering/instance.hpp
	${CMAKE_SOURCE_DIR}/rendering/logging.hpp
	${CMAKE_SOURCE_DIR}/rendering/logging.cpp
	${CMAKE_SOURCE_DIR}/rendering/device.hpp
	${CMAKE_SOURCE_DIR}/rendering/device.cpp
)

add_library(rendering ${RENDERING_SOURCES})

target_include_directories(
	${PROJECT_NAME}
	PUBLIC ${VULKAN_SDK}/Include
	PUBLIC ${CMAKE_SOURCE_DIR}/external/glfw/include
	PUBLIC ${CMAKE_SOURCE_DIR}/external/glm/glm
	PUBLIC ${CMAKE_SOURCE_DIR}/rendering
	PUBLIC ${CMAKE_SOURCE_DIR}/core
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
)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE Vulkan::Vulkan
	PRIVATE glfw
	PRIVATE glm::glm
	PRIVATE core
	PRIVATE rendering
)