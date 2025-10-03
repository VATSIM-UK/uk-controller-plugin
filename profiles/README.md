# Conan Profiles for UK Controller Plugin

This directory contains Conan profiles for building the UK Controller Plugin with different configurations.

## Available Profiles

### clang-win32
- **Compiler**: Clang 19 (clang-cl wrapper)
- **Architecture**: x86 (32-bit)
- **Build Type**: Release
- **C++ Standard**: C++20

### clang-win32-debug  
- **Compiler**: Clang 19 (clang-cl wrapper)
- **Architecture**: x86 (32-bit)
- **Build Type**: Debug
- **C++ Standard**: C++20

## Usage

To use these profiles with Conan:

```bash
# Install dependencies for Release build
conan install . --profile=profiles/clang-win32 --build=missing

# Install dependencies for Debug build  
conan install . --profile=profiles/clang-win32-debug --build=missing
```

## Requirements

- Visual Studio 2022 with Clang/LLVM support
- Conan 2.x
- CMake 3.20+

The profiles are configured to use the clang-cl wrapper which provides MSVC compatibility while using the Clang frontend.