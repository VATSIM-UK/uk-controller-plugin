set(PROJECT_NAME UKControllerPluginTestingUtils)

################################################################################
# Source groups
################################################################################
set(helper
    "helper/ApiRequestHelperFunctions.cpp"
    "helper/ApiRequestHelperFunctions.h"
    "helper/InitTests.cpp"
    "helper/Matchers.h"
    "helper/TestEnvironment.h"
    "helper/TestingFunctions.cpp"
    "helper/TestingFunctions.h"
    "helper/TestLogger.cpp"
    "helper/TestLogger.h"
)
source_group("helper" FILES ${helper})

set(mock
    "mock/MockApiInterface.h" mock/MockApiInterface.cpp
    "mock/MockCurlApi.h" mock/MockCurlApi.cpp
    "mock/MockDialogProvider.h" mock/MockDialogProvider.cpp
    "mock/MockTaskRunnerInterface.h" mock/MockTaskRunnerInterface.cpp
    "mock/MockWinApi.h" mock/MockWinApi.cpp
    mock/MockApiSettingsProvider.h mock/MockApiSettingsProvider.cpp
    mock/MockApiRequestPerformer.h mock/MockApiRequestPerformer.cpp
    mock/MockApiRequestPerformerFactory.h mock/MockApiRequestPerformerFactory.cpp
    mock/MockSettingRepository.h mock/MockSettingRepository.cpp )
source_group("mock" FILES ${mock})

set(test
        test/ApiTestCase.cpp test/ApiTestCase.h
        test/ApiExpectation.cpp test/ApiExpectation.h
        test/ApiMethodExpectation.h
        test/ApiUriExpectation.h
        test/ApiRequestExpectation.h
        test/ApiResponseExpectation.h
        test/EventBusTestCase.h)
source_group("test" FILES ${test})

set(ALL_FILES
    ${helper}
    ${mock}
    ${test})

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})
# No special properties needed

# Link dependencies first so they're available for precompiled headers
add_dependencies(${PROJECT_NAME}
    gmock
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    nlohmann_json::nlohmann_json
    spdlog::spdlog_header_only
    fmt::fmt
    gmock
)

# Temporarily disable precompiled headers while fixing build system
# target_precompile_headers(${PROJECT_NAME} PRIVATE "pch/pch.h")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE TestingUtils)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
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
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/.;"
)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/googletest/googlemock/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/googletest/googletest/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../resource;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/utils;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party"
    "    # Using Conan spdlog instead of bundled version"
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
        ${DEFAULT_CXX_EXCEPTION_HANDLING};
        /Y-
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()

# Dependencies now handled above before precompiled headers
