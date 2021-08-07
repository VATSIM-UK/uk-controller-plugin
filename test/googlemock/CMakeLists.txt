set(GOOGLETEST_VERSION "1.11.0")
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(
    "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/googletest/googlemock"
    "${CMAKE_BINARY_DIR}/googlemock"
)
