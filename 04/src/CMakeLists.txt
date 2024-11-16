add_library(${CMAKE_PROJECT_NAME}_Fourier Fourier.cpp)
add_library(${CMAKE_PROJECT_NAME}_Unique_ptr unique.cpp)
add_library(${CMAKE_PROJECT_NAME}_Vector vector.cpp)
add_library(${CMAKE_PROJECT_NAME}_Big_int
        BigInt.cpp Constructors.cpp Predicates.cpp ArithmeticOperations.cpp)
add_executable(${CMAKE_PROJECT_NAME} main.cpp)

target_include_directories(${CMAKE_PROJECT_NAME}_Fourier PRIVATE ${INCLUDES})
target_include_directories(${CMAKE_PROJECT_NAME}_Unique_ptr PRIVATE ${INCLUDES})
target_include_directories(${CMAKE_PROJECT_NAME}_Vector PRIVATE ${INCLUDES})
target_include_directories(${CMAKE_PROJECT_NAME}_Big_int PRIVATE ${INCLUDES})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${INCLUDES})

target_link_libraries(${CMAKE_PROJECT_NAME}_Big_int PUBLIC ${CMAKE_PROJECT_NAME}_Fourier ${CMAKE_PROJECT_NAME}_Unique_ptr ${CMAKE_PROJECT_NAME}_Vector)
target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_Big_int)
message("Includes from src: ${INCLUDES}")
