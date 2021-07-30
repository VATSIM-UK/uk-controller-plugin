set(PROJECT_NAME UKControllerPluginUpdater)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "dllmain.cpp"
)
source_group("" FILES ${no_group_source_files})

set(pch
    "pch/pch.cpp"
    "pch/pch.h"
)
source_group("pch" FILES ${pch})

set(updater
    "updater/PerformUpdates.cpp"
    "updater/PerformUpdates.h"
)
source_group("updater" FILES ${updater})

set(ALL_FILES
    ${no_group_source_files}
    ${pch}
    ${updater}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" ".")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE UKControllerPluginUpdater)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG   "${PROJECT_NAME}"
    TARGET_NAME_RELEASE "${PROJECT_NAME}"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../utils;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG;"
        ""
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
        /std:c++17;
        /sdl;
        /W3;
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
    "Shell32;"
    "Ole32;"
    "Ws2_32;"
    "Wldap32;"
    "normaliz;"
    "crypt32"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

