cmake_minimum_required(VERSION 3.10)
project(serialize)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if (UNIX)
    message("Build for Unix")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fsanitize=address -g -pthread")
endif ()

if (WIN32)
    message("Build for Windows")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /WX /fsanitize=address /Zi")
endif()

find_package(GTest)

if (NOT GTest_FOUND)
    message(STATUS "Downloading GTest")
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
    message(STATUS "GTest downloaded!")
endif ()

add_executable(tests tests/tests.cpp)

set(INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

target_include_directories(tests PRIVATE ${INCLUDES})

target_link_libraries(tests PRIVATE gtest_main gtest)

add_test(NAME ${CMAKE_PROJECT_NAME}_tests COMMAND tests)

set(CMAKE_BUILD_TYPE Release)

target_compile_definitions(tests
        PRIVATE
        # If the debug configuration pass the DEBUG define to the compiler
        $<$<CONFIG:Debug>:DEBUG>
        )
