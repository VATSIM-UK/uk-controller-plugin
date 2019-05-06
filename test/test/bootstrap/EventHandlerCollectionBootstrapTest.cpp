#include "pch/pch.h"
#include "bootstrap/EventHandlerCollectionBootstrap.h"
#include "bootstrap/PersistenceContainer.h"

using UKControllerPlugin::Bootstrap::EventHandlerCollectionBootstrap;
using UKControllerPlugin::Bootstrap::PersistenceContainer;

using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Bootstrap {

        class EventHandlerCollectionBootstrapTest : public Test
        {
            public:
                public:

                    void SetUp()
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

        TEST_F(EventHandlerCollectionBootstrapTest, BootstrapPluginCreatesMetarEventHandler)
        {
            EXPECT_EQ(0, this->container.metarEventHandler->CountHandlers());
        }

        TEST_F(EventHandlerCollectionBootstrapTest, BootstrapPluginCreatesDefferedEventHandler)
        {
            EXPECT_EQ(0, this->container.deferredHandlers->Count());
        }

        TEST_F(EventHandlerCollectionBootstrapTest, BootstrapPluginCreatesUserSettingAwareHandler)
        {
            EXPECT_EQ(0, this->container.userSettingHandlers->Count());
        }

        TEST_F(EventHandlerCollectionBootstrapTest, BootstrapPluginCreatesCommandHandler)
        {
            EXPECT_EQ(0, this->container.commandHandlers->CountHandlers());
        }

        TEST_F(EventHandlerCollectionBootstrapTest, BootstrapPluginRegistersDeferredHandlersForTimedEvents)
        {
            EXPECT_EQ(1, this->container.timedHandler->CountHandlers());
            EXPECT_EQ(1, this->container.timedHandler->CountHandlersForFrequency(3));
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPluginTest
