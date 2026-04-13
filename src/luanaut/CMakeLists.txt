include(${CMAKE_SOURCE_DIR}/cmake/set_target_defaults.cmake)

find_package(SDL3 CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(VulkanHeaders CONFIG REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_executable(luanaut
  Main.cc
  VulkanStuff.cc
)

set_target_defaults(luanaut)

target_include_directories(luanaut PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(luanaut
  PRIVATE
    SDL3::SDL3
    Vulkan::Vulkan
    Vulkan::Headers
    GPUOpen::VulkanMemoryAllocator
    spdlog::spdlog
)

find_program(SLANGC_EXECUTABLE slangc REQUIRED)
set(SHADER_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader.slang)
set(SHADER_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders/shader.spv)
add_custom_command(
    OUTPUT ${SHADER_OUTPUT}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/shaders
    COMMAND ${SLANGC_EXECUTABLE} ${SHADER_SOURCE} 
            -target spirv -profile spirv_1_4 
            -emit-spirv-directly 
            -fvk-use-entrypoint-name 
            -entry vertMain -entry fragMain 
            -o ${SHADER_OUTPUT}
    DEPENDS ${SHADER_SOURCE}
    COMMENT "Compiling Slang shader: ${SHADER_SOURCE}"
    VERBATIM
)
add_custom_target(compiled_shaders DEPENDS ${SHADER_OUTPUT})
add_dependencies(luanaut compiled_shaders)
target_compile_definitions(luanaut PRIVATE
    SHADER_PATH="${CMAKE_CURRENT_BINARY_DIR}/shaders/shader.spv"
)
