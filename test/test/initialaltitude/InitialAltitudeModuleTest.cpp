#include "pch/pch.h"
#include "initialaltitude/InitialAltitudeModule.h"
#include "dependency/DependencyCache.h"
#include "bootstrap/PersistenceContainer.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "plugin/FunctionCallEventHandler.h"

using UKControllerPlugin::InitialAltitude::InitialAltitudeModule;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Euroscope::UserSettingAwareCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace InitialAltitude {

        class InitialAltitudeModuleTest : public Test
        {
            public:

                void SetUp()
                {
                    container.flightplanHandler = std::make_unique<FlightPlanEventHandlerCollection>();
                    container.userSettingHandlers = std::make_unique<UserSettingAwareCollection>();
                    container.pluginFunctionHandlers = std::make_unique<FunctionCallEventHandler>();
                }

                PersistenceContainer container;
                DependencyCache dependency;
        };

        TEST_F(InitialAltitudeModuleTest, BootstrapPluginCreatesInitialAltitudes)
        {
            InitialAltitudeModule::BootstrapPlugin(this->dependency, this->container);
            EXPECT_NO_THROW(container.initialAltitudes->HasSid("EGLL", "DET2G"));
        }

        TEST_F(InitialAltitudeModuleTest, BootstrapPluginRegistersFlightplanEvents)
        {
            InitialAltitudeModule::BootstrapPlugin(this->dependency, this->container);
            EXPECT_EQ(1, container.flightplanHandler->CountHandlers());
        }

        TEST_F(InitialAltitudeModuleTest, BootstrapPluginRegistersRecycleTagFunction)
        {
            InitialAltitudeModule::BootstrapPlugin(this->dependency, this->container);
            EXPECT_EQ(1, container.pluginFunctionHandlers->CountTagFunctions());
            EXPECT_TRUE(container.pluginFunctionHandlers->HasTagFunction(9002));
        }

        TEST_F(InitialAltitudeModuleTest, BootstrapPluginRegistersUserSettingsEvents)
        {
            InitialAltitudeModule::BootstrapPlugin(this->dependency, this->container);
            EXPECT_EQ(1, container.userSettingHandlers->Count());
        }
    }  // namespace InitialAltitude
}  // namespace UKControllerPluginTest
