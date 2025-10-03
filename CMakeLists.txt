cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(UKControllerPlugin CXX)

# Set C++ standard to 20 (required by third_party dependencies)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

################################################################################
# Conan setup
################################################################################
find_package(nlohmann_json REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(GTest REQUIRED)
find_package(CURL REQUIRED)

################################################################################
# Testing
################################################################################
enable_testing()

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "Win32")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32"))
    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

################################################################################
# Global compiler options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
endif()

################################################################################
# Clang-specific compiler options
################################################################################
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Remove MSVC-specific flags and add Clang equivalents
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
    
    # Clang flags for Windows DLL development - using /clang: prefix for clang-cl compatibility
    add_compile_options(
        /W4                      # High warning level
        /clang:-fms-extensions   # Enable Microsoft extensions for Windows API
        /clang:-fms-compatibility # MSVC compatibility
    )
    
    # Debug configuration
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /Zi")
    
    # Release configuration  
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
    
    # Windows-specific definitions
    add_compile_definitions(
        WIN32
        _WINDOWS
        UNICODE
        _UNICODE
    )
endif()

################################################################################
# Global linker options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_STATIC_LINKER_FLAGS "")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
endif()

################################################################################
# Global linker options for Clang on Windows
################################################################################
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Use lld-link for better Windows compatibility
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_STATIC_LINKER_FLAGS "")
endif()

################################################################################
# Nuget packages function stub.
################################################################################
function(use_package TARGET PACKAGE VERSION)
    message(WARNING "No implementation of use_package. Create yours. "
                    "Package \"${PACKAGE}\" with version \"${VERSION}\" "
                    "for target \"${TARGET}\" is ignored!")
endfunction()

################################################################################
# Common utils
################################################################################
include(CMake/Utils.cmake)

################################################################################
# Additional Global Settings(add specific info there)
################################################################################
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Build Options
################################################################################
# Temporarily disable ECFMP to fix build issues
# TODO: Update ECFMP SDK to compatible version or fix API issues
option(ENABLE_ECFMP "Enable ECFMP SDK integration" OFF)

################################################################################
# Sub-projects
################################################################################
if(ENABLE_ECFMP)
    add_subdirectory(third_party/ecfmp/src)
    message(STATUS "ECFMP SDK enabled")
else()
    message(STATUS "ECFMP SDK disabled - use -DENABLE_ECFMP=ON to enable")
endif()

add_subdirectory(src/loader)
add_subdirectory(src/plugin)
add_subdirectory(src/updater)
add_subdirectory(src/utils)
add_subdirectory(test/googlemock)
add_subdirectory(test/loader)
add_subdirectory(test/plugin)
add_subdirectory(test/testingutils)
add_subdirectory(test/updater)
add_subdirectory(test/utils)
