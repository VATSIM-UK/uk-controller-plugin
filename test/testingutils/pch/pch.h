#pragma once

#define _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING
#define NOMINMAX 1

// add headers that you want to pre-compile here
// Ignore warnings about uninitialised variables in the Gmock headers
#include "json/json.hpp"
#pragma warning(push)
#pragma warning(disable : 26495 26451 28251)
#include "gmock/gmock.h"
#include "helper/Matchers.h"
#pragma warning(pop)

#include <Windows.h>
#include <string>
#include <fstream>
#include <filesystem>
#define SPDLOG_WCHAR_FILENAMES
#include "spdlog/include/spdlog/logger.h"
#include "spdlog/include/spdlog/sinks/basic_file_sink.h"
#include "spdlog/include/spdlog/sinks/null_sink.h"
#include "mock/MockApiRequestPerformer.h"
#include "mock/MockApiRequestPerformerFactory.h"
#include "mock/MockApiSettingsProvider.h"
using std::max;
using std::min;
#include <gdiplus.h>
#include <gdiplusgraphics.h>
#include <gdiplustypes.h>
#include <gdiplusenums.h>
