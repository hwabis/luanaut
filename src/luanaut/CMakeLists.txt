include(${CMAKE_SOURCE_DIR}/cmake/set_target_defaults.cmake)

find_package(SDL3 CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(VulkanHeaders CONFIG REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_executable(luanaut
  Main.cc
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
