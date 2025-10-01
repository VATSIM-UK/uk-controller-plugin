set(PROJECT_NAME UKControllerPluginUtils)

# Using Conan packages for dependencies

################################################################################
# Source groups
################################################################################
set(api
    "api/ApiBootstrap.cpp"
    "api/ApiBootstrap.h"
    "api/ApiException.h"
    "api/ApiHelper.cpp"
    "api/ApiHelper.h"
    "api/ApiInterface.h"
    "api/ApiNotAuthorisedException.cpp"
    "api/ApiNotFoundException.h"
    "api/ApiRequestBuilder.cpp"
    "api/ApiRequestBuilder.h"
    "api/ApiResponse.cpp"
    "api/ApiResponse.h"
    "api/ApiResponseFactory.cpp"
    "api/ApiResponseFactory.h"
    "api/ApiResponseValidator.cpp"
    "api/ApiResponseValidator.h"
    api/ApiRequestFactory.cpp api/ApiRequestFactory.h
    api/ApiSettings.cpp api/ApiSettings.h api/ApiRequestFactoryInterface.h
    api/ApiRequestPerformerInterface.h
    api/Response.cpp api/Response.h
    api/ApiRequestData.h api/ApiRequestData.cpp
    api/CurlApiRequestPerformer.cpp api/CurlApiRequestPerformer.h
    api/ApiUrlBuilder.cpp api/ApiUrlBuilder.h
    api/ApiHeaderApplicator.cpp api/ApiHeaderApplicator.h
    api/ApiCurlRequestFactory.cpp api/ApiCurlRequestFactory.h
    api/ApiRequest.cpp api/ApiRequest.h
    api/ApiFactory.cpp api/ApiFactory.h
    api/AbstractApiRequestPerformerFactory.h api/CurlApiRequestPerformerFactory.cpp
    api/CurlApiRequestPerformerFactory.h
    api/ChainableRequest.cpp api/ChainableRequest.h
    api/ApiSettingsProviderInterface.h
    api/ConfigApiSettingsProvider.cpp api/ConfigApiSettingsProvider.h
    api/ApiRequestFacade.cpp api/ApiRequestFacade.h
    api/ApiRequestException.cpp api/ApiRequestException.h
    api/ApiConfigurationListener.cpp api/ApiConfigurationListener.h
    api/ApiConfigurationListenerInterface.h
    api/ApiKeyReceivedEvent.h
    api/SetApiKeyInConfig.cpp api/SetApiKeyInConfig.h
    api/SetApiKeyInSettings.cpp api/SetApiKeyInSettings.h
    api/ApiKeyRedirectUrlBuilder.cpp api/ApiKeyRedirectUrlBuilder.h)
source_group("api" FILES ${api})

set(collection
        collection/Collection.h
        collection/Collection.tpp collection/CollectionIterator.h collection/CollectionIterator.tpp)
source_group("src\\collection" FILES ${collection})

set(curl
    "curl/CurlApi.cpp"
    "curl/CurlApi.h"
    "curl/CurlInterface.h"
    "curl/CurlRequest.cpp"
    "curl/CurlRequest.h"
    "curl/CurlResponse.cpp"
    "curl/CurlResponse.h"
    "curl/HttpException.h"
    curl/CurlInterface.cpp)
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
        dialog/DialogData.cpp)
source_group("dialog" FILES ${dialog})

set(duplicate
    "duplicate/DuplicatePlugin.cpp"
    "duplicate/DuplicatePlugin.h"
)
source_group("duplicate" FILES ${duplicate})

set(eventhandler
        eventhandler/EventHandler.h
        eventhandler/EventBus.h eventhandler/EventBus.cpp
        eventhandler/MutableEventBus.h eventhandler/MutableEventBus.cpp
        eventhandler/EventStream.tpp eventhandler/EventStream.h
        eventhandler/EventHandlerFlags.h eventhandler/EventBus.tpp eventhandler/EventObserver.h eventhandler/EventBusFactory.h eventhandler/StandardEventBusFactory.cpp eventhandler/StandardEventBusFactory.h eventhandler/EuroscopeThreadEventSink.h eventhandler/EuroscopeThreadEventProcessor.cpp eventhandler/EuroscopeThreadEventProcessor.h eventhandler/DrainableEuroscopeThreadEventSink.h)
source_group("eventhandler" FILES ${eventhandler})

