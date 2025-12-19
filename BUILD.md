# Building BlackboxLoader

## Prerequisites

- CMake 3.15 or higher
- MinGW-w64 (for cross-compilation on Linux/macOS) or MSVC (on Windows)

## Build Instructions

### Windows (MSVC)

#### Build as version.dll (default)
```cmd
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

#### Build as winhttp.dll
```cmd
cmake -B build -G "Visual Studio 17 2022" -A x64 -DPROXY_DLL=WINHTTP
cmake --build build --config Release
```

### Linux/macOS (MinGW Cross-Compilation)

#### Build as version.dll (default)
```bash
cmake -B build -DCMAKE_TOOLCHAIN_FILE=toolchain-mingw.cmake
cmake --build build
```

#### Build as winhttp.dll
```bash
cmake -B build -DCMAKE_TOOLCHAIN_FILE=toolchain-mingw.cmake -DPROXY_DLL=WINHTTP
cmake --build build
```

## Output

The compiled DLL will be in the `build/` directory:
- `version.dll` (when built with default settings)
- `winhttp.dll` (when built with `-DPROXY_DLL=WINHTTP`)

## Switching Between Proxy Types

To switch from one proxy type to another:

1. Clean the build directory: `rm -rf build/` (Linux/macOS) or `rmdir /s build` (Windows)
2. Run CMake with the desired option
3. Build

Alternatively, you can use different build directories for each configuration:

```bash
# Version.dll build
cmake -B build-version
cmake --build build-version

# Winhttp.dll build
cmake -B build-winhttp -DPROXY_DLL=WINHTTP
cmake --build build-winhttp
```
