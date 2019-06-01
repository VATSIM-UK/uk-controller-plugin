#pragma once
#define _WIN32_WINNT 0x0603
#define _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING
#define _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING
#define NOMINMAX 1

// Custom headers
#include <boost/beast.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "date/include/date/date.h"
#include "../../resource/resource.h"
#include "json/json.hpp"
#include "spdlog/include/spdlog/logger.h"
#include "spdlog/include/spdlog/sinks/file_sinks.h"
#include "spdlog/include/spdlog/sinks/null_sink.h"
#include "log/LoggerFunctions.h"
#include "euroscope/EuroScopePlugIn.h"

// Standard headers
#include <algorithm>
#include <CommCtrl.h>
#include <CommDlg.h>
#include <shtypes.h>
#include <cctype>
#include <ctime>
#include <string>
#include <tchar.h>
#include <map>
#include <mutex>
#include <vector>
#include <iterator>
#include <sstream>
#include <queue>
#include <set>
#include <fstream>
#include <mmsystem.h>
#include <minmax.h>
#include <gdiplus.h>
#include <gdiplusgraphics.h>
#include <gdiplustypes.h>
#include <gdiplusenums.h>
#include <thread>
#include <regex>
#include <type_traits>
#include <gdipluspixelformats.h>
#include <unordered_set>
#include <Shobjidl.h>
#include <Shlobj.h>
#include <Windows.h>
