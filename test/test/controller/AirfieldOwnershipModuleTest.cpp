#include "pch/pch.h"
#include "airfield/AirfieldOwnershipModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "dependency/DependencyCache.h"
#include "command/CommandHandlerCollection.h"

using UKControllerPlugin::Airfield::AirfieldOwnershipModule;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection;
using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPlugin::Command::CommandHandlerCollection;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Airfield {

        class AirfieldOwnershipModuleTest : public Test
        {
            public:

                void SetUp()
                {
                    this->container.controllerHandler.reset(new ControllerStatusEventHandlerCollection);
                    this->container.commandHandlers.reset(new CommandHandlerCollection);
                }

                PersistenceContainer container;
                DependencyCache dependency;
        };

        TEST_F(AirfieldOwnershipModuleTest, BootstrapPluginRegistersWithCommandHandlers)
        {
            EXPECT_EQ(0, this->container.commandHandlers->CountHandlers());
            AirfieldOwnershipModule::BootstrapPlugin(this->container, this->dependency);
            EXPECT_EQ(1, this->container.commandHandlers->CountHandlers());
        }

        TEST_F(AirfieldOwnershipModuleTest, BootstrapPluginRegistersWithControllerEvents)
        {
            EXPECT_EQ(0, this->container.controllerHandler->CountHandlers());
            AirfieldOwnershipModule::BootstrapPlugin(this->container, this->dependency);
            EXPECT_EQ(1, this->container.controllerHandler->CountHandlers());
        }
    }  // namespace Airfield
}  // namespace UKControllerPluginTest
