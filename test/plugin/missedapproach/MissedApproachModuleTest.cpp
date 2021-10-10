#include "bootstrap/PersistenceContainer.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "missedapproach/MissedApproachModule.h"
#include "plugin/FunctionCallEventHandler.h"
#include "push/PushEventProcessorCollection.h"
#include "timedevent/TimedEventCollection.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "radarscreen/RadarRenderableCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Euroscope::UserSettingAwareCollection;
using UKControllerPlugin::MissedApproach::BootstrapPlugin;
using UKControllerPlugin::MissedApproach::BootstrapRadarScreen;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Push::PushEventProcessorCollection;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;

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
        }

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

    TEST_F(MissedApproachModuleTest, ItRegistersTheRenderer)
    {
        BootstrapRadarScreen(container, renderers, configurableDisplays, asrHandlers);
        EXPECT_EQ(1, renderers.CountRenderers());
        EXPECT_EQ(1, renderers.CountRenderersInPhase(RadarRenderableCollection::afterTags));
    }

    TEST_F(MissedApproachModuleTest, ItRegistersTheRenderOptionsForAsrEvents)
    {
        BootstrapRadarScreen(container, renderers, configurableDisplays, asrHandlers);
        EXPECT_EQ(1, asrHandlers.CountHandlers());
    }
} // namespace UKControllerPluginTest::MissedApproach
