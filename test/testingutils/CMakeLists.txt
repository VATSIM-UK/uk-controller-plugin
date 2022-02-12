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
    "mock/MockApiInterface.h"
    "mock/MockCurlApi.h"
    "mock/MockDialogProvider.h"
    "mock/MockTaskRunnerInterface.h"
    "mock/MockWinApi.h"
)
source_group("mock" FILES ${mock})

set(pch
    "pch/pch.cpp"
    "pch/pch.h"
)
source_group("pch" FILES ${pch})

set(ALL_FILES
    ${helper}
    ${mock}
    ${pch}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

target_precompile_headers(${PROJECT_NAME} PRIVATE "pch/pch.h")

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
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
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
        /std:c++17;
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

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    gmock
)

target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")
