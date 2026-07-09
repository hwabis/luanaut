## Luanaut

This is a general purpose game engine, but written with 3D motion graphics and sequential keyframing in mind. Think Blender, but code-only (or like [Manim](https://github.com/manimCommunity/manim) or [storybrew](https://github.com/Damnae/storybrew)).

Check out the [examples folder](https://github.com/hwabis/luanaut/tree/master/examples).

This is not related to the Lua programming language 🙂

### Building

Install [Vulkan SDK](https://vulkan.lunarg.com/) (for the dxc shader compiler). Then run:

```
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake" -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```
