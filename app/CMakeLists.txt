include(${CMAKE_SOURCE_DIR}/cmake/set_target_defaults.cmake)

add_executable(luanaut
  src/Main.cc
)

set_target_defaults(luanaut)

target_include_directories(luanaut PRIVATE src)

target_link_libraries(luanaut
  PRIVATE
    lneng
)

# todo the asset stuff should be here as well for user-defined models etc
