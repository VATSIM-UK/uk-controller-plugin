#include "pch/pch.h"
#include "releases/ReleaseModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "mock/MockDependencyLoader.h"
#include "websocket/WebsocketEventProcessorCollection.h"

using ::testing::NiceMock;
using ::testing::Test;
using ::testing::Return;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Releases::BootstrapPlugin;
using UKControllerPlugin::Websocket::WebsocketEventProcessorCollection;
using UKControllerPluginTest::Dependency::MockDependencyLoader;

namespace UKControllerPluginTest {
    namespace Releases {

        class ReleaseModuleTest : public Test
        {
            public:

                ReleaseModuleTest()
                {
                    container.websocketProcessors.reset(new WebsocketEventProcessorCollection);

                    nlohmann::json dependency = nlohmann::json::array();
                    dependency.push_back(
                        {
                            {"id", 1},
                            {"tag_string", "RFC"},
                            {"description", "Released For Climb"}
                        }
                    );
                    dependency.push_back(
                        {
                            {"id", 2},
                            {"tag_string", "RFD"},
                            {"description", "Released For Descent"}
                        }
                    );

                    ON_CALL(
                        this->dependencyLoader,
                        LoadDependency("DEPENDENCY_ENROUTE_RELEASE_TYPES", nlohmann::json::array())
                    )
                        .WillByDefault(Return(dependency));
                }

                NiceMock<MockDependencyLoader> dependencyLoader;
                PersistenceContainer container;
        };

        TEST_F(ReleaseModuleTest, ItRegistersForWebsocketEvents)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            EXPECT_EQ(1, this->container.websocketProcessors->CountProcessorsForChannel("private-enroute-releases"));
        }
    }  // namespace Releases
}  // namespace UKControllerPluginTest
