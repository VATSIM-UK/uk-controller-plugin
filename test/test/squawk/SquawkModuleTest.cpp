#include "pch/pch.h"
#include "squawk/SquawkModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "timedevent/TimedEventCollection.h"
#include "squawk/SquawkEventHandler.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Squawk::SquawkModule;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::TimedEvent::TimedEventCollection;

namespace UKControllerPluginModuleTest {
    namespace Squawk {

        TEST(SquawkModule, BootstrapPluginRegistersForFlightplanEvents)
        {
            PersistenceContainer container;
            container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
            container.pluginFunctionHandlers.reset(new FunctionCallEventHandler);
            container.timedHandler.reset(new TimedEventCollection);

            SquawkModule::BootstrapPlugin(container, false, false);
            EXPECT_EQ(1, container.flightplanHandler->CountHandlers());
        }

        TEST(SquawkModule, BootstrapPluginRegistersAllocationsForTimedEvents)
        {
            PersistenceContainer container;
            container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
            container.pluginFunctionHandlers.reset(new FunctionCallEventHandler);
            container.timedHandler.reset(new TimedEventCollection);

            SquawkModule::BootstrapPlugin(container, false, false);
            EXPECT_EQ(
                1,
                container.timedHandler->CountHandlersForFrequency(SquawkModule::allocationCheckFrequency)
            );
        }

        TEST(SquawkModule, BootstrapPluginRegistersEventHandlerForTimedEvents)
        {
            PersistenceContainer container;
            container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
            container.pluginFunctionHandlers.reset(new FunctionCallEventHandler);
            container.timedHandler.reset(new TimedEventCollection);

            SquawkModule::BootstrapPlugin(container, false, false);
            EXPECT_EQ(
                1,
                container.timedHandler->CountHandlersForFrequency(SquawkModule::trackedAircraftCheckFrequency)
            );
        }

        TEST(SquawkModule, BootstrapPluginRegistersFunctionCallbacks)
        {
            PersistenceContainer container;
            container.flightplanHandler.reset(new FlightPlanEventHandlerCollection());
            container.pluginFunctionHandlers.reset(new FunctionCallEventHandler);
            container.timedHandler.reset(new TimedEventCollection);

            SquawkModule::BootstrapPlugin(container, false, false);
            EXPECT_EQ(2, container.pluginFunctionHandlers->CountTagFunctions());
            EXPECT_EQ(0, container.pluginFunctionHandlers->CountCallbacks());
        }

        TEST(SquawkModule, BootstrapPluginDisablesSquawksWhereInstructed)
        {
            PersistenceContainer container;
            container.flightplanHandler.reset(new FlightPlanEventHandlerCollection());
            container.pluginFunctionHandlers.reset(new FunctionCallEventHandler);
            container.timedHandler.reset(new TimedEventCollection);

            SquawkModule::BootstrapPlugin(container, true, false);
            EXPECT_TRUE(container.squawkAssignmentRules->disabled);
        }

        TEST(SquawkModule, BootstrapPluginDisablesAutomaticGenerationWhereRequired)
        {
            PersistenceContainer container;
            container.flightplanHandler.reset(new FlightPlanEventHandlerCollection());
            container.pluginFunctionHandlers.reset(new FunctionCallEventHandler);
            container.timedHandler.reset(new TimedEventCollection);

            SquawkModule::BootstrapPlugin(container, false, true);
            EXPECT_TRUE(container.squawkEvents->automaticAssignmentDisabled);
        }
    }  // namespace Squawk
}  // namespace UKControllerPluginModuleTest
