project(GoogleMock)
set(GOOGLETEST_VERSION "1.11.0")
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Remove -m32 flags for MSVC/clang-cl compatibility
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND NOT CMAKE_CXX_SIMULATE_ID))
    set(CMAKE_CXX_FLAGS -m32)
endif()
add_subdirectory(
    "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/googletest/googlemock"
    "${CMAKE_BINARY_DIR}/googlemock"
)
