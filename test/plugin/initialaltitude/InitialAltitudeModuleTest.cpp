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
using UKControllerPlugin::EventHandler::EventBus;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::InitialAltitude::InitialAltitudeModule;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::TimedEvent::TimedEventCollection;

namespace UKControllerPluginTest::InitialAltitude {

    class InitialAltitudeModuleTest : public EventBusTestCase
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
        const auto eventStream = std::any_cast<std::shared_ptr<UKControllerPlugin::EventHandler::EventStream<
            UKControllerPlugin::Departure::UserShouldClearDepartureDataEvent>>>(
            EventBus::Bus().GetAnyStream(typeid(UKControllerPlugin::Departure::UserShouldClearDepartureDataEvent)));
        EXPECT_EQ(1, eventStream->Handlers().size());
        const auto handler = eventStream->Handlers()[0];
        EXPECT_EQ(UKControllerPlugin::EventHandler::EventHandlerFlags::Sync, handler.flags);
        EXPECT_NO_THROW(static_cast<void>(
            dynamic_cast<const UKControllerPlugin::InitialAltitude::ClearInitialAltitude&>(*handler.handler.get())));
    }
} // namespace UKControllerPluginTest::InitialAltitude
