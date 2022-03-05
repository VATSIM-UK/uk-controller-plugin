set(PROJECT_NAME UKControllerPluginUtilsTest)

################################################################################
# Source groups
################################################################################
set(test__api
    "api/ApiHelperTest.cpp"
    "api/ApiRequestBuilderTest.cpp"
    "api/ApiResponseFactoryTest.cpp"
    "api/ApiResponseTest.cpp"
    "api/ApiResponseValidatorTest.cpp"
    api/ApiBootstrapTest.cpp
    api/ApiCurlRequestFactoryTest.cpp
    api/ApiFactoryTest.cpp
    api/ApiHeaderApplicatorTest.cpp
    api/ApiRequestTest.cpp
    api/ApiRequestDataTest.cpp
    api/ApiRequestExceptionTest.cpp
    api/ApiRequestFactoryTest.cpp
    api/ApiSettingsTest.cpp
    api/ApiUrlBuilderTest.cpp
    api/ConfigApiSettingsProviderTest.cpp
    api/CurlApiRequestPerformerTest.cpp
    api/CurlApiRequestPerformerFactoryTest.cpp
    api/ResponseTest.cpp)
source_group("test\\api" FILES ${test__api})

set(test__curl
    "curl/CurlRequestTest.cpp"
    "curl/CurlResponseTest.cpp"
)
source_group("test\\curl" FILES ${test__curl})

set(test__data
    "data/PluginDataLocationsTest.cpp"
)
source_group("test\\data" FILES ${test__data})

set(test__dialog
    "dialog/CompareDialogsTest.cpp"
    "dialog/DialogDataTest.cpp"
    "dialog/DialogManagerTest.cpp"
)
source_group("test\\dialog" FILES ${test__dialog})

set(test__duplicate
    "duplicate/DuplicatePluginTest.cpp"
)
source_group("test\\duplicate" FILES ${test__duplicate})

set(test__helper
    "helper/HelperFunctionsTest.cpp"
)
source_group("test\\helper" FILES ${test__helper})

set(test__http
        "helper/HelperFunctionsTest.cpp"
        http/HttpMethodTest.cpp http/HttpStatusCodeTest.cpp)
source_group("test\\http" FILES ${test__http})

set(test__log
    "log/LoggerBootstrapTest.cpp"
)
source_group("test\\log" FILES ${test__log})

set(test__mock
        mock/MockSettingProvider.h)
source_group("test\\mock" FILES ${test__mock})

set(test__pch
    "pch/pch.cpp"
    "pch/pch.h"
)
source_group("test\\pch" FILES ${test__pch})

set(test__setting
    "setting/SettingRepositoryFactoryTest.cpp"
    "setting/SettingRepositoryTest.cpp"
        setting/JsonFileSettingProviderTest.cpp)
source_group("test\\setting" FILES ${test__setting})

set(test__squawk
    "squawk/SquawkValidatorTest.cpp"
)
source_group("test\\squawk" FILES ${test__squawk})

set(test__string
        string/StringTrimFunctionTest.cpp)
source_group("test\\string" FILES ${test__string})

set(test__update
    "update/UpdateBinariesTest.cpp"
    "update/CheckDevelopmentVersionTest.cpp"
        update/BootstrapReleaseChannelSettingsTest.cpp)
source_group("test\\update" FILES ${test__update})

set(ALL_FILES
    ${test__api}
    ${test__curl}
    ${test__data}
    ${test__dialog}
    ${test__duplicate}
    ${test__helper}
    ${test__http}
    ${test__log}
    ${test__mock}
    ${test__pch}
    ${test__setting}
    ${test__squawk}
    ${test__string}
    ${test__update}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_OPTIONS "-m32" LINK_FLAGS "-m32")
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

target_precompile_headers(${PROJECT_NAME} PRIVATE "pch/pch.h")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE UKControllerPluginTest)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
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
    "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/continuable/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/function2/include"
)

################################################################################
# Link directories
################################################################################
target_link_directories(
        ${PROJECT_NAME} PUBLIC
        $ENV{CURL_DEBUG_LIBRARYDIR}
        $ENV{CURL_LIBRARYDIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../lib
        "${CMAKE_BINARY_DIR}/lib/"
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
    "_CONSOLE;"
    "STATIC_LIBCURL;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od;
            -Zm200
        >
        $<$<CONFIG:Release>:
            /O2;
            /Oi;
            /Gy;
            -Zm120
        >
        /std:c++17;
        /sdl;
        /W4;
        /WX;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /DEBUG:FASTLINK;
            /INCREMENTAL
        >
        $<$<CONFIG:Release>:
            /DEBUG;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        /NODEFAULTLIB:LIBCMT;
        /SUBSYSTEM:CONSOLE
    )
endif()

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    gmock
    UKControllerPluginTestingUtils
    UKControllerPluginUtils
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
    gmock
    UKControllerPluginTestingUtils
    UKControllerPluginUtils
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "$<$<CONFIG:Debug>:"
        "EuroScopePlugInDll;"
        "libcurl_a_debug;"
    ">"
    "$<$<CONFIG:Release>:"
        "EuroScopePluginDll;"
        "libcurl_a;"
    ">"
    "gdiplus"
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
    "crypt32;"
    "Winmm;"
    "dbghelp"
    "Ws2_32;"
    "Wldap32;"
    "normaliz;"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/lib/test"
)
