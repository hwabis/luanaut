cmake_minimum_required(VERSION 4.0)

project(
  luanaut
  LANGUAGES CXX
)

enable_testing()
find_package(GTest CONFIG REQUIRED)

find_package(spdlog CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)
