#include "pch/pch.h"
#include "squawk/SquawkModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "timedevent/TimedEventCollection.h"
#include "squawk/SquawkEventHandler.h"
#include "euroscope/UserSettingAwareCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Squawk::SquawkModule;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPlugin::Euroscope::UserSettingAwareCollection;
using ::testing::Test;

namespace UKControllerPluginModuleTest {
    namespace Squawk {

        class SquawkModuleTest : public Test
        {
            public:

                void SetUp()
                {
                    this->container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
                    this->container.pluginFunctionHandlers.reset(new FunctionCallEventHandler);
                    this->container.timedHandler.reset(new TimedEventCollection);
                    this->container.userSettingHandlers.reset(new UserSettingAwareCollection);
                }

                PersistenceContainer container;
        };

        TEST_F(SquawkModuleTest, BootstrapPluginRegistersForFlightplanEvents)
        {
            SquawkModule::BootstrapPlugin(container, false, false);
            EXPECT_EQ(1, this->container.flightplanHandler->CountHandlers());
        }

        TEST_F(SquawkModuleTest, BootstrapPluginRegistersForTimedEvents)
        {
            SquawkModule::BootstrapPlugin(container, false, false);
            EXPECT_EQ(
                1,
                container.timedHandler->CountHandlersForFrequency(SquawkModule::allocationCheckFrequency)
            );
        }

        TEST_F(SquawkModuleTest, BootstrapPluginRegistersEventHandlerForTimedEvents)
        {
            SquawkModule::BootstrapPlugin(container, false, false);
            EXPECT_EQ(
                1,
                this->container.timedHandler->CountHandlersForFrequency(SquawkModule::trackedAircraftCheckFrequency)
            );
        }

        TEST_F(SquawkModuleTest, BootstrapPluginRegistersFunctionCallbacks)
        {
            SquawkModule::BootstrapPlugin(container, false, false);
            EXPECT_EQ(2, this->container.pluginFunctionHandlers->CountTagFunctions());
            EXPECT_EQ(0, this->container.pluginFunctionHandlers->CountCallbacks());
        }

        TEST_F(SquawkModuleTest, BootstrapPluginRegistersForUserSettingsEvents)
        {
            SquawkModule::BootstrapPlugin(container, false, false);
            EXPECT_EQ(1, this->container.userSettingHandlers->Count());
        }

        TEST_F(SquawkModuleTest, BootstrapPluginDisablesSquawksWhereInstructed)
        {
            SquawkModule::BootstrapPlugin(container, true, false);
            EXPECT_TRUE(this->container.squawkAssignmentRules->disabled);
        }

        TEST_F(SquawkModuleTest, BootstrapPluginDisablesAutomaticGenerationWhereRequired)
        {
            SquawkModule::BootstrapPlugin(container, false, true);
            EXPECT_TRUE(this->container.squawkEvents->automaticAssignmentDisabled);
        }
    }  // namespace Squawk
}  // namespace UKControllerPluginModuleTest
