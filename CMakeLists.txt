# Set version requirement
cmake_minimum_required(VERSION 3.20..4.0)

# Define project
project(dal
    VERSION 0.0.1
    DESCRIPTION "Simplistic database abstraction example."
    LANGUAGES C CXX)

# Set CMake options
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Set standard
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

# Set default to Debug
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

# Add external dependencies
add_subdirectory("deps/hiberlite")
add_subdirectory("deps/json")

# Set up library
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# Set up hiberlite driver
set(PROJECT_NAME_HIBERLITE "${PROJECT_NAME}-hiberlite")
add_library(${PROJECT_NAME_HIBERLITE} INTERFACE)
target_include_directories(${PROJECT_NAME_HIBERLITE} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/hiberlite
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/hiberlite/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)
target_link_libraries(${PROJECT_NAME_HIBERLITE} INTERFACE ${PROJECT_NAME})

# Set up json driver
set(PROJECT_NAME_JSON "${PROJECT_NAME}-json")
add_library(${PROJECT_NAME_JSON} INTERFACE)
target_include_directories(${PROJECT_NAME_JSON} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/json
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/json/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)
target_link_libraries(${PROJECT_NAME_JSON} INTERFACE ${PROJECT_NAME})

# Collect all source files
set (SRC_FILES "test/hiberlite.cxx")
set (PROJECT_NAME_TEST "${PROJECT_NAME}-test")

# Create executable
add_executable(${PROJECT_NAME_TEST} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME_TEST} PRIVATE ${PROJECT_NAME_HIBERLITE} hiberlite)
target_compile_options(${PROJECT_NAME_TEST} PUBLIC "-Wno-terminate")

# Execute as test
include(CTest)
add_test(NAME hiberlite COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_TEST})

# Collect all source files
set (SRC_FILES "test/json.cxx")
set (PROJECT_NAME_JSON_TEST "${PROJECT_NAME}-json-test")

# Create executable
add_executable(${PROJECT_NAME_JSON_TEST} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME_JSON_TEST} PRIVATE ${PROJECT_NAME_JSON} nlohmann_json)
target_compile_options(${PROJECT_NAME_JSON_TEST} PUBLIC "-Wno-terminate")

# Execute as test
include(CTest)
add_test(NAME json COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_JSON_TEST})
