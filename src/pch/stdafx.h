#pragma once
#define _WIN32_WINNT 0x0603
// Needs to go here, because windows.
#include "date/include/date/date.h"
#include <afxwin.h>

// Custom headers
#include "../../resource/resource.h"
#include "json/json.hpp"
#include "spdlog/include/spdlog/logger.h"
#include "spdlog/include/spdlog/sinks/file_sinks.h"
#include "spdlog/include/spdlog/sinks/null_sink.h"
#include "log/LoggerFunctions.h"
#include "euroscope/EuroScopePlugIn.h"

// Standard headers
#include <afxspinbuttonctrl.h>
#include <afxcolorbutton.h>
#include <cctype>
#include <ctime>
#include <string>
#include <map>
#include <mutex>
#include <vector>
#include <iterator>
#include <sstream>
#include <afxbutton.h>
#include <queue>
#include <gdiplus.h>
#include <windowsx.h>
#include <set>
#include <fstream>
#include <mmsystem.h>
#include <gdiplusgraphics.h>
#include <gdiplustypes.h>
#include <gdiplusenums.h>
#include <thread>
#include <regex>
#include <type_traits>
#include <gdipluspixelformats.h>
#include <unordered_set>
