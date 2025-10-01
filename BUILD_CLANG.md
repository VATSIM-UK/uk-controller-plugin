# Building with Clang and Conan 2

This project now supports building with Clang compiler using Conan 2 for dependency management.

## Prerequisites

1. **Install Clang**: Download and install LLVM/Clang from [LLVM releases](https://releases.llvm.org/) or use a package manager
2. **Install Conan 2**: `pip install "conan>=2.0"`
3. **Install CMake 3.20+**: Download from [cmake.org](https://cmake.org/download/) or use a package manager
4. **Install Ninja** (recommended): `pip install ninja` or download from [ninja-build.org](https://ninja-build.org/)
5. **Install ccache** (optional, for faster rebuilds): Download from [ccache.dev](https://ccache.dev/)

## Quick Start

### Using Conan 2.0 with Clang (Recommended)

```powershell
# Create Conan profile (first time only)
conan profile detect --force
conan profile show default

# Install dependencies with MSVC for stability
conan install . --build=missing -s compiler=msvc -s compiler.version=194 -s compiler.cppstd=17 -s build_type=Release

# Configure with Clang for your project code
cmake --preset conan-default -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang

# Build
cmake --build --preset conan-release
```

### Alternative: Manual Configuration

```powershell
# Install dependencies with MSVC
conan install . --build=missing -s compiler=msvc -s compiler.version=194 -s compiler.cppstd=17 -s build_type=Release

# Manually configure with Clang
cmake -B build-clang-release -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=build/generators/conan_toolchain.cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang

# Build
cmake --build build-clang-release --config Release -j
```

## Configuration Options

### Conan Configuration

For Conan 2.0, dependencies are specified in `conanfile.py`. The main dependencies are:

- nlohmann_json 3.11.3 (JSON handling)
- spdlog 1.12.0 (logging)
- fmt 10.2.1 (string formatting)
- gtest 1.14.0 (testing framework)

### Build Types

- **Release**: Optimized build with `-O3`
- **Debug**: Debug build with `-g -O0`

### Features

- **C++17**: Minimum standard enforced
- **ccache**: Automatic detection and usage for faster rebuilds
- **Ninja**: Recommended generator for faster builds
- **Colored diagnostics**: Enhanced error/warning output with Clang
- **lld linker**: Faster linking on Windows when available

## Dependencies Managed by Conan 2

- nlohmann_json 3.11.3
- spdlog 1.12.0
- fmt 10.2.1
- gtest 1.14.0

## Troubleshooting

### Clang not found
Ensure Clang is in your PATH or specify the full path:
```powershell
cmake .. -DCMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang++.exe"
```

### Conan profile issues
Create a new profile:
```powershell
conan profile detect --force
conan profile show default
```

### ccache not working
Check if ccache is in PATH:
```powershell
ccache --version
```

### Conan cppstd error
If you get "The compiler.cppstd is not defined" error, ensure you include the cppstd setting:
```powershell
conan install . --build=missing -s compiler=clang -s compiler.version=17 -s compiler.cppstd=17 -s build_type=Release
```

### Debug build
For debug builds, change the build_type:
```powershell
conan install . --build=missing -s compiler=clang -s compiler.version=17 -s compiler.cppstd=17 -s build_type=Debug
cmake --preset conan-debug  # or clang-debug
cmake --build --preset conan-debug
```

### fmt package build errors on Windows + Clang
If you encounter build errors with fmt package, use MSVC for dependencies and Clang for your code:

1. **Install dependencies with MSVC (recommended approach):**
```powershell
conan install . --build=missing -s compiler=msvc -s compiler.version=194 -s compiler.cppstd=17 -s build_type=Release
```

2. **Then configure CMake to use Clang for your project:**
```powershell
cmake --preset conan-default -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang
cmake --build --preset conan-release
```

3. **Clean cache if needed:**
```powershell
conan remove "fmt/*" -c
# Then run step 1 again
```

This hybrid approach uses stable MSVC-built dependencies while compiling your project code with Clang for better diagnostics and modern C++ features.