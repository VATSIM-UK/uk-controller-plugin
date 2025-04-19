set(PROJECT_NAME UKControllerPluginLoader)

################################################################################
# Source groups
################################################################################
set(src
    "dllmain.cpp"
)
source_group("src" FILES ${src})

set(src__loader
    "loader/loader.cpp"
    "loader/loader.h"
)
source_group("src\\loader" FILES ${src__loader})

set(ALL_FILES
    ${src}
    ${src__loader}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

target_precompile_headers(${PROJECT_NAME} PRIVATE "pch/pch.h")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE UKControllerPluginLoader)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG   "UKControllerPlugin"
    TARGET_NAME_RELEASE "UKControllerPlugin"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/bin/"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party"
)


################################################################################
# Link directories for cURL
################################################################################
target_link_directories(
        ${PROJECT_NAME} PUBLIC
        $ENV{CURL_DEBUG_LIBRARYDIR}
        $ENV{CURL_LIBRARYDIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../lib
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "WIN32;"
    "UKCONTROLLERPLUGINLOADER_EXPORTS;"
    "_WINDOWS;"
    "_USRDLL;"
    "CURL_STATICLIB;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /Oi;
            /Gy
        >
        /permissive-;
        /std:c++20;
        /sdl;
        /W4;
        /WX;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /INCREMENTAL
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        /NODEFAULTLIB:LIBCMT;
        /DEBUG;
        /SUBSYSTEM:WINDOWS
    )
endif()

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    UKControllerPluginUtils
    UKControllerPluginUpdater
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    UKControllerPluginUtils
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "$<$<CONFIG:Debug>:"
        "libcurl_a_debug"
    ">"
    "$<$<CONFIG:Release>:"
        "libcurl_a"
    ">"
    "kernel32;"
    "user32;"
    "gdi32;"
    "winspool;"
    "comdlg32;"
    "advapi32;"
    "shell32;"
    "ole32;"
    "oleaut32;"
    "uuid;"
    "odbc32;"
    "odbccp32;"
    "EuroScopePlugInDll;"
    "Winmm;"
    "Ws2_32;"
    "Wldap32;"
    "normaliz;"
    "crypt32"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")
