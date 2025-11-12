#pragma once

#define _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING // NOLINT
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING     // NOLINT
#define _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING     // NOLINT
#define NOMINMAX 1                                            // NOLINT

#pragma warning(push)
#pragma warning(disable : 26495 26451)

// Disable deprecated-literal-operator warning for json.hpp
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-literal-operator"
#include <json/json.hpp>
#pragma clang diagnostic pop

#pragma warning(pop)

#include <Windows.h>
#include <any>
#include <filesystem>
#include <fstream>
#include <set>
#include <shlobj_core.h>
#include <string>
#include <utility>

#include "log/LoggerFunctions.h"
