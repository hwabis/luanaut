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

add_custom_target(copy_app_assets ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${APP_ASSETS_SRC_DIR}/models"
    "${APP_ASSETS_BIN_DIR}/models"
  COMMENT "Copying runtime models to binary directory..."
)
add_dependencies(luanaut copy_app_assets)
