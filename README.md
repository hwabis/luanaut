### Building

Install [Vulkan SDK](https://vulkan.lunarg.com/) (for the dxc shader compiler). Then run:

```
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake" -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build
```
