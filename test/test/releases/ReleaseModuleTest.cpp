#include "pch/pch.h"
#include "releases/ReleaseModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "mock/MockDependencyLoader.h"
#include "websocket/WebsocketEventProcessorCollection.h"
#include "tag/TagItemCollection.h"
#include "timedevent/TimedEventCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "controller/HandoffEventHandlerCollection.h"
#include "dialog/DialogManager.h"
#include "mock/MockDialogProvider.h"
#include "radarscreen/RadarRenderableCollection.h"

using ::testing::NiceMock;
using ::testing::Test;
using ::testing::Return;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::HandoffEventHandlerCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Releases::BootstrapPlugin;
using UKControllerPlugin::Releases::BootstrapRadarScreen;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPlugin::Websocket::WebsocketEventProcessorCollection;
using UKControllerPluginTest::Dependency::MockDependencyLoader;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;

namespace UKControllerPluginTest {
    namespace Releases {

        class ReleaseModuleTest : public Test
        {
            public:

                ReleaseModuleTest()
                {
                    container.websocketProcessors.reset(new WebsocketEventProcessorCollection);
                    container.tagHandler.reset(new TagItemCollection);
                    container.timedHandler.reset(new TimedEventCollection);
                    container.pluginFunctionHandlers.reset(new FunctionCallEventHandler);
                    container.controllerHandoffHandlers.reset(new HandoffEventHandlerCollection);
                    container.dialogManager.reset(new DialogManager(this->dialogProvider));

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

                NiceMock<MockDialogProvider> dialogProvider;
                NiceMock<MockDependencyLoader> dependencyLoader;
                PersistenceContainer container;
        };

        TEST_F(ReleaseModuleTest, ItRegistersForEnrouteWebsocketEvents)
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

        TEST_F(ReleaseModuleTest, ItRegistersTagItemForEnrouteReleasePointOrBlank)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            EXPECT_EQ(1, this->container.tagHandler->HasHandlerForItemId(111));
        }

        TEST_F(ReleaseModuleTest, ItRegistersAllTagItems)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            EXPECT_EQ(5, this->container.tagHandler->CountHandlers());
        }

        TEST_F(ReleaseModuleTest, ItRegistersForTimedEvents)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            EXPECT_EQ(2, this->container.timedHandler->CountHandlers());
            EXPECT_EQ(1, this->container.timedHandler->CountHandlersForFrequency(10));
            EXPECT_EQ(1, this->container.timedHandler->CountHandlersForFrequency(15));
        }

        TEST_F(ReleaseModuleTest, ItRegistersForHandoffEvents)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            EXPECT_EQ(1, this->container.controllerHandoffHandlers->CountHandlers());
        }

        TEST_F(ReleaseModuleTest, ItRegistersReleaseTypeSelectionFunctions)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasTagFunction(9005));
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackFunction(5000));
        }

        TEST_F(ReleaseModuleTest, ItRegistersReleasePointEditingFunctions)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasTagFunction(9006));
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackFunction(5001));
        }

        TEST_F(ReleaseModuleTest, ItRegistersDialogs)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            EXPECT_TRUE(this->container.dialogManager->HasDialog(IDD_DEPARTURE_RELEASE_APPROVE));
            EXPECT_TRUE(this->container.dialogManager->HasDialog(IDD_DEPARTURE_RELEASE_REQUEST));
            EXPECT_EQ(2, this->container.dialogManager->CountDialogs());
        }

        TEST_F(ReleaseModuleTest, ItRegistersOpenRequestDialogTagFunction)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasTagFunction(9012));
        }

        TEST_F(ReleaseModuleTest, ItRegistersOpenDecisionMenuTagFunction)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasTagFunction(9013));
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackFunction(5002));
        }

        TEST_F(ReleaseModuleTest, ItRegistersForDepartureWebsocketEvents)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            EXPECT_EQ(1, this->container.websocketProcessors->CountProcessorsForChannel("private-departure-releases"));
        }

        TEST_F(ReleaseModuleTest, ItRegistersTagItemForDepartureReleaseStatusIndicator)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            EXPECT_EQ(1, this->container.tagHandler->HasHandlerForItemId(124));
        }

        TEST_F(ReleaseModuleTest, ItRegistersTagItemForDepartureReleaseCountdownTimer)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            EXPECT_EQ(1, this->container.tagHandler->HasHandlerForItemId(125));
        }

        TEST_F(ReleaseModuleTest, ItRegistersTheStatusViewTagFunction)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasTagFunction(9014));
        }

        TEST_F(ReleaseModuleTest, RadarScreenAddsRenderable)
        {
            RadarRenderableCollection renderables;
            BootstrapRadarScreen(this->container, renderables);
            EXPECT_EQ(1, renderables.CountRenderers());
        }

        TEST_F(ReleaseModuleTest, ItRegistersTheCancelRequestFunctions)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasTagFunction(9015));
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackFunction(5003));
        }
    }  // namespace Releases
}  // namespace UKControllerPluginTest
