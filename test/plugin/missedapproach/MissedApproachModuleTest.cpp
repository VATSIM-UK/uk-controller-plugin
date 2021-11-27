#include "airfield/AirfieldCollection.h"
#include "bootstrap/PersistenceContainer.h"
#include "dialog/DialogManager.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "integration/IntegrationPersistenceContainer.h"
#include "integration/IntegrationServer.h"
#include "missedapproach/MissedApproachModule.h"
#include "plugin/FunctionCallEventHandler.h"
#include "push/PushEventProcessorCollection.h"
#include "timedevent/TimedEventCollection.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "radarscreen/RadarRenderableCollection.h"

using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Euroscope::UserSettingAwareCollection;
using UKControllerPlugin::Integration::IntegrationPersistenceContainer;
using UKControllerPlugin::MissedApproach::BootstrapPlugin;
using UKControllerPlugin::MissedApproach::BootstrapRadarScreen;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Push::PushEventProcessorCollection;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPluginTest::Dialog::MockDialogProvider;

namespace UKControllerPluginTest::MissedApproach {
    class MissedApproachModuleTest : public testing::Test
    {
        public:
        MissedApproachModuleTest()
        {
            container.timedHandler = std::make_unique<TimedEventCollection>();
            container.pushEventProcessors = std::make_shared<PushEventProcessorCollection>();
            container.pluginFunctionHandlers = std::make_unique<FunctionCallEventHandler>();
            container.userSettingHandlers = std::make_unique<UserSettingAwareCollection>();
            container.dialogManager = std::make_unique<DialogManager>(mockProvider);
            container.airfields = std::make_unique<AirfieldCollection>();
            container.integrationModuleContainer =
                std::make_unique<IntegrationPersistenceContainer>(nullptr, nullptr, nullptr);
        }

        testing::NiceMock<MockDialogProvider> mockProvider;
        AsrEventHandlerCollection asrHandlers;
        ConfigurableDisplayCollection configurableDisplays;
        RadarRenderableCollection renderers;
        PersistenceContainer container;
    };

    TEST_F(MissedApproachModuleTest, ItRegistersTheExpiredApproachRemover)
    {
        BootstrapPlugin(container);
        EXPECT_EQ(1, container.timedHandler->CountHandlers());
        EXPECT_EQ(1, container.timedHandler->CountHandlersForFrequency(30));
    }

    TEST_F(MissedApproachModuleTest, ItRegistersTheNewMissedApproachPushEvent)
    {
        BootstrapPlugin(container);
        EXPECT_EQ(1, container.pushEventProcessors->CountProcessorsForEvent("missed-approach.created"));
    }

    TEST_F(MissedApproachModuleTest, ItRegistersTheTriggerMissedApproachTagFunction)
    {
        BootstrapPlugin(container);
        EXPECT_TRUE(container.pluginFunctionHandlers->HasTagFunction(9020));
    }

    TEST_F(MissedApproachModuleTest, ItRegistersTheOptionsForUserSettingEvents)
    {
        BootstrapPlugin(container);
        EXPECT_EQ(1, container.userSettingHandlers->Count());
    }

    TEST_F(MissedApproachModuleTest, ItRegistersTheConfigurationDialog)
    {
        BootstrapPlugin(container);
        EXPECT_EQ(1, container.dialogManager->CountDialogs());
    }

    TEST_F(MissedApproachModuleTest, ItRegistersTheRenderers)
    {
        BootstrapRadarScreen(container, renderers, configurableDisplays, asrHandlers);
        EXPECT_EQ(2, renderers.CountRenderers());
        EXPECT_EQ(1, renderers.CountRenderersInPhase(RadarRenderableCollection::afterTags));
        EXPECT_EQ(1, renderers.CountRenderersInPhase(RadarRenderableCollection::afterLists));
    }

    TEST_F(MissedApproachModuleTest, ItRegistersTheScreenObjects)
    {
        BootstrapRadarScreen(container, renderers, configurableDisplays, asrHandlers);
        EXPECT_EQ(1, renderers.CountScreenObjects());
    }

    TEST_F(MissedApproachModuleTest, ItRegistersTheRenderersForAsrEvents)
    {
        BootstrapRadarScreen(container, renderers, configurableDisplays, asrHandlers);
        EXPECT_EQ(2, asrHandlers.CountHandlers());
    }

    TEST_F(MissedApproachModuleTest, ItRegistersTheToggleButtonConfigurable)
    {
        BootstrapRadarScreen(container, renderers, configurableDisplays, asrHandlers);
        EXPECT_EQ(2, configurableDisplays.CountDisplays());
        EXPECT_TRUE(container.pluginFunctionHandlers->HasCallbackByDescription("Toggle Missed Approach Button"));
    }

    TEST_F(MissedApproachModuleTest, ItRegistersTheConfigurationConfigurable)
    {
        BootstrapRadarScreen(container, renderers, configurableDisplays, asrHandlers);
        EXPECT_EQ(2, configurableDisplays.CountDisplays());
        EXPECT_TRUE(container.pluginFunctionHandlers->HasCallbackByDescription("Configure Missed Approaches"));
    }
} // namespace UKControllerPluginTest::MissedApproach
