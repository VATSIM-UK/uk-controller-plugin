#include "pch/pch.h"
#include "ownership/AirfieldOwnershipModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "controller/ActiveCallsignCollection.h"
#include "mock/MockDependencyLoader.h"
#include "command/CommandHandlerCollection.h"

using UKControllerPlugin::Ownership::AirfieldOwnershipModule;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPluginTest::Dependency::MockDependencyLoader;
using UKControllerPlugin::Command::CommandHandlerCollection;
using ::testing::Test;
using ::testing::NiceMock;

namespace UKControllerPluginTest {
    namespace Ownership {

        class AirfieldOwnershipModuleTest : public Test
        {
            public:

                void SetUp()
                {
                    this->container.activeCallsigns.reset(new ActiveCallsignCollection);
                    this->container.commandHandlers.reset(new CommandHandlerCollection);
                }

                PersistenceContainer container;
                NiceMock<MockDependencyLoader> dependency;
        };

        TEST_F(AirfieldOwnershipModuleTest, BootstrapPluginRegistersWithCommandHandlers)
        {
            EXPECT_EQ(0, this->container.commandHandlers->CountHandlers());
            AirfieldOwnershipModule::BootstrapPlugin(this->container, this->dependency);
            EXPECT_EQ(1, this->container.commandHandlers->CountHandlers());
        }

        TEST_F(AirfieldOwnershipModuleTest, BootstrapPluginRegistersWithActiveCallsigns)
        {
            EXPECT_EQ(0, this->container.activeCallsigns->CountHandlers());
            AirfieldOwnershipModule::BootstrapPlugin(this->container, this->dependency);
            EXPECT_EQ(1, this->container.activeCallsigns->CountHandlers());
        }

        TEST_F(AirfieldOwnershipModuleTest, ItCreatesAirfieldOwnershipManager)
        {
            AirfieldOwnershipModule::BootstrapPlugin(this->container, this->dependency);
            EXPECT_NO_THROW(this->container.airfieldOwnership->Flush());
        }
    }  // namespace Ownership
}  // namespace UKControllerPluginTest
