
cmake_minimum_required(VERSION 3.15)
project(pregunta4_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Descargar GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
FetchContent_MakeAvailable(googletest)

# Biblioteca principal
add_library(transformations src/transformations.cpp)
target_include_directories(transformations PUBLIC include)

# Ejecutable de tests (puedes añadir más fuentes si lo deseas)

# Ejecutable de tests con GoogleTest
add_executable(tests_runner
    tests/test_transformations.cpp
    tests/test_parsing.cpp
    tests/test_flujo_y_validacion.cpp
    tests/test_output.cpp
)
target_link_libraries(tests_runner PRIVATE transformations gtest gtest_main)

# Ejecutable principal
add_executable(main_app main.cpp)
target_link_libraries(main_app PRIVATE transformations)

# Registrar tests
include(GoogleTest)
enable_testing()
gtest_discover_tests(tests_runner)