set(helper
    "helper/HelperFunctions.cpp"
    "helper/HelperFunctions.h"
)
source_group("helper" FILES ${helper})

set(http
        http/HttpStatusCode.h
        http/HttpMethod.h
        http/HttpMethod.cpp)
source_group("http" FILES ${http})

set(log
    "log/LoggerBootstrap.cpp"
    "log/LoggerBootstrap.h"
    "log/LoggerFunctions.cpp"
    "log/LoggerFunctions.h"
        log/ApiLogger.cpp
        log/ApiLogger.h
        log/ApiLoggerInterface.h
)
source_group("log" FILES ${log})

set(setting
    "setting/SettingRepository.cpp"
    "setting/SettingRepository.h"
    "setting/SettingRepositoryFactory.cpp"
    "setting/SettingRepositoryFactory.h"
    setting/SettingRepositoryInterface.h
    setting/SettingProviderInterface.h
    setting/JsonFileSettingProvider.cpp setting/JsonFileSettingProvider.h)
source_group("setting" FILES ${setting})

set(squawk
    "squawk/ApiSquawkAllocation.h"
    "squawk/SquawkValidator.cpp"
    "squawk/SquawkValidator.h"
        squawk/ApiSquawkAllocation.cpp)
source_group("squawk" FILES ${squawk})

set(srd
    "srd/SrdSearchParameters.h"
)
source_group("srd" FILES ${srd})

set(string
        string/StringTrimFunctions.cpp string/StringTrimFunctions.h)
source_group("string" FILES ${string})

set(task
        "task/TaskRunner.cpp"
        "task/TaskRunner.h"
        "task/TaskRunnerInterface.h"
        task/RunAsyncTask.cpp ../utils/task/RunAsyncTask.h)
source_group("task" FILES ${task})

set(update
    "update/CheckDevelopmentVersion.cpp"
    "update/LoadChangelog.cpp"
    "update/LoadChangelog.h"
    "update/PluginVersion.cpp"
    "update/PluginVersion.h"
    "update/UpdateBinaries.cpp"
    "update/UpdateBinaries.h"
        update/BootstrapReleaseChannelSettings.cpp update/BootstrapReleaseChannelSettings.h)
source_group("update" FILES ${update})

set(windows
    "windows/WinApi.cpp"
    "windows/WinApi.h"
    "windows/WinApiBootstrap.cpp"
    "windows/WinApiBootstrap.h"
    "windows/WinApiInterface.h"
        windows/WinApiInterface.cpp)
source_group("windows" FILES ${windows})

set(ALL_FILES
    ${api}
    ${collection}
    ${curl}
    ${data}
    ${dialog}
    ${duplicate}
    ${eventhandler}
    ${helper}
    ${http}
    ${log}
    ${setting}
    ${squawk}
    ${srd}
    ${string}
    ${task}
    ${update}
    ${windows})

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})
# No special properties needed for 64-bit build

# Link Conan dependencies first so they're available for precompiled headers
find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(CURL REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        nlohmann_json::nlohmann_json
        spdlog::spdlog_header_only
        fmt::fmt
        CURL::libcurl
)

# Temporarily disable precompiled headers while fixing build system
# target_precompile_headers(${PROJECT_NAME} PRIVATE "pch/pch.h")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE UKControllerPluginUtils)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/lib/"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "$ENV{CURL_INCLUDEDIR}"
)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party"
    "    # Using Conan spdlog instead of bundled version"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/continuable/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/function2/include"
)

################################################################################
# Link libraries
################################################################################
target_link_libraries(${PROJECT_NAME} PUBLIC
    CURL::libcurl
    nlohmann_json::nlohmann_json
    spdlog::spdlog_header_only
    fmt::fmt
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
        /std:c++latest;
        /sdl;
        /W4;
        /WX;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
        # Suppress non-critical warnings
        $<$<CXX_COMPILER_ID:MSVC>:/wd4100> # unreferenced formal parameter
        $<$<CXX_COMPILER_ID:MSVC>:/wd4458> # declaration hides class member
        $<$<CXX_COMPILER_ID:MSVC>:/wd4244> # conversion warnings
        $<$<CXX_COMPILER_ID:MSVC>:/wd4267> # size_t conversion warnings
        $<$<CXX_COMPILER_ID:MSVC>:/wd4101> # unreferenced local variable
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
