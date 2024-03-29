#include "minstack/MinStackModule.h"
#include "minstack/MinStackManager.h"
#include "curl/CurlResponse.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "graphics/GdiplusBrushes.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "curl/CurlRequest.h"
#include "push/PushEventProcessorCollection.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Curl::CurlResponse;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::MinStack::MinStackManager;
using UKControllerPlugin::MinStack::MinStackModule;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Push::PushEventProcessorCollection;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;

namespace UKControllerPluginTest {
    namespace MinStack {

        class MinStackModuleTest : public Test
        {
            public:
            MinStackModuleTest() : dialogManager(dialogProvider)
            {
            }
            // For the plugin tests
            NiceMock<MockApiInterface> mockApi;
            MockTaskRunnerInterface mockRunner;
            PushEventProcessorCollection pushEvents;
            std::shared_ptr<MinStackManager> manager;

            // For the radar screen tests
            NiceMock<MockDialogProvider> dialogProvider;
            DialogManager dialogManager;
            FunctionCallEventHandler functionHandlers;
            MinStackManager managerObject;
            RadarRenderableCollection radarRenderables;
            ConfigurableDisplayCollection configruables;
            GdiplusBrushes brushes;
            AsrEventHandlerCollection userSettingHandlers;
        };

        TEST_F(MinStackModuleTest, BootstrapPluginCreatesTheManager)
        {
            nlohmann::json mslData;
            mslData["airfield"] = {{"EGLL", 8000}};
            mslData["tma"] = {{"LTMA", 7000}};
            EXPECT_CALL(this->mockApi, GetMinStackLevels()).Times(1).WillRepeatedly(Return(mslData));

            MinStackModule::BootstrapPlugin(
                this->manager, this->mockRunner, this->mockApi, this->pushEvents, this->dialogManager);
            EXPECT_NO_THROW(static_cast<void>(manager->GetMslKeyTma("LTMA")));
        }

        TEST_F(MinStackModuleTest, BootstrapPluginRegistersManagerForWebsocketEvents)
        {
            nlohmann::json mslData;
            mslData["airfield"] = {{"EGLL", 8000}};
            mslData["tma"] = {{"LTMA", 7000}};
            EXPECT_CALL(this->mockApi, GetMinStackLevels()).Times(1).WillRepeatedly(Return(mslData));

            MinStackModule::BootstrapPlugin(
                this->manager, this->mockRunner, this->mockApi, this->pushEvents, this->dialogManager);
            EXPECT_EQ(1, pushEvents.CountProcessorsForChannel("private-minstack-updates"));
        }

        TEST_F(MinStackModuleTest, BootstrapPluginRegistersDialog)
        {
            nlohmann::json mslData;
            mslData["airfield"] = {{"EGLL", 8000}};
            mslData["tma"] = {{"LTMA", 7000}};
            EXPECT_CALL(this->mockApi, GetMinStackLevels()).Times(1).WillRepeatedly(Return(mslData));

            MinStackModule::BootstrapPlugin(
                this->manager, this->mockRunner, this->mockApi, this->pushEvents, this->dialogManager);
            EXPECT_EQ(1, dialogManager.CountDialogs());
            EXPECT_EQ(1, dialogManager.HasDialog(IDD_MINSTACK));
        }

        TEST_F(MinStackModuleTest, BootstrapRadarScreenAddsToFunctionEvents)
        {
            MinStackModule::BootstrapRadarScreen(
                this->functionHandlers,
                this->managerObject,
                this->radarRenderables,
                this->configruables,
                this->brushes,
                this->userSettingHandlers,
                this->dialogManager);
            EXPECT_EQ(1, functionHandlers.CountCallbacks());
            EXPECT_EQ(0, functionHandlers.CountTagFunctions());
        }

        TEST_F(MinStackModuleTest, BootstrapRadarScreenAddsToRadarRenderablesInBeforeTagsPhase)
        {
            MinStackModule::BootstrapRadarScreen(
                this->functionHandlers,
                this->managerObject,
                this->radarRenderables,
                this->configruables,
                this->brushes,
                this->userSettingHandlers,
                this->dialogManager);
            EXPECT_EQ(1, radarRenderables.CountRenderers());
            EXPECT_EQ(1, radarRenderables.CountRenderersInPhase(radarRenderables.beforeTags));
        }

        TEST_F(MinStackModuleTest, BootstrapRadarScreenRegistersScreenObjects)
        {
            MinStackModule::BootstrapRadarScreen(
                this->functionHandlers,
                this->managerObject,
                this->radarRenderables,
                this->configruables,
                this->brushes,
                this->userSettingHandlers,
                this->dialogManager);
            EXPECT_EQ(3, radarRenderables.CountScreenObjects());
        }

        TEST_F(MinStackModuleTest, BootstrapRadarScreenAddsToConfigurableDisplays)
        {
            MinStackModule::BootstrapRadarScreen(
                this->functionHandlers,
                this->managerObject,
                this->radarRenderables,
                this->configruables,
                this->brushes,
                this->userSettingHandlers,
                this->dialogManager);
            EXPECT_EQ(1, configruables.CountDisplays());
        }

        TEST_F(MinStackModuleTest, BootstrapRadarScreenAddsToUserSettingEvents)
        {
            MinStackModule::BootstrapRadarScreen(
                this->functionHandlers,
                this->managerObject,
                this->radarRenderables,
                this->configruables,
                this->brushes,
                this->userSettingHandlers,
                this->dialogManager);
            EXPECT_EQ(1, userSettingHandlers.CountHandlers());
        }
    } // namespace MinStack
} // namespace UKControllerPluginTest
