#pragma once
#include "pch/pch.h"

extern std::shared_ptr<spdlog::logger> testLogger;
std::shared_ptr<spdlog::logger> GetTestLogger(void);
