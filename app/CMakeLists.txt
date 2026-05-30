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

set(APP_ASSETS_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(APP_ASSETS_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets")
target_compile_definitions(luanaut PRIVATE
    APP_ASSETS_BIN_DIR="${APP_ASSETS_BIN_DIR}"
)

file(COPY "${APP_ASSETS_SRC_DIR}/models" DESTINATION "${APP_ASSETS_BIN_DIR}")
