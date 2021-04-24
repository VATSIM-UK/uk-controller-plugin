#include "pch/pch.h"
#include "initialheading/InitialHeadingModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "controller/ActiveCallsignCollection.h"

using UKControllerPlugin::InitialHeading::BootstrapPlugin;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Euroscope::UserSettingAwareCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using ::testing::Test;
using ::testing::NiceMock;

namespace UKControllerPluginTest {
    namespace InitialHeading {

        class InitialHeadingModuleTest : public Test
        {
            public:

                void SetUp()
                {
                    container.flightplanHandler = std::make_unique<FlightPlanEventHandlerCollection>();
                    container.userSettingHandlers = std::make_unique<UserSettingAwareCollection>();
                    container.pluginFunctionHandlers = std::make_unique<FunctionCallEventHandler>();
                    container.activeCallsigns = std::make_unique<ActiveCallsignCollection>();
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
    } // namespace InitialHeading
}  // namespace UKControllerPluginTest
