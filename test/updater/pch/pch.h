#pragma once
#define _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING
#define NOMINMAX 1

// Ignore warnings about uninitialised variables in the Gmock headers
#pragma warning(push)
#pragma warning(disable : 26495 26451 28251)
#include "gmock/gmock.h"
#include <nlohmann/json.hpp>
#pragma warning(pop)

// Standard and Windows
#include "Windows.h"
#include "shtypes.h"

#include <chrono>
#include <filesystem>

// Mocks
#include "../../testingutils/mock/MockApiInterface.h"
#include "../../testingutils/mock/MockCurlApi.h"
#include "../../testingutils/mock/MockWinApi.h"
