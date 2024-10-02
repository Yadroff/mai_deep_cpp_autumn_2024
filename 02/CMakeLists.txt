cmake_minimum_required(VERSION 3.10)
project(allocator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message("Build for Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -fsanitize=address -g -pthread")
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message("Build for Windows")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /WX /fsanitize=address /Zi")
    message("Downloading GTest")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
        TLS_VERIFY false
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    message("GTest downloaded!")
endif()

add_library(${CMAKE_PROJECT_NAME}_lib src/allocator.cpp)
add_executable(tests test/test.cpp)

set(INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(${CMAKE_PROJECT_NAME}_lib PRIVATE ${INCLUDES})
target_include_directories(tests PRIVATE ${INCLUDES})

target_link_libraries(tests PRIVATE ${CMAKE_PROJECT_NAME}_lib gtest_main gtest)

add_test(NAME AllocatorTests COMMAND tests)
