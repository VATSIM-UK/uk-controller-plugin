#include "bootstrap/EventHandlerCollectionBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "command/CommandHandlerCollection.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "controller/HandoffEventHandlerCollection.h"
#include "euroscope/RadarTargetEventHandlerCollection.h"
#include "euroscope/RunwayDialogAwareCollection.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "tag/TagItemCollection.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Bootstrap::EventHandlerCollectionBootstrap;
using UKControllerPlugin::Bootstrap::PersistenceContainer;

using ::testing::Test;

namespace UKControllerPluginTest::Bootstrap {

    class EventHandlerCollectionBootstrapTest : public Test
    {
        public:
        void SetUp() override
        {
            EventHandlerCollectionBootstrap::BoostrapPlugin(container);
        };

        PersistenceContainer container;
    };

    TEST_F(EventHandlerCollectionBootstrapTest, BootstrapPluginCreatesTagHandler)
    {
        EXPECT_EQ(0, this->container.tagHandler->CountHandlers());
    }

    TEST_F(EventHandlerCollectionBootstrapTest, BootstrapPluginCreatesRadarTargetHandler)
    {
        EXPECT_EQ(0, this->container.radarTargetHandler->CountHandlers());
    }

    TEST_F(EventHandlerCollectionBootstrapTest, BootstrapPluginCreatesFlightplanHandler)
    {
        EXPECT_EQ(0, this->container.flightplanHandler->CountHandlers());
    }

    TEST_F(EventHandlerCollectionBootstrapTest, BootstrapPluginCreatesControllerHandler)
    {
        EXPECT_EQ(0, this->container.controllerHandler->CountHandlers());
    }

    TEST_F(EventHandlerCollectionBootstrapTest, BootstrapPluginCreatesTimedEventHandler)
    {
        EXPECT_NO_THROW(this->container.timedHandler->CountHandlers());
    }

    TEST_F(EventHandlerCollectionBootstrapTest, BootstrapPluginCreatesFunctionHandler)
    {
        EXPECT_EQ(0, this->container.pluginFunctionHandlers->CountCallbacks());
    }

    TEST_F(EventHandlerCollectionBootstrapTest, BootstrapPluginCreatesUserSettingAwareHandler)
    {
        EXPECT_EQ(0, this->container.userSettingHandlers->Count());
    }

    TEST_F(EventHandlerCollectionBootstrapTest, BootstrapPluginCreatesCommandHandler)
    {
        EXPECT_EQ(0, this->container.commandHandlers->CountHandlers());
    }

    TEST_F(EventHandlerCollectionBootstrapTest, BootstrapPluginCreatesRunwayDialogHandler)
    {
        EXPECT_EQ(0, this->container.runwayDialogEventHandlers->CountHandlers());
    }

    TEST_F(EventHandlerCollectionBootstrapTest, BootstrapPluginCreatesHandoffHandlers)
    {
        EXPECT_EQ(0, this->container.controllerHandoffHandlers->CountHandlers());
    }
} // namespace UKControllerPluginTest::Bootstrap
