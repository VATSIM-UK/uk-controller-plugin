#include "pch/pch.h"
#include "regional/RegionalPressureModule.h"
#include "mock/MockTaskRunnerInterface.h"
#include "mock/MockApiInterface.h"
#include "regional/RegionalPressureManager.h"
#include "curl/CurlResponse.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "graphics/GdiplusBrushes.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "curl/CurlRequest.h"
#include "websocket/WebsocketEventProcessorCollection.h"
#include "mock/MockDialogProvider.h"
#include "dialog/DialogProviderInterface.h"

using UKControllerPlugin::Regional::RegionalPressureModule;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;
using UKControllerPlugin::Regional::RegionalPressureManager;
using UKControllerPlugin::Curl::CurlResponse;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Websocket::WebsocketEventProcessorCollection;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::_;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Regional {

        class RegionalPressureModuleTest : public Test
        {
            public:

                RegionalPressureModuleTest()
                    : dialogManager(dialogProvider)
                {

                }
                // For the plugin tests
                NiceMock<MockApiInterface> mockApi;
                MockTaskRunnerInterface mockRunner;
                WebsocketEventProcessorCollection websockets;
                std::shared_ptr<RegionalPressureManager> manager;

                // For the radar screen tests
                NiceMock<MockDialogProvider> dialogProvider;
                DialogManager dialogManager;
                FunctionCallEventHandler functionHandlers;
                RegionalPressureManager managerObject;
                RadarRenderableCollection radarRenderables;
                ConfigurableDisplayCollection configruables;
                GdiplusBrushes brushes;
                AsrEventHandlerCollection userSettingHandlers;
        };

        TEST_F(RegionalPressureModuleTest, BootstrapPluginCreatesTheManager)
        {
            nlohmann::json pressureData;
            pressureData = {
                {"ASR_LONDON", 1013},
                {"ASR_SCOTTISH", 1014}
            };
            EXPECT_CALL(this->mockApi, GetRegionalPressures())
                .Times(1)
                .WillRepeatedly(Return(pressureData));

            RegionalPressureModule::BootstrapPlugin(
                this->manager,
                this->mockRunner,
                this->mockApi,
                this->websockets,
                this->dialogManager
            );
            EXPECT_NO_THROW(manager->GetAllRegionalPressureKeys());
        }

        TEST_F(RegionalPressureModuleTest, BootstrapPluginRegistersManagerForWebsocketEvents)
        {
            nlohmann::json pressureData;
            pressureData = {
                {"ASR_LONDON", 1013},
                {"ASR_SCOTTISH", 1014}
            };
            EXPECT_CALL(this->mockApi, GetRegionalPressures())
                .Times(1)
                .WillRepeatedly(Return(pressureData));

            RegionalPressureModule::BootstrapPlugin(
                this->manager,
                this->mockRunner,
                this->mockApi,
                this->websockets,
                this->dialogManager
            );
            EXPECT_EQ(1, websockets.CountProcessorsForChannel("private-rps-updates"));
        }

        TEST_F(RegionalPressureModuleTest, BootstrapPluginRegistersDialog)
        {
            nlohmann::json pressureData;
            pressureData = {
                {"ASR_LONDON", 1013},
                {"ASR_SCOTTISH", 1014}
            };
            EXPECT_CALL(this->mockApi, GetRegionalPressures())
                .Times(1)
                .WillRepeatedly(Return(pressureData));

            RegionalPressureModule::BootstrapPlugin(
                this->manager,
                this->mockRunner,
                this->mockApi,
                this->websockets,
                this->dialogManager
            );
            EXPECT_EQ(1, dialogManager.CountDialogs());
            EXPECT_EQ(1, dialogManager.HasDialog(IDD_REGIONAL_PRESSURE));
        }

        TEST_F(RegionalPressureModuleTest, BootstrapRadarScreenAddsToFunctionEvents)
        {
            RegionalPressureModule::BootstrapRadarScreen(
                this->functionHandlers,
                this->managerObject,
                this->radarRenderables,
                this->configruables,
                this->brushes,
                this->userSettingHandlers,
                this->dialogManager
            );
            EXPECT_EQ(1, functionHandlers.CountCallbacks());
            EXPECT_EQ(0, functionHandlers.CountTagFunctions());
        }

        TEST_F(RegionalPressureModuleTest, BootstrapRadarScreenAddsToRadarRenderablesInBeforeTagsPhase)
        {
            RegionalPressureModule::BootstrapRadarScreen(
                this->functionHandlers,
                this->managerObject,
                this->radarRenderables,
                this->configruables,
                this->brushes,
                this->userSettingHandlers,
                this->dialogManager
            );
            EXPECT_EQ(1, radarRenderables.CountRenderers());
            EXPECT_EQ(1, radarRenderables.CountRenderersInPhase(radarRenderables.beforeTags));
        }

        TEST_F(RegionalPressureModuleTest, BootstrapRadarScreenRegistersScreenObjects)
        {
            RegionalPressureModule::BootstrapRadarScreen(
                this->functionHandlers,
                this->managerObject,
                this->radarRenderables,
                this->configruables,
                this->brushes,
                this->userSettingHandlers,
                this->dialogManager
            );
            EXPECT_EQ(3, radarRenderables.CountScreenObjects());
        }

        TEST_F(RegionalPressureModuleTest, BootstrapRadarScreenAddsToConfigurableDisplays)
        {
            RegionalPressureModule::BootstrapRadarScreen(
                this->functionHandlers,
                this->managerObject,
                this->radarRenderables,
                this->configruables,
                this->brushes,
                this->userSettingHandlers,
                this->dialogManager
            );
            EXPECT_EQ(1, configruables.CountDisplays());
        }

        TEST_F(RegionalPressureModuleTest, BootstrapRadarScreenAddsToUserSettingEvents)
        {
            RegionalPressureModule::BootstrapRadarScreen(
                this->functionHandlers,
                this->managerObject,
                this->radarRenderables,
                this->configruables,
                this->brushes,
                this->userSettingHandlers,
                this->dialogManager
            );
            EXPECT_EQ(1, userSettingHandlers.CountHandlers());
        }
    }  // namespace Regional
}  // namespace UKControllerPluginTest
