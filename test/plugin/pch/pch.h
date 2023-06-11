#pragma once
#ifndef ISOLATION_AWARE_ENABLED
#define ISOLATION_AWARE_ENABLED 1 // NOLINT
#endif
#define _WIN32_WINNT 0x0603
#define _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING // NOLINT
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING     // NOLINT
#define _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING     // NOLINT

// Resource identifiers
#include "resource.h"

// Ignore warnings about uninitialised variables in the Gmock headers
#pragma warning(push)
#pragma warning(disable : 26495 26451 28251)
#include "gmock/gmock.h"
#include "helper/Matchers.h"
#include "json/json.hpp"
#pragma warning(pop)

// Windows and standard
#include "fmt/include/fmt/chrono.h"
#include "Windows.h"
#include "ShlObj.h"
#include "shtypes.h"

#include <any>
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <gdiplus.h>
#include <gdiplusgraphics.h>
#include <gdiplustypes.h>
#include <gdiplusenums.h>
#include <list>
#include <mutex>
#include <queue>
#include <regex>
#include <set>
#include <string>
#include <typeindex>
#include <unordered_set>

// Euroscope
#include "euroscope/EuroScopePlugIn.h"

// Testingutils mocks
#include "../../testingutils/mock/MockApiInterface.h"
#include "../../testingutils/mock/MockApiSettingsProvider.h"
#include "../../testingutils/mock/MockCurlApi.h"
#include "../../testingutils/mock/MockDialogProvider.h"
#include "../../testingutils/mock/MockWinApi.h"

// Test case
#include "../../testingutils/test/ApiTestCase.h"
#include "../../testingutils/test/ApiMethodExpectation.h"
#include "../../testingutils/test/ApiRequestExpectation.h"
#include "../../testingutils/test/ApiResponseExpectation.h"
#include "../../testingutils/test/ApiUriExpectation.h"

// Plugin mocks
#include "../mock/MockAbstractTimedEvent.h"
#include "../mock/MockActiveCallsignEventHandler.h"
#include "../mock/MockAircraftFirExitGenerator.h"
#include "../mock/MockAircraftIntentionCodeGenerator.h"
#include "../mock/MockAircraftTypeMapper.h"
#include "../mock/MockAsrEventHandlerInterface.h"
#include "../mock/MockBootstrapProvider.h"
#include "../mock/MockCallsignSelectionProvider.h"
#include "../mock/MockCondition.h"
#include "../mock/MockConfigurableDisplay.h"
#include "../mock/MockConnection.h"
#include "../mock/MockControllerStatusEventHandlerInterface.h"
#include "../mock/MockDepartureHandoffResolver.h"
#include "../mock/MockDependencyLoader.h"
#include "../mock/MockEuroScopeCControllerInterface.h"
#include "../mock/MockEuroScopeCFlightplanInterface.h"
#include "../mock/MockEuroScopeCRadarTargetInterface.h"
#include "../mock/MockEuroscopeCoordinateInterface.h"
#include "../mock/MockEuroscopeExtractedRouteInterface.h"
#include "../mock/MockEuroscopeFlightplanList.h"
#include "../mock/MockEuroscopePluginLoopbackInterface.h"
#include "../mock/MockEuroscopeRadarScreenLoopbackInterface.h"
#include "../mock/MockEuroscopeSectorFileElementInterface.h"
#include "../mock/MockExternalMessageHandlerInterface.h"
#include "../mock/MockExitDetermination.h"
#include "../mock/MockFlightPlanEventHandlerInterface.h"
#include "../mock/MockFlightplanRadarTargetPair.h"
#include "../mock/MockGraphicsInterface.h"
#include "../mock/MockIntegrationActionProcessor.h"
#include "../mock/MockIntegrationDataInitialiser.h"
#include "../mock/MockListItemProvider.h"
#include "../mock/MockMenuToggleableDisplay.h"
#include "../mock/MockMetarEventHandler.h"
#include "../mock/MockOutboundIntegrationEventHandler.h"
#include "../mock/MockPluginSettingsProvider.h"
#include "../mock/MockPopupList.h"
#include "../mock/MockPrenoteMessageEventHandlerInterface.h"
#include "../mock/MockPrenoteUserRelevanceChecker.h"
#include "../mock/MockPushEventConnection.h"
#include "../mock/MockPushEventProcessor.h"
#include "../mock/MockRadarRendererableInterface.h"
#include "../mock/MockRadarTargetEventHandlerInterface.h"
#include "../mock/MockRunwayDialogAwareInterface.h"
#include "../mock/MockSectorFileProviderInterface.h"
#include "../mock/MockSidMapperInterface.h"
#include "../mock/MockSocket.h"
#include "mock/MockTaskRunnerInterface.h"
#include "../mock/MockUserSettingAwareInterface.h"
#include "../mock/MockUserSettingProviderInterface.h"
#include "../mock/MockWakeCategoryMapper.h"

// Test cases
#include "../test/BootstrapProviderTestCase.h"
