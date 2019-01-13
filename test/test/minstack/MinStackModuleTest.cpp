#include "pch/pch.h"
#include "minstack/MinStackModule.h"
#include "mock/MockTaskRunnerInterface.h"
#include "mock/MockCurlApi.h"
#include "metar/MetarEventHandlerCollection.h"
#include "minstack/MinStackManager.h"
#include "curl/CurlResponse.h"
#include "minstack/TerminalControlArea.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "graphics/GdiplusBrushes.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "curl/CurlRequest.h"

using UKControllerPlugin::MinStack::MinStackModule;
using UKControllerPlugin::Metar::MetarEventHandlerCollection;
using UKControllerPluginTest::Curl::MockCurlApi;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;
using UKControllerPlugin::MinStack::MinStackManager;
using UKControllerPlugin::Curl::CurlResponse;
using UKControllerPlugin::MinStack::TerminalControlArea;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Curl::CurlRequest;
using ::testing::StrictMock;
using ::testing::Return;
using ::testing::_;

namespace UKControllerModulesTest {
    namespace MinStack {

        TEST(MinStackModule, BootstrapPluginCreatesTheManager)
        {
            StrictMock<MockCurlApi> mockCurl;
            MockTaskRunnerInterface mockRunner;
            MetarEventHandlerCollection metarEvents;
            std::shared_ptr<MinStackManager> manager;

            EXPECT_CALL(mockCurl, MakeCurlRequest(_))
                .Times(4)
                .WillRepeatedly(Return(CurlResponse("BKN002, Q1010", false, 200L)));

            MinStackModule::BootstrapPlugin(manager, metarEvents, mockRunner, mockCurl);
            EXPECT_NO_THROW(manager->HasTerminalControlArea("TESTTMA"));
        }

        TEST(MinStackModule, BootstrapPluginRegistersManagerForMetarEvents)
        {
            StrictMock<MockCurlApi> mockCurl;
            MockTaskRunnerInterface mockRunner;
            MetarEventHandlerCollection metarEvents;
            std::shared_ptr<MinStackManager> manager;

            EXPECT_CALL(mockCurl, MakeCurlRequest(_))
                .Times(4)
                .WillRepeatedly(Return(CurlResponse("BKN002, Q1010", false, 200L)));

            MinStackModule::BootstrapPlugin(manager, metarEvents, mockRunner, mockCurl);
            EXPECT_EQ(1, metarEvents.CountHandlers());
        }

        TEST(MinStackModule, BootstrapPluginAddsLondonTmaAndGetsMinStack)
        {
            StrictMock<MockCurlApi> mockCurl;
            MockTaskRunnerInterface mockRunner;
            MetarEventHandlerCollection metarEvents;
            std::shared_ptr<MinStackManager> manager;

            EXPECT_CALL(mockCurl, MakeCurlRequest(_))
                .Times(3)
                .WillRepeatedly(Return(CurlResponse("BKN002, Q1010", false, 200L)));

            EXPECT_CALL(
                    mockCurl,
                    MakeCurlRequest(CurlRequest(MinStackModule::metarUrl + "EGLL", CurlRequest::METHOD_GET))
                )
                .Times(1)
                .WillOnce(Return(CurlResponse("BKN002, Q1013", false, 200L)));

            MinStackModule::BootstrapPlugin(manager, metarEvents, mockRunner, mockCurl);
            EXPECT_TRUE(manager->HasTerminalControlArea("LTMA"));
            EXPECT_EQ(70, manager->GetAllTmas()[0]->GetCurrentMinStackInt());
        }

        TEST(MinStackModule, BootstrapPluginAddsManchesterTmaAndGetsMinStack)
        {
            StrictMock<MockCurlApi> mockCurl;
            MockTaskRunnerInterface mockRunner;
            MetarEventHandlerCollection metarEvents;
            std::shared_ptr<MinStackManager> manager;

            EXPECT_CALL(mockCurl, MakeCurlRequest(_))
                .Times(3)
                .WillRepeatedly(Return(CurlResponse("BKN002, Q1010", false, 200L)));

            EXPECT_CALL(
                    mockCurl,
                    MakeCurlRequest(CurlRequest(MinStackModule::metarUrl + "EGCC", CurlRequest::METHOD_GET))
                )
                .Times(1)
                .WillOnce(Return(CurlResponse("BKN002, Q1013", false, 200L)));

            MinStackModule::BootstrapPlugin(manager, metarEvents, mockRunner, mockCurl);
            EXPECT_TRUE(manager->HasTerminalControlArea("MTMA"));
            EXPECT_EQ(60, manager->GetAllTmas()[1]->GetCurrentMinStackInt());
        }

