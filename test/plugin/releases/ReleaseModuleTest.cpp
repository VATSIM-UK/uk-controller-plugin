#include "controller/ActiveCallsignCollection.h"
#include "releases/ReleaseModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "releases/DepartureReleaseRequestedEvent.h"
#include "releases/DepartureReleaseRequestedEvent.h"
#include "releases/SendReleaseRequestedChatAreaMessage.h"
#include "test/EventBusTestCase.h"
#include "push/PushEventProcessorCollection.h"
#include "tag/TagItemCollection.h"
#include "timedevent/TimedEventCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "controller/HandoffEventHandlerCollection.h"
#include "dialog/DialogManager.h"
#include "radarscreen/RadarRenderableCollection.h"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::HandoffEventHandlerCollection;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Push::PushEventProcessorCollection;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::Releases::BootstrapPlugin;
using UKControllerPlugin::Releases::BootstrapRadarScreen;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPluginTest::Dependency::MockDependencyLoader;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;

namespace UKControllerPluginTest {
    namespace Releases {

        class ReleaseModuleTest : public UKControllerPluginUtilsTest::EventBusTestCase
        {
            public:
            ReleaseModuleTest()
            {
                container.pushEventProcessors.reset(new PushEventProcessorCollection);
                container.tagHandler.reset(new TagItemCollection);
                container.timedHandler.reset(new TimedEventCollection);
                container.pluginFunctionHandlers.reset(new FunctionCallEventHandler);
                container.controllerHandoffHandlers.reset(new HandoffEventHandlerCollection);
                container.dialogManager.reset(new DialogManager(this->dialogProvider));
                container.activeCallsigns =
                    std::make_shared<UKControllerPlugin::Controller::ActiveCallsignCollection>();

                nlohmann::json dependency = nlohmann::json::array();
                dependency.push_back({{"id", 1}, {"tag_string", "RFC"}, {"description", "Released For Climb"}});
                dependency.push_back({{"id", 2}, {"tag_string", "RFD"}, {"description", "Released For Descent"}});

                ON_CALL(
                    this->dependencyLoader, LoadDependency("DEPENDENCY_ENROUTE_RELEASE_TYPES", nlohmann::json::array()))
                    .WillByDefault(Return(dependency));
            }

            NiceMock<MockEuroscopePluginLoopbackInterface> plugin;
            NiceMock<MockDialogProvider> dialogProvider;
            NiceMock<MockDependencyLoader> dependencyLoader;
            PersistenceContainer container;
            RadarRenderableCollection renderables;
        };

        TEST_F(ReleaseModuleTest, ItRegistersForEnroutePushEvents)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_EQ(1, this->container.pushEventProcessors->CountProcessorsForChannel("private-enroute-releases"));
        }

        TEST_F(ReleaseModuleTest, ItRegistersTagItemForEnrouteReleaseType)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_EQ(1, this->container.tagHandler->HasHandlerForItemId(108));
        }

        TEST_F(ReleaseModuleTest, ItRegistersTagItemForEnrouteReleasePoint)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_EQ(1, this->container.tagHandler->HasHandlerForItemId(109));
        }

        TEST_F(ReleaseModuleTest, ItRegistersTagItemForEnrouteReleasePointOrBlank)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_EQ(1, this->container.tagHandler->HasHandlerForItemId(111));
        }

        TEST_F(ReleaseModuleTest, ItRegistersAllTagItems)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_EQ(6, this->container.tagHandler->CountHandlers());
        }

        TEST_F(ReleaseModuleTest, ItRegistersForTimedEvents)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_EQ(2, this->container.timedHandler->CountHandlers());
            EXPECT_EQ(1, this->container.timedHandler->CountHandlersForFrequency(10));
            EXPECT_EQ(1, this->container.timedHandler->CountHandlersForFrequency(15));
        }

        TEST_F(ReleaseModuleTest, ItRegistersForHandoffEvents)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_EQ(1, this->container.controllerHandoffHandlers->CountHandlers());
        }

        TEST_F(ReleaseModuleTest, ItRegistersReleaseTypeSelectionFunctions)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasTagFunction(9005));
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackByDescription("Release Type Selected"));
        }

        TEST_F(ReleaseModuleTest, ItRegistersReleasePointEditingFunctions)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasTagFunction(9006));
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackByDescription("Release Point Edited"));
        }

        TEST_F(ReleaseModuleTest, ItRegistersDialogs)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_TRUE(this->container.dialogManager->HasDialog(IDD_DEPARTURE_RELEASE_APPROVE));
            EXPECT_TRUE(this->container.dialogManager->HasDialog(IDD_DEPARTURE_RELEASE_REQUEST));
            EXPECT_TRUE(this->container.dialogManager->HasDialog(IDD_DEPARTURE_RELEASE_REJECT));
            EXPECT_EQ(3, this->container.dialogManager->CountDialogs());
        }

        TEST_F(ReleaseModuleTest, ItRegistersOpenRequestDialogTagFunction)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasTagFunction(9012));
        }

        TEST_F(ReleaseModuleTest, ItRegistersOpenDecisionMenuTagFunction)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasTagFunction(9013));
            EXPECT_TRUE(
                this->container.pluginFunctionHandlers->HasCallbackByDescription("Departure Release Decision Made"));
        }

        TEST_F(ReleaseModuleTest, ItRegistersForDeparturePushEvents)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_EQ(1, this->container.pushEventProcessors->CountProcessorsForChannel("private-departure-releases"));
        }

        TEST_F(ReleaseModuleTest, ItRegistersTagItemForDepartureReleaseStatusIndicator)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_EQ(1, this->container.tagHandler->HasHandlerForItemId(124));
        }

        TEST_F(ReleaseModuleTest, ItRegistersTagItemForDepartureReleaseCountdownTimer)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_EQ(1, this->container.tagHandler->HasHandlerForItemId(125));
        }

        TEST_F(ReleaseModuleTest, ItRegistersTheStatusViewTagFunction)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasTagFunction(9014));
        }

        TEST_F(ReleaseModuleTest, RadarScreenAddsRenderable)
        {
            BootstrapRadarScreen(this->container, renderables);
            EXPECT_EQ(1, renderables.CountRenderers());
        }

        TEST_F(ReleaseModuleTest, ItRegistersTheCancelRequestFunctions)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasTagFunction(9015));
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackByDescription(
                "Departure Release Request Cancelled"));
        }

        TEST_F(ReleaseModuleTest, ItRegistersTagItemForDepartureReleaseRequestingController)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_EQ(1, this->container.tagHandler->HasHandlerForItemId(126));
        }

        TEST_F(ReleaseModuleTest, ItRegistersReleaseRequestedChatAreaHandler)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            AssertSingleEventHandlerRegistrationForEvent<
                UKControllerPlugin::Releases::DepartureReleaseRequestedEvent>();
            AssertHandlerRegisteredForEvent<
                UKControllerPlugin::Releases::SendReleaseRequestedChatAreaMessage,
                UKControllerPlugin::Releases::DepartureReleaseRequestedEvent>(
                UKControllerPluginUtils::EventHandler::EventHandlerFlags::Sync);
        }
    } // namespace Releases
} // namespace UKControllerPluginTest
