# OpenCV multithread

Messing with multithreading with OpenCV

## How to build

You need [OpenCV](https://opencv.org/) 4.2 or higher,
[CMake](https://cmake.org/) of version 3.12 or higher
and a camera.

You can start with
```bash
cmake -S . -B build && cmake --build build
```
and then find your executable file in the `build` directory
(will be created automatically).

Also, you can create an optimized code
with compile commands exported (for static analysis tools)
```bash
cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_FLAGS="-march=native -mtune=native" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -S . -B build
```

and build it as usually
```bash
cmake --build build
```
