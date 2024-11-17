cmake_minimum_required(VERSION 3.24)
project(BigInt)

set(CMAKE_CXX_STANDARD 20)

if (UNIX)
    message("Build for Unix")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fsanitize=address -fsanitize=undefined -g")
endif ()

if (WIN32)
    message("Build for Windows")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /fsanitize=address /Zi")

endif ()

set(INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_BUILD_TYPE Release)

target_compile_definitions(${CMAKE_PROJECT_NAME}
        PRIVATE
        # If the debug configuration pass the DEBUG define to the compiler
        $<$<CONFIG:Debug>:DEBUG>
        )

# Дополнительные флаги компилятора для Release
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    # Например, можно указать оптимизацию
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif ()

add_subdirectory(src)
add_subdirectory(tests)
