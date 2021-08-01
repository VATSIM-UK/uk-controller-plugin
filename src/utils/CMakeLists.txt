set(PROJECT_NAME UKControllerPluginUtils)

# Check for curl includes
if (DEFINED ENV{CURL_INCLUDEDIR})
    message("Curl include directory set to $ENV{CURL_INCLUDEDIR}")
ELSE()
    message(FATAL_ERROR "Curl include directory not set, please set ENV::CURL_INCLUDEDIR")
ENDIF()

# Check for curl libraries
if (DEFINED ENV{CURL_LIBRARYDIR})
    message("Curl library directory set to $ENV{CURL_LIBRARYDIR}")
ELSE()
    message(FATAL_ERROR "Curl library directory not set, please set ENV::CURL_LIBRARYDIR")
ENDIF()

# Check for curl libraries (debug)
if (DEFINED ENV{CURL_DEBUG_LIBRARYDIR})
    message("Curl debug library directory set to $ENV{CURL_DEBUG_LIBRARYDIR}")
ELSE()
    message(FATAL_ERROR "Curl debug library directory not set, please set ENV::CURL_DEBUG_LIBRARYDIR")
ENDIF()

################################################################################
# Source groups
################################################################################
set(api
    "api/ApiAuthChecker.cpp"
    "api/ApiAuthChecker.h"
    "api/ApiBootstrap.cpp"
    "api/ApiBootstrap.h"
    "api/ApiException.h"
    "api/ApiHelper.cpp"
    "api/ApiHelper.h"
    "api/ApiInterface.h"
    "api/ApiNotAuthorisedException.h"
    "api/ApiNotFoundException.h"
    "api/ApiRequestBuilder.cpp"
    "api/ApiRequestBuilder.h"
    "api/ApiResponse.cpp"
    "api/ApiResponse.h"
    "api/ApiResponseFactory.cpp"
    "api/ApiResponseFactory.h"
    "api/ApiResponseValidator.cpp"
    "api/ApiResponseValidator.h"
    "api/LocateApiSettings.cpp"
    "api/LocateApiSettings.h"
)
source_group("api" FILES ${api})

set(curl
    "curl/CurlApi.cpp"
    "curl/CurlApi.h"
    "curl/CurlInterface.h"
    "curl/CurlRequest.cpp"
    "curl/CurlRequest.h"
    "curl/CurlResponse.cpp"
    "curl/CurlResponse.h"
    "curl/HttpException.h"
)
source_group("curl" FILES ${curl})

set(data
    "data/PluginDataLocations.cpp"
    "data/PluginDataLocations.h"
)
source_group("data" FILES ${data})

set(dialog
    "dialog/CompareDialogs.cpp"
    "dialog/CompareDialogs.h"
    "dialog/DialogCallArgument.h"
    "dialog/DialogData.h"
    "dialog/DialogManager.cpp"
    "dialog/DialogManager.h"
    "dialog/DialogProviderInterface.h"
)
source_group("dialog" FILES ${dialog})

set(duplicate
    "duplicate/DuplicatePlugin.cpp"
    "duplicate/DuplicatePlugin.h"
)
source_group("duplicate" FILES ${duplicate})

set(helper
    "helper/HelperFunctions.cpp"
    "helper/HelperFunctions.h"
)
source_group("helper" FILES ${helper})

set(log
    "log/LoggerBootstrap.cpp"
    "log/LoggerBootstrap.h"
    "log/LoggerFunctions.cpp"
    "log/LoggerFunctions.h"
)
source_group("log" FILES ${log})

set(pch
    "pch/pch.cpp"
    "pch/pch.h"
)
source_group("pch" FILES ${pch})

set(setting
    "setting/SettingRepository.cpp"
    "setting/SettingRepository.h"
    "setting/SettingRepositoryFactory.cpp"
    "setting/SettingRepositoryFactory.h"
    "setting/SettingValue.h"
)
source_group("setting" FILES ${setting})

set(squawk
    "squawk/ApiSquawkAllocation.h"
    "squawk/SquawkValidator.cpp"
    "squawk/SquawkValidator.h"
)
source_group("squawk" FILES ${squawk})

set(srd
    "srd/SrdSearchParameters.h"
)
source_group("srd" FILES ${srd})

set(update
    "update/LoadChangelog.cpp"
    "update/LoadChangelog.h"
    "update/PluginVersion.cpp"
    "update/PluginVersion.h"
    "update/UpdateBinaries.cpp"
    "update/UpdateBinaries.h"
)
source_group("update" FILES ${update})

set(windows
    "windows/WinApi.cpp"
    "windows/WinApi.h"
    "windows/WinApiBootstrap.cpp"
    "windows/WinApiBootstrap.h"
    "windows/WinApiInterface.h"
)
source_group("windows" FILES ${windows})

set(ALL_FILES
    ${api}
    ${curl}
    ${data}
    ${dialog}
    ${duplicate}
    ${helper}
    ${log}
    ${pch}
    ${setting}
    ${squawk}
    ${srd}
    ${update}
    ${windows}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})
target_precompile_headers(${PROJECT_NAME} PRIVATE "pch/pch.h")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE UKControllerPluginUtils)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
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
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party"
    "$ENV{CURL_INCLUDEDIR}"
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
    "_LIB;"
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
            /NODEFAULTLIB:LIBCMT
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()