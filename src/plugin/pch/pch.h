#pragma once

#ifndef ISOLATION_AWARE_ENABLED
#define ISOLATION_AWARE_ENABLED 1 // NOLINT
#endif
#define _WIN32_WINNT 0x0603
#define _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING // NOLINT
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING     // NOLINT
#define _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING     // NOLINT
#define NOMINMAX 1                                            // NOLINT

// Custom headers
#pragma warning(push)
#pragma warning(disable : 26495 26451)

#include "../../resource/resource.h"
#include "json/json.hpp"
#include "fmt/include/fmt/chrono.h"

#include "log/LoggerFunctions.h"
#include "task/RunAsyncTask.h"
#pragma warning(pop)

// Standard headers
using std::max;
using std::min;
#include <ws2tcpip.h>
#include <winsock2.h>
#include <Windows.h>
#include <CommCtrl.h>
#include <CommDlg.h>
#include <KnownFolders.h>
#include <Shlobj.h>
#include <Shobjidl.h>
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
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#include "euroscope/EuroScopePlugIn.h"
