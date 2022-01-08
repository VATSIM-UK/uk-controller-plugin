#include "departure/DepartureModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "push/PushEventProcessorCollection.h"
#include "tag/TagItemCollection.h"
#include "timedevent/TimedEventCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "controller/HandoffEventHandlerCollection.h"
#include "dialog/DialogManager.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "radarscreen/ConfigurableDisplayCollection.h"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::HandoffEventHandlerCollection;
using UKControllerPlugin::Departure::BootstrapRadarScreen;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Push::PushEventProcessorCollection;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPluginTest::Dependency::MockDependencyLoader;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;

namespace UKControllerPluginTest::Departure {

    class DepartureModuleTest : public Test
    {
        public:
        DepartureModuleTest()
        {
            container.tagHandler = std::make_unique<TagItemCollection>();
            container.pluginFunctionHandlers = std::make_unique<FunctionCallEventHandler>();
        }

        PersistenceContainer container;
        RadarRenderableCollection renderables;
        ConfigurableDisplayCollection configurables;
        UKControllerPlugin::Euroscope::AsrEventHandlerCollection asr;
    };

    TEST_F(DepartureModuleTest, RadarScreenAddsRenderable)
    {
        BootstrapRadarScreen(this->container, renderables, configurables, asr);
        EXPECT_EQ(1, renderables.CountRenderers());
    }

    TEST_F(DepartureModuleTest, RadarScreenAddsToAsr)
    {
        BootstrapRadarScreen(this->container, renderables, configurables, asr);
        EXPECT_EQ(1, asr.CountHandlers());
    }

    TEST_F(DepartureModuleTest, RadarScreenAddsReleaseRequestShowOption)
    {
        BootstrapRadarScreen(this->container, renderables, configurables, asr);
        EXPECT_EQ(1, configurables.CountDisplays());
        EXPECT_EQ(1, this->container.pluginFunctionHandlers->CountCallbacks());
        EXPECT_TRUE(
            this->container.pluginFunctionHandlers->HasCallbackByDescription("Toggle Departure Coordination List"));
    }
} // namespace UKControllerPluginTest::Departure
