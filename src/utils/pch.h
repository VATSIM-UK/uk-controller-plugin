#pragma once

#define _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING
#define NOMINMAX 1

// add headers that you want to pre-compile here
#pragma warning( push )
#pragma warning( disable : 26495 26451)
#include "json/json.hpp"
#include "spdlog/include/spdlog/spdlog.h"
#include "spdlog/include/spdlog/logger.h"
#include "spdlog/include/spdlog/sinks/file_sinks.h"
#include "spdlog/include/spdlog/sinks/null_sink.h"
#include "curl/curl.h"
#include <date/include/date/date.h>
#include "log/LoggerFunctions.h"
#pragma warning( pop )

#include <CommCtrl.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")  // NOLINT
#include <CommDlg.h>
#include<sstream>
#include <string>
#include <shellapi.h>
#include <filesystem>
#include <stdexcept>
#include <playsoundapi.h>
#include <fstream>
#include <shtypes.h>
#include <shobjidl_core.h>
#include <set>
#include <stdexcept>
#include <regex>
#include <memory>
#include <shlobj_core.h>
