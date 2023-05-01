#include "initialaltitude/InitialAltitudeModule.h"
#include "bootstrap/InitialisePlugin.h"
#include "bootstrap/PersistenceContainer.h"
#include "departure/UserShouldClearDepartureDataEvent.h"
#include "eventhandler/EventBus.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "initialaltitude/ClearInitialAltitude.h"
#include "plugin/FunctionCallEventHandler.h"
#include "controller/ActiveCallsignCollection.h"
#include "test/EventBusTestCase.h"
#include "timedevent/TimedEventCollection.h"

using testing::NiceMock;
using testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Euroscope::UserSettingAwareCollection;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::InitialAltitude::InitialAltitudeModule;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPluginUtils::EventHandler::EventBus;

namespace UKControllerPluginTest::InitialAltitude {

    class InitialAltitudeModuleTest : public UKControllerPluginUtilsTest::EventBusTestCase
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

    TEST_F(InitialAltitudeModuleTest, BootstrapPluginRegistersFlightplanEvents)
    {
        InitialAltitudeModule::BootstrapPlugin(this->container);
        EXPECT_EQ(1, container.flightplanHandler->CountHandlers());
    }

    TEST_F(InitialAltitudeModuleTest, BootstrapPluginRegistersActiveCallsignEvents)
    {
        InitialAltitudeModule::BootstrapPlugin(this->container);
        EXPECT_EQ(1, container.activeCallsigns->CountHandlers());
    }

    TEST_F(InitialAltitudeModuleTest, BootstrapPluginRegistersRecycleTagFunction)
    {
        InitialAltitudeModule::BootstrapPlugin(this->container);
        EXPECT_EQ(1, container.pluginFunctionHandlers->CountTagFunctions());
        EXPECT_TRUE(container.pluginFunctionHandlers->HasTagFunction(9002));
    }

    TEST_F(InitialAltitudeModuleTest, BootstrapPluginRegistersUserSettingsEvents)
    {
        InitialAltitudeModule::BootstrapPlugin(this->container);
        EXPECT_EQ(1, container.userSettingHandlers->Count());
    }

    TEST_F(InitialAltitudeModuleTest, BootstrapPluginRegistersTimedEvents)
    {
        InitialAltitudeModule::BootstrapPlugin(this->container);
        EXPECT_EQ(1, container.timedHandler->CountHandlers());
        EXPECT_EQ(1, container.timedHandler->CountHandlersForFrequency(10));
    }

    TEST_F(InitialAltitudeModuleTest, BootstrapPluginRegistersClearInitialAltitude)
    {
        InitialAltitudeModule::BootstrapPlugin(this->container);
        AssertSingleEventHandlerRegistrationForEvent<
            UKControllerPlugin::Departure::UserShouldClearDepartureDataEvent>();
        AssertHandlerRegisteredForEvent<
            UKControllerPlugin::InitialAltitude::ClearInitialAltitude,
            UKControllerPlugin::Departure::UserShouldClearDepartureDataEvent>(
            UKControllerPluginUtils::EventHandler::EventHandlerFlags::Sync);
    }
} // namespace UKControllerPluginTest::InitialAltitude
