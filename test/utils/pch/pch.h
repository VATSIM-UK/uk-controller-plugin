#pragma once

// Ignore warnings about uninitialised variables in the Gmock headers
#pragma warning(push)
#pragma warning(disable : 26495 26451 28251)
#include "gmock/gmock.h"
#include "json/json.hpp"
#pragma warning(pop)

// Windows and standard headers
#include "Windows.h"
#include "KnownFolders.h"
#include "ShlObj.h"
#include "shtypes.h"

#include <chrono>
#include <filesystem>
#include <mutex>
#include <regex>
#include <string>

// Mocks
#include "mock/MockSettingProvider.h"
#include "../../testingutils/mock/MockApiInterface.h"
#include "../../testingutils/mock/MockApiRequestPerformer.h"
#include "../../testingutils/mock/MockApiRequestPerformerFactory.h"
#include "../../testingutils/mock/MockApiSettingsProvider.h"
#include "../../testingutils/mock/MockCurlApi.h"
#include "../../testingutils/mock/MockDialogProvider.h"
#include "../../testingutils/mock/MockSettingRepository.h"
#include "../../testingutils/mock/MockWinApi.h"
