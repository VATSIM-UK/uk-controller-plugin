#include "pch/pch.h"
#include "releases/ReleaseModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "mock/MockDependencyLoader.h"
#include "websocket/WebsocketEventProcessorCollection.h"
#include "tag/TagItemCollection.h"

using ::testing::NiceMock;
using ::testing::Test;
using ::testing::Return;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Releases::BootstrapPlugin;
using UKControllerPlugin::Tag::TagItemCollection;
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
                    container.tagHandler.reset(new TagItemCollection);

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

        TEST_F(ReleaseModuleTest, ItRegistersTagItemForEnrouteReleaseType)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            EXPECT_EQ(1, this->container.tagHandler->HasHandlerForItemId(108));
        }

        TEST_F(ReleaseModuleTest, ItRegistersTagItemForEnrouteReleasePoint)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            EXPECT_EQ(1, this->container.tagHandler->HasHandlerForItemId(109));
        }

        TEST_F(ReleaseModuleTest, ItRegistersAllTagItems)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            EXPECT_EQ(2, this->container.tagHandler->CountHandlers());
        }
    }  // namespace Releases
}  // namespace UKControllerPluginTest