        TEST(MinStackModule, BootstrapPluginAddsScottishTmaAndGetsMinStack)
        {
            StrictMock<MockCurlApi> mockCurl;
            MockTaskRunnerInterface mockRunner;
            MetarEventHandlerCollection metarEvents;
            std::shared_ptr<MinStackManager> manager;

            EXPECT_CALL(mockCurl, MakeCurlRequest(_))
                .Times(3)
                .WillRepeatedly(Return(CurlResponse("BKN002, Q1010", false, 200L)));

            EXPECT_CALL(
                    mockCurl,
                    MakeCurlRequest(CurlRequest(MinStackModule::metarUrl + "EGPF", CurlRequest::METHOD_GET))
                )
                .Times(1)
                .WillOnce(Return(CurlResponse("BKN002, Q1013", false, 200L)));

            // QNH 1013 is "low pressure" in STMA
            MinStackModule::BootstrapPlugin(manager, metarEvents, mockRunner, mockCurl);
            EXPECT_TRUE(manager->HasTerminalControlArea("STMA"));
            EXPECT_EQ(80, manager->GetAllTmas()[2]->GetCurrentMinStackInt());
        }

        TEST(MinStackModule, BootstrapPluginAddsChannelIslandsControlZoneAndGetsMinStack)
        {
            StrictMock<MockCurlApi> mockCurl;
            MockTaskRunnerInterface mockRunner;
            MetarEventHandlerCollection metarEvents;
            std::shared_ptr<MinStackManager> manager;

            EXPECT_CALL(mockCurl, MakeCurlRequest(_))
                .Times(3)
                .WillRepeatedly(Return(CurlResponse("BKN002, Q1010", false, 200L)));

            EXPECT_CALL(
                    mockCurl,
                    MakeCurlRequest(CurlRequest(MinStackModule::metarUrl + "EGJJ", CurlRequest::METHOD_GET))
                )
                .Times(1)
                .WillOnce(Return(CurlResponse("BKN002, Q1013", false, 200L)));

            MinStackModule::BootstrapPlugin(manager, metarEvents, mockRunner, mockCurl);
            EXPECT_TRUE(manager->HasTerminalControlArea("CICZ"));
            EXPECT_EQ(60, manager->GetAllTmas()[3]->GetCurrentMinStackInt());
        }

        TEST(MinStackModule, BootstrapRadarScreenAddsToFunctionEvents)
        {
            FunctionCallEventHandler functionHandlers;
            MinStackManager manager;
            RadarRenderableCollection radarRenderables;
            ConfigurableDisplayCollection configruables;
            GdiplusBrushes brushes;
            AsrEventHandlerCollection userSettingHandlers;

            MinStackModule::BootstrapRadarScreen(
                functionHandlers,
                manager,
                radarRenderables,
                configruables,
                brushes,
                userSettingHandlers
            );
            EXPECT_EQ(1, functionHandlers.CountCallbacks());
            EXPECT_EQ(0, functionHandlers.CountTagFunctions());
        }

        TEST(MinStackModule, BootstrapRadarScreenAddsToRadarRenderablesInBeforeTagsPhase)
        {
            FunctionCallEventHandler functionHandlers;
            MinStackManager manager;
            RadarRenderableCollection radarRenderables;
            ConfigurableDisplayCollection configruables;
            GdiplusBrushes brushes;
            AsrEventHandlerCollection userSettingHandlers;

            MinStackModule::BootstrapRadarScreen(
                functionHandlers,
                manager,
                radarRenderables,
                configruables,
                brushes,
                userSettingHandlers
            );
            EXPECT_EQ(1, radarRenderables.CountRenderers());
            EXPECT_EQ(1, radarRenderables.CountRenderersInPhase(radarRenderables.beforeTags));
        }

        TEST(MinStackModule, BootstrapRadarScreenRegistersScreenObjects)
        {
            FunctionCallEventHandler functionHandlers;
            MinStackManager manager;
            RadarRenderableCollection radarRenderables;
            ConfigurableDisplayCollection configruables;
            GdiplusBrushes brushes;
            AsrEventHandlerCollection userSettingHandlers;

            MinStackModule::BootstrapRadarScreen(
                functionHandlers,
                manager,
                radarRenderables,
                configruables,
                brushes,
                userSettingHandlers
            );
            EXPECT_EQ(3, radarRenderables.CountScreenObjects());
        }

        TEST(MinStackModule, BootstrapRadarScreenAddsToConfigurableDisplays)
        {
            FunctionCallEventHandler functionHandlers;
            MinStackManager manager;
            RadarRenderableCollection radarRenderables;
            ConfigurableDisplayCollection configruables;
            GdiplusBrushes brushes;
            AsrEventHandlerCollection userSettingHandlers;

            MinStackModule::BootstrapRadarScreen(
                functionHandlers,
                manager,
                radarRenderables,
                configruables,
                brushes,
                userSettingHandlers
            );
            EXPECT_EQ(1, configruables.CountDisplays());
        }

        TEST(MinStackModule, BootstrapRadarScreenAddsToUserSettingEvents)
        {
            FunctionCallEventHandler functionHandlers;
            MinStackManager manager;
            RadarRenderableCollection radarRenderables;
            ConfigurableDisplayCollection configruables;
            GdiplusBrushes brushes;
            AsrEventHandlerCollection userSettingHandlers;

            MinStackModule::BootstrapRadarScreen(
                functionHandlers,
                manager,
                radarRenderables,
                configruables,
                brushes,
                userSettingHandlers
            );
            EXPECT_EQ(1, userSettingHandlers.CountHandlers());
        }
    }  // namespace MinStack
}  // namespace UKControllerModulesTest
