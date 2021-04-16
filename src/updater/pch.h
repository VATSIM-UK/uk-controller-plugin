// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "log/LoggerBootstrap.h"
#include <Windows.h>
#include <string>
#include <shlobj_core.h>
#include <filesystem>
#include <fstream>
#include <utility>
#include "json/json.hpp"
#include "curl/curl.h"
#endif //PCH_H
