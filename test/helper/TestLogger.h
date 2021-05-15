#pragma once
#include "pch/utilstestpch.h"

extern std::shared_ptr<spdlog::logger> testLogger;
std::shared_ptr<spdlog::logger> GetTestLogger(void);
