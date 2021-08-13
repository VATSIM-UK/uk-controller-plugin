#pragma once
#define _WIN32_WINNT 0x0603
#define _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING // NOLINT
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING     // NOLINT
#define _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING     // NOLINT
#define NOMINMAX 1                                            // NOLINT
#define ISOLATION_AWARE_ENABLED 1                             // NOLINT

// Custom headers
#pragma warning(push)
#pragma warning(disable : 26495 26451)
#include "date/include/date/date.h"

#include "../../resource/resource.h"
#include "spdlog/include/spdlog/logger.h"
#include "spdlog/include/spdlog/sinks/file_sinks.h"
#include "spdlog/include/spdlog/sinks/null_sink.h"
#include "json/json.hpp"

#include "euroscope/EuroScopePlugIn.h"
#include "log/LoggerFunctions.h"
#pragma warning(pop)

// Standard headers
using std::min;
using std::max;
#include <CommCtrl.h>
#include <CommDlg.h>
#include <KnownFolders.h>
#include <Shlobj.h>
#include <Shobjidl.h>
#include <Windows.h>
#include <algorithm>
#include <cctype>
#include <codecvt>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <gdiplus.h>
#include <gdiplusenums.h>
#include <gdiplusgraphics.h>
#include <gdipluspixelformats.h>
#include <gdiplustypes.h>
#include <iterator>
#include <locale>
#include <map>
#include <minmax.h>
#include <mmsystem.h>
#include <mutex>
#include <queue>
#include <regex>
#include <set>
#include <shellapi.h>
#include <shtypes.h>
#include <sstream>
#include <string>
#include <tchar.h>
#include <thread>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <vector>
#include <ws2def.h>
#include <ws2tcpip.h>
