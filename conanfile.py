from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.files import copy
import os

class UKControllerPluginConan(ConanFile):
    name = "uk-controller-plugin"
    version = "1.0.0"
    
    settings = "os", "compiler", "build_type", "arch"
    
    def requirements(self):
        # Replace third_party dependencies with Conan packages where available
        self.requires("nlohmann_json/3.11.2")
        self.requires("fmt/10.2.1")  # Updated to match spdlog requirement
        self.requires("spdlog/1.12.0")
        self.requires("gtest/1.14.0")
        self.requires("libcurl/8.4.0")  # For HTTP client functionality
        # Note: Some dependencies like EuroScope SDK, ECFMP, cpp-httplib may need to remain as local dependencies
        
    def build_requirements(self):
        self.tool_requires("cmake/[>=3.20]")
        
    def layout(self):
        cmake_layout(self)
        
    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["CMAKE_CXX_STANDARD"] = "20"  # Updated to C++20
        tc.variables["CMAKE_CXX_STANDARD_REQUIRED"] = "ON"
        
        # Configure for Clang on Windows
        if self.settings.compiler == "clang" and self.settings.os == "Windows":
            # Use Visual Studio generator with ClangCL toolset for Windows
            tc.generator = "Visual Studio 17 2022"
            tc.generator_toolset = "ClangCL"
            tc.generator_platform = "Win32"  # For x86 architecture
        elif self.settings.compiler == "clang":
            tc.variables["CMAKE_CXX_COMPILER"] = "clang++"
            tc.variables["CMAKE_C_COMPILER"] = "clang"
        tc.generate()
        
        deps = CMakeDeps(self)
        deps.generate()
        
    def configure(self):
        # Configure libcurl to use static linking to match project requirements
        self.options["libcurl"].shared = False
        self.options["libcurl"].with_ssl = "openssl"  # Use OpenSSL for HTTPS
        
        # Configure other dependencies
        self.options["fmt"].shared = False
        self.options["spdlog"].shared = False