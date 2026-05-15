include(${CMAKE_SOURCE_DIR}/cmake/set_target_defaults.cmake)

find_package(glm CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_executable(luanaut
  # todo separate app/engine to different targets
  src/Game.cc
  src/Main.cc
  src/Node.cc
)

set_target_defaults(luanaut)

target_include_directories(luanaut PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(luanaut
  PRIVATE
    glm::glm
    SDL3::SDL3
    spdlog::spdlog
)

find_program(DXC_EXECUTABLE dxc REQUIRED)
set(SHADER_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders")
set(SHADER_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets/shaders")
file(GLOB_RECURSE SHADER_SOURCES "${SHADER_SRC_DIR}/*.hlsl")
set(ALL_COMPILED_BINARIES "")

foreach(SRC_PATH ${SHADER_SOURCES})
    get_filename_component(FILE_NAME ${SRC_PATH} NAME)
    string(REGEX REPLACE "\\.hlsl$" ".spv" BIN_NAME ${FILE_NAME})
    set(BIN_PATH "${SHADER_BIN_DIR}/${BIN_NAME}")

    if(FILE_NAME MATCHES "\\.vert\\.hlsl$")
        set(DXC_PROFILE "vs_6_0")
        set(DXC_ENTRY "vertMain")
    elseif(FILE_NAME MATCHES "\\.frag\\.hlsl$")
        set(DXC_PROFILE "ps_6_0")
        set(DXC_ENTRY "fragMain")
    else()
        message(WARNING "Unknown shader stage type for file: ${FILE_NAME}. Skipping.")
        continue()
    endif()

    add_custom_command(
        OUTPUT ${BIN_PATH}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_BIN_DIR}
        COMMAND ${DXC_EXECUTABLE}
                -spirv
                -T ${DXC_PROFILE}
                -E ${DXC_ENTRY}
                -Fo ${BIN_PATH}
                ${SRC_PATH}
        DEPENDS ${SRC_PATH}
        COMMENT "DXC compiling: ${FILE_NAME} -> ${BIN_NAME}"
        VERBATIM
    )

    list(APPEND ALL_COMPILED_BINARIES ${BIN_PATH})
endforeach()

add_custom_target(compiled_shaders DEPENDS ${ALL_COMPILED_BINARIES})
add_dependencies(luanaut compiled_shaders)
target_compile_definitions(luanaut PRIVATE
    SHADER_BIN_DIR="${SHADER_BIN_DIR}"
)

# todo same thing for models and textures lol
