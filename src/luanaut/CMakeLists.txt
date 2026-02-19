include(${CMAKE_SOURCE_DIR}/cmake/set_target_defaults.cmake)

add_executable(luanaut
  Main.cc
)

set_target_defaults(luanaut)

target_include_directories(luanaut PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# target_link_libraries todo
