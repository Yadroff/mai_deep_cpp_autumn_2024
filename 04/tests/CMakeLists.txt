# Пробуем найти GTest
find_package(GTest)

# Проверяем, найден ли GTest
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

enable_testing()
add_executable(${CMAKE_PROJECT_NAME}_Tests tests_main.cpp constructor_tests.cpp predicates_tests.cpp arithmetic_tests.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME}_Tests gtest gtest_main)
target_link_libraries(${CMAKE_PROJECT_NAME}_Tests ${CMAKE_PROJECT_NAME}_Big_int)
target_include_directories(${CMAKE_PROJECT_NAME}_Tests PRIVATE ${INCLUDES})

add_test(NAME ${CMAKE_PROJECT_NAME}_Tests COMMAND ${CMAKE_PRJECT_NAME}_Tests)
