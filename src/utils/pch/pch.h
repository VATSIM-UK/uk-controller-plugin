#pragma once

#define _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING // NOLINT
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING     // NOLINT
#define _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING     // NOLINT
#define NOMINMAX 1                                            // NOLINT

// add headers that you want to pre-compile here
#pragma warning(push)
#pragma warning(disable : 26495 26451)
#define SPDLOG_WCHAR_FILENAMES
#define CONTINUABLE_HAS_DISABLED_COROUTINE
#include <curl/curl.h>

// Disable deprecated-literal-operator warning for json.hpp
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-literal-operator"
#include <json/json.hpp>
#pragma clang diagnostic pop
#include <spdlog/include/spdlog/logger.h>
#include <spdlog/include/spdlog/sinks/basic_file_sink.h>
#include <spdlog/include/spdlog/sinks/null_sink.h>
#include <spdlog/include/spdlog/spdlog.h>
#include <fmt/include/fmt/chrono.h>
#pragma warning(pop)

#include <any>
#include <cstdint>
#include <CommCtrl.h>
#include <CommDlg.h>
#include <codecvt>
#include <deque>
#include <Mmsystem.h>
#include <iterator>
#include <filesystem>
#include <fstream>
#include <memory>
#include <playsoundapi.h>
#include <regex>
#include <set>
#include <shellapi.h>
#include <shlobj_core.h>
#include <shobjidl_core.h>
#include <shtypes.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>

// Custom headers
#include "api/ApiRequestFacade.h"
#include "continuable/continuable.hpp"
#include "log/LoggerFunctions.h"
