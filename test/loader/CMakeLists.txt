set(PROJECT_NAME UKControllerPluginLoaderTest)

################################################################################
# Source groups
################################################################################
set(loader
    "loader/LoaderTest.cpp"
)
source_group("loader" FILES ${loader})

set(pch
    "pch/pch.cpp"
    "pch/pch.h"
)
source_group("pch" FILES ${pch})

set(ALL_FILES
    ${loader}
    ${pch}
    $<TARGET_OBJECTS:UKControllerPluginLoader>
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

target_precompile_headers(${PROJECT_NAME} PRIVATE "pch/pch.h")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/bin/"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/"
)
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
    $<$<CONFIG:Debug>:
        MultiThreadedDebugDLL
    >
    $<$<CONFIG:Release>:
        MultiThreadedDLL
    >
    $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/.;"
)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../../resource;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party"
)

################################################################################
# Link directories
################################################################################
target_link_directories(
        ${PROJECT_NAME} PUBLIC
        $ENV{CURL_DEBUG_LIBRARYDIR}
        $ENV{CURL_LIBRARYDIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../lib
        ${CMAKE_BINARY_DIR}/lib
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "UNICODE;"
    "_UNICODE"
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "WIN32;"
    "_CONSOLE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od;
            /RTC1
        >
        /std:c++20;
        /W4;
        /WX;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /SAFESEH:NO
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF;
        >
        /NODEFAULTLIB:LIBCMT;
        /DEBUG;
        /SUBSYSTEM:CONSOLE;
    )
endif()

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    UKControllerPluginLoader
    gmock
    UKControllerPluginTestingUtils
    UKControllerPluginUpdater
    UKControllerPluginUtils
    UKControllerPluginCore
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    UKControllerPluginLoader
    gmock
    UKControllerPluginTestingUtils
    UKControllerPluginUpdater
    UKControllerPluginUtils
    UKControllerPluginCore
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "$<$<CONFIG:Debug>:"
        "libcurl_a_debug"
    ">"
    "$<$<CONFIG:Release>:"
        "libcurl_a"
    ">"
    "Winmm;"
    "Shell32;"
    "Ole32;"
    "User32;"
    "crypt32;"
    "Ws2_32;"
    "Wldap32;"
    "normaliz;"
    "advapi32"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")
