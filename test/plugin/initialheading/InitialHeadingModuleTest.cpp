#include "eventhandler/EventBus.h"
#include "initialheading/ClearInitialHeading.h"
#include "initialheading/InitialHeadingModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "departure/UserShouldClearDepartureDataEvent.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "controller/ActiveCallsignCollection.h"
#include "test/EventBusTestCase.h"
#include "timedevent/TimedEventCollection.h"

using ::testing::NiceMock;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Euroscope::UserSettingAwareCollection;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::InitialHeading::BootstrapPlugin;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPluginUtils::EventHandler::EventBus;

namespace UKControllerPluginTest {
    namespace InitialHeading {

        class InitialHeadingModuleTest : public UKControllerPluginUtilsTest::EventBusTestCase
        {
            public:
            void SetUp()
            {
                EventBusTestCase::SetUp();
                container.flightplanHandler = std::make_unique<FlightPlanEventHandlerCollection>();
                container.userSettingHandlers = std::make_unique<UserSettingAwareCollection>();
                container.pluginFunctionHandlers = std::make_unique<FunctionCallEventHandler>();
                container.activeCallsigns = std::make_shared<ActiveCallsignCollection>();
                container.timedHandler = std::make_unique<TimedEventCollection>();
            }

            PersistenceContainer container;
        };

        TEST_F(InitialHeadingModuleTest, BootstrapPluginRegistersFlightplanEvents)
        {
            BootstrapPlugin(this->container);
            EXPECT_EQ(1, container.flightplanHandler->CountHandlers());
        }

        TEST_F(InitialHeadingModuleTest, BootstrapPluginRegistersActiveCallsignEvents)
        {
            BootstrapPlugin(this->container);
            EXPECT_EQ(1, container.activeCallsigns->CountHandlers());
        }

        TEST_F(InitialHeadingModuleTest, BootstrapPluginRegistersRecycleTagFunction)
        {
            BootstrapPlugin(this->container);
            EXPECT_EQ(1, container.pluginFunctionHandlers->CountTagFunctions());
            EXPECT_TRUE(container.pluginFunctionHandlers->HasTagFunction(9011));
        }

        TEST_F(InitialHeadingModuleTest, BootstrapPluginRegistersUserSettingsEvents)
        {
            BootstrapPlugin(this->container);
            EXPECT_EQ(1, container.userSettingHandlers->Count());
        }

        TEST_F(InitialHeadingModuleTest, BootstrapPluginRegistersForTimedEvents)
        {
            BootstrapPlugin(this->container);
            EXPECT_EQ(1, container.timedHandler->CountHandlers());
            EXPECT_EQ(1, container.timedHandler->CountHandlersForFrequency(10));
        }

        TEST_F(InitialHeadingModuleTest, BootstrapPluginRegistersClearInitialHeading)
        {
            BootstrapPlugin(this->container);
            const auto eventStream = std::any_cast<std::shared_ptr<UKControllerPluginUtils::EventHandler::EventStream<
                UKControllerPlugin::Departure::UserShouldClearDepartureDataEvent>>>(
                EventBus::Bus().GetAnyStream(typeid(UKControllerPlugin::Departure::UserShouldClearDepartureDataEvent)));
            EXPECT_EQ(1, eventStream->Handlers().size());
            const auto handler = eventStream->Handlers()[0];
            EXPECT_EQ(UKControllerPluginUtils::EventHandler::EventHandlerFlags::Sync, handler.flags);
            EXPECT_NO_THROW(static_cast<void>(
                dynamic_cast<const UKControllerPlugin::InitialHeading::ClearInitialHeading&>(*handler.handler.get())));
        }
    } // namespace InitialHeading
} // namespace UKControllerPluginTest
