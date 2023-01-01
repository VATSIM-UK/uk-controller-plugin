#include "bootstrap/PersistenceContainer.h"
#include "command/CommandHandlerCollection.h"
#include "controller/ActiveCallsignCollection.h"
#include "ownership/AirfieldServiceProviderCollection.h"
#include "ownership/AirfieldOwnershipModule.h"

using ::testing::NiceMock;
using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Ownership::AirfieldOwnershipModule;
using UKControllerPluginTest::Dependency::MockDependencyLoader;

namespace UKControllerPluginTest::Ownership {

    class AirfieldOwnershipModuleTest : public Test
    {
        public:
        void SetUp() override
        {
            this->container.activeCallsigns = std::make_shared<ActiveCallsignCollection>();
            this->container.commandHandlers = std::make_unique<CommandHandlerCollection>();
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
} // namespace UKControllerPluginTest::Ownership
