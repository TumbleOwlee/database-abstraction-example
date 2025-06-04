# Set version requirement
cmake_minimum_required(VERSION 3.16)

# Define project
project(database-abstraction-example
    VERSION 0.0.1
    DESCRIPTION "Simplistic database abstraction example."
    LANGUAGES C CXX)

# Export compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

# Set default to Debug
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()
message("Build type: ${CMAKE_BUILD_TYPE}")

add_subdirectory("deps/hiberlite")

# Create list of all test files
set (SRC_FILES "src/main.cxx")

# Create target for each test
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/hiberlite/include
)
target_link_libraries(${PROJECT_NAME} PUBLIC hiberlite)
