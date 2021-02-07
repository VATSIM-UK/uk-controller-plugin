#pragma once
#include "euroscope/EuroScopePlugIn.h"

#ifndef UKCP_API
#define UKCP_API __declspec(dllexport)
#endif

UKCP_API EuroScopePlugIn::CPlugIn * LoadPlugin(void);
UKCP_API void UnloadPlugin(void);
UKCP_API const char * GetPluginVersion(void);