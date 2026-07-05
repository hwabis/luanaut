include(${CMAKE_SOURCE_DIR}/cmake/set_target_defaults.cmake)

add_executable(earendel
  src/Main.cc
)

set_target_defaults(earendel)

target_include_directories(earendel PRIVATE src)

target_link_libraries(earendel
  PRIVATE
    lneng
)

set(APP_ASSETS_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(APP_ASSETS_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets")
target_compile_definitions(earendel PRIVATE
  APP_ASSETS_BIN_DIR="${APP_ASSETS_BIN_DIR}"
)

add_custom_target(copy_app_assets ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${APP_ASSETS_SRC_DIR}"
    "${APP_ASSETS_BIN_DIR}"
  COMMENT "Copying runtime assets to binary directory..."
)
add_dependencies(earendel copy_app_assets)
