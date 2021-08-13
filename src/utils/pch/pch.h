#pragma once

#define _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING // NOLINT
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING     // NOLINT
#define _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING     // NOLINT
#define NOMINMAX 1                                            // NOLINT

// add headers that you want to pre-compile here
#pragma warning(push)
#pragma warning(disable : 26495 26451)
#include <curl/curl.h>
#include <date/include/date/date.h>
#include <json/json.hpp>
#include <spdlog/include/spdlog/logger.h>
#include <spdlog/include/spdlog/sinks/file_sinks.h>
#include <spdlog/include/spdlog/sinks/null_sink.h>
#include <spdlog/include/spdlog/spdlog.h>
#pragma warning(pop)

#include <CommCtrl.h>
#include <CommDlg.h>
#include <Mmsystem.h>
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

// Custom headers
#include "log/LoggerFunctions.h"
