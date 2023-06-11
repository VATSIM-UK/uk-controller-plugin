#include "bootstrap/PersistenceContainer.h"
#include "controller/ActiveCallsignCollection.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "eventhandler/EventHandlerFlags.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "squawk/ResetSquawkOnFailedDelete.h"
#include "squawk/SquawkAssignmentDeleteForConspicuityFailedEvent.h"
#include "squawk/SquawkEventHandler.h"
#include "squawk/SquawkModule.h"
#include "test/EventBusTestCase.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Euroscope::UserSettingAwareCollection;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Squawk::SquawkModule;
using UKControllerPlugin::TimedEvent::TimedEventCollection;

namespace UKControllerPluginTest::Squawk {

    class SquawkModuleTest : public UKControllerPluginUtilsTest::EventBusTestCase
    {
        public:
        void SetUp() override
        {
            UKControllerPluginUtilsTest::EventBusTestCase::SetUp();
            this->container.flightplanHandler = std::make_unique<FlightPlanEventHandlerCollection>();
            this->container.pluginFunctionHandlers = std::make_unique<FunctionCallEventHandler>();
            this->container.timedHandler = std::make_unique<TimedEventCollection>();
            this->container.userSettingHandlers = std::make_shared<UserSettingAwareCollection>();
            this->container.activeCallsigns = std::make_shared<ActiveCallsignCollection>();
        }

        PersistenceContainer container;
    };

    TEST_F(SquawkModuleTest, BootstrapPluginRegistersForFlightplanEvents)
    {
        SquawkModule::BootstrapPlugin(container, false);
        EXPECT_EQ(1, this->container.flightplanHandler->CountHandlers());
    }

    TEST_F(SquawkModuleTest, BootstrapPluginRegistersForTimedEvents)
    {
        SquawkModule::BootstrapPlugin(container, false);
        EXPECT_EQ(1, container.timedHandler->CountHandlersForFrequency(SquawkModule::allocationCheckFrequency));
    }

    TEST_F(SquawkModuleTest, BootstrapPluginRegistersEventHandlerForTimedEvents)
    {
        SquawkModule::BootstrapPlugin(container, false);
        EXPECT_EQ(
            1, this->container.timedHandler->CountHandlersForFrequency(SquawkModule::squawkAssignmentsCheckFrequency));
    }

    TEST_F(SquawkModuleTest, BootstrapPluginRegistersEventHandlerForActiveCallsignEvents)
    {
        SquawkModule::BootstrapPlugin(container, false);
        EXPECT_EQ(1, this->container.activeCallsigns->CountHandlers());
    }

    TEST_F(SquawkModuleTest, BootstrapPluginRegistersFunctionCallbacks)
    {
        SquawkModule::BootstrapPlugin(container, false);
        EXPECT_EQ(3, this->container.pluginFunctionHandlers->CountTagFunctions());
        EXPECT_EQ(2, this->container.pluginFunctionHandlers->CountCallbacks());
    }

    TEST_F(SquawkModuleTest, BootstrapPluginRegistersForUserSettingsEvents)
    {
        SquawkModule::BootstrapPlugin(container, false);
        EXPECT_EQ(1, this->container.userSettingHandlers->Count());
    }

    TEST_F(SquawkModuleTest, BootstrapPluginDisablesAutomaticGenerationWhereRequired)
    {
        SquawkModule::BootstrapPlugin(container, true);
        EXPECT_TRUE(this->container.squawkEvents->AutomaticAssignmentsDisabled());
    }

    TEST_F(SquawkModuleTest, BootstrapPluginRegistersResetSquawkOnFailedDelete)
    {
        SquawkModule::BootstrapPlugin(container, true);
        AssertSingleEventHandlerRegistrationForEvent<
            UKControllerPlugin::Squawk ::SquawkAssignmentDeleteForConspicuityFailedEvent>();
        AssertHandlerRegisteredForEvent<
            UKControllerPlugin::Squawk ::ResetSquawkOnFailedDelete,
            UKControllerPlugin::Squawk ::SquawkAssignmentDeleteForConspicuityFailedEvent>(
            UKControllerPluginUtils::EventHandler::EventHandlerFlags::EuroscopeThread);
    }

    TEST_F(SquawkModuleTest, BootstrapPluginRegistersSquawkAssignmentMenuTagFunction)
    {
        SquawkModule::BootstrapPlugin(container, true);
        EXPECT_TRUE(this->container.pluginFunctionHandlers->HasTagFunction(9022));
    }

    TEST_F(SquawkModuleTest, BootstrapPluginRegistersSquawkAssignmentMenuCallbackFunction)
    {
        SquawkModule::BootstrapPlugin(container, true);
        EXPECT_TRUE(
            this->container.pluginFunctionHandlers->HasCallbackByDescription("Squawk Assignment Menu Callback"));
    }

    TEST_F(SquawkModuleTest, BootstrapPluginRegistersManaulSquawkEnteredCallbackFunction)
    {
        SquawkModule::BootstrapPlugin(container, true);
        EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackByDescription("Manual Squawk Entered Callback"));
    }
} // namespace UKControllerPluginTest::Squawk
