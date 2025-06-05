# Set version requirement
cmake_minimum_required(VERSION 3.20..4.0)

# Define project
project(database-abstraction-example
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

add_subdirectory("deps/hiberlite")

# Collect all source files
set (SRC_FILES "src/main.cxx")

# Create executable
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/hiberlite/include
)
target_link_libraries(${PROJECT_NAME} PUBLIC hiberlite)
target_compile_options(${PROJECT_NAME} PUBLIC "-Wno-terminate")

# Execute as test
include(CTest)
add_test(NAME Basic COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})
