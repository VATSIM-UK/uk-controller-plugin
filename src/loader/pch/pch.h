#pragma once

#define _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING // NOLINT
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING // NOLINT
#define _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING  // NOLINT
#define NOMINMAX 1 // NOLINT

#pragma warning(push)
#pragma warning(disable : 26495 26451)
#include <json/json.hpp>
#include <spdlog/include/spdlog/logger.h>
#include <spdlog/include/spdlog/sinks/file_sinks.h>
#include <spdlog/include/spdlog/sinks/null_sink.h>
#pragma warning(pop)

#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <set>
#include <shlobj_core.h>
#include <string>
#include <utility>

#include "log/LoggerFunctions.h"
