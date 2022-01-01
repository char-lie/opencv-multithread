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

## How to check code style

It is better to use an alias for the static code analysis tool
to launch it more conveniently.

First, install `scan-build`
([Clang Static Analyzer](https://clang-analyzer.llvm.org)),
[Clang-Tidy](https://clang.llvm.org/extra/clang-tidy),
and [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html)
for static code analysis.
You can install them via `apt` (if you use it)

```bash
apt install clang-tools clang-tidy clang-format
```

Build the application with the analysis tools.
First, it is convenient to create an alias

```bash
alias clang-static-analyzer="scan-build \
    --use-cc=clang \
    --use-c++=clang++ \
    --force-analyze-debug-code \
    -enable-checker alpha.clone \
    -enable-checker alpha.core \
    -enable-checker alpha.cplusplus \
    -enable-checker alpha.deadcode \
    -enable-checker alpha.llvm \
    -enable-checker alpha.security \
    -enable-checker alpha.security.cert \
    -enable-checker alpha.security.cert.pos \
    -enable-checker alpha.unix \
    -enable-checker core \
    -enable-checker cplusplus \
    -enable-checker debug \
    -enable-checker optin \
    -enable-checker security \
    -enable-checker unix \
    -v -v -v"
```

Then, configure the project

```bash
clang-static-analyzer cmake \
    -DCMAKE_CXX_CLANG_TIDY="clang-tidy;-header-filter=$(realpath ..)" \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_FLAGS="-pedantic -Wall -Wextra -Wshadow -Weffc++ -Wconversion -Werror" \
    -B build
```

Finally, analyze the code

```bash
clang-static-analyzer cmake --build build
```

You can use `clang-format` in conjunction with `diff`
to see proposed changes

```bash
for FILE in `find . -name "*.[hc]pp"`
do
    diff -u --color $FILE <(clang-format $FILE)
done
```

It can automatically reformat your code with `-i` flag

```bash
find ./{lib,include} -name "*.[hc]pp" -exec clang-format --verbose -i {} \+
```

You can use it to reformat your current diff before committing the changes

```bash
git diff -U0 --no-color HEAD^ | clang-format-diff -i -p1
```
