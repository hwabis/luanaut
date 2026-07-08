include(${CMAKE_SOURCE_DIR}/cmake/add_test_executable.cmake)
enable_testing()
find_package(GTest CONFIG REQUIRED)

add_subdirectory(lneng)
