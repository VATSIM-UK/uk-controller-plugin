from conan import ConanFile
from conan.tools.cmake import CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.build import check_min_cppstd
from conan.tools.scm import Version
import os


class UKControllerPluginConan(ConanFile):
    name = "uk-controller-plugin"
    version = "1.0.0"
    
    # Package metadata
    license = "MIT"
    author = "VATSIM UK"
    url = "https://github.com/VATSIM-UK/uk-controller-plugin"
    description = "UK Controller Plugin for EuroScope"
    topics = ("vatsim", "euroscope", "atc", "flight-simulation")
    
    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    
    # No options needed for a consumer-only conanfile
    # options = {}
    # default_options = {}
    
    def requirements(self):
        # Core dependencies
        self.requires("nlohmann_json/3.11.3")
        self.requires("spdlog/1.12.0") 
        self.requires("fmt/10.2.1")
        self.requires("libcurl/8.4.0")
        
        # Test dependencies
        self.test_requires("gtest/1.14.0")
    
    def configure(self):
        # Set C++17 as the standard if not already set
        if not self.settings.get_safe("compiler.cppstd"):
            self.settings.compiler.cppstd = "17"
        
        # Ensure static linking for dependencies
        self.options["spdlog/*"].shared = False
        self.options["spdlog/*"].header_only = True  # Use header-only spdlog to avoid linking issues
        self.options["fmt/*"].shared = False
        
        # Windows + Clang specific configuration
        if self.settings.os == "Windows" and self.settings.compiler == "clang":
            # Use MSVC runtime for better compatibility
            self.settings.compiler.libcxx = "libstdc++11"
    
    def layout(self):
        cmake_layout(self)
    
    def validate(self):
        # Ensure minimum C++17 support
        check_min_cppstd(self, "17")
        
        # Ensure minimum CMake version
        if self.settings.os == "Windows" and self.settings.compiler == "clang":
            # Clang on Windows requires newer CMake
            pass  # Will be handled in build_requirements
    
    def build_requirements(self):
        # Require CMake 3.20 or greater
        self.tool_requires("cmake/[>=3.20]")
        
        # For Windows + Clang, we might need specific build tools
        if self.settings.os == "Windows" and self.settings.compiler == "clang":
            # Ensure we have ninja for better build performance
            self.tool_requires("ninja/1.11.1")
    
    def generate(self):
        # Generate CMake dependencies
        deps = CMakeDeps(self)
        deps.generate()
        
        # Generate CMake toolchain  
        tc = CMakeToolchain(self)
        
        # Set C++17 as minimum standard
        tc.variables["CMAKE_CXX_STANDARD"] = "17"
        tc.variables["CMAKE_CXX_STANDARD_REQUIRED"] = "ON"
        tc.variables["CMAKE_CXX_EXTENSIONS"] = "OFF"
        
        # Enable compile_commands.json for better IDE support
        tc.variables["CMAKE_EXPORT_COMPILE_COMMANDS"] = "ON"
        
        # Set architecture properly
        if self.settings.arch == "x86_64":
            tc.variables["CMAKE_VS_PLATFORM_NAME"] = "x64"
        
        # Enable ccache if available
        tc.variables["CMAKE_C_COMPILER_LAUNCHER"] = "ccache"
        tc.variables["CMAKE_CXX_COMPILER_LAUNCHER"] = "ccache"
        
        tc.generate()