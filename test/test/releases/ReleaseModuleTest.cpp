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
#include "mock/MockEuroscopeFlightplanList.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "radarscreen/ConfigurableDisplayCollection.h"

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
using UKControllerPluginTest::Euroscope::MockEuroscopeFlightplanList;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;

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


                    this->mockFlightplanList = std::make_shared<NiceMock<MockEuroscopeFlightplanList>>();
                    ON_CALL(*this->mockFlightplanList, NumberOfColumns)
                        .WillByDefault(Return(3));

                    ON_CALL(this->plugin, RegisterFlightplanList("Departure Release Requests"))
                        .WillByDefault(Return(this->mockFlightplanList));
                }

                std::shared_ptr<NiceMock<MockEuroscopeFlightplanList>> mockFlightplanList;
                NiceMock<MockEuroscopePluginLoopbackInterface> plugin;
                NiceMock<MockDialogProvider> dialogProvider;
                NiceMock<MockDependencyLoader> dependencyLoader;
                PersistenceContainer container;
                RadarRenderableCollection renderables;
                ConfigurableDisplayCollection configurables;
        };

        TEST_F(ReleaseModuleTest, ItRegistersForEnrouteWebsocketEvents)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_EQ(1, this->container.websocketProcessors->CountProcessorsForChannel("private-enroute-releases"));
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
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackFunction(5000));
        }

        TEST_F(ReleaseModuleTest, ItRegistersReleasePointEditingFunctions)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasTagFunction(9006));
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackFunction(5001));
        }

        TEST_F(ReleaseModuleTest, ItRegistersDialogs)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_TRUE(this->container.dialogManager->HasDialog(IDD_DEPARTURE_RELEASE_APPROVE));
            EXPECT_TRUE(this->container.dialogManager->HasDialog(IDD_DEPARTURE_RELEASE_REQUEST));
            EXPECT_EQ(2, this->container.dialogManager->CountDialogs());
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
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackFunction(5002));
        }

        TEST_F(ReleaseModuleTest, ItRegistersForDepartureWebsocketEvents)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_EQ(1, this->container.websocketProcessors->CountProcessorsForChannel("private-departure-releases"));
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
            BootstrapRadarScreen(this->container, renderables, configurables);
            EXPECT_EQ(1, renderables.CountRenderers());
        }

        TEST_F(ReleaseModuleTest, RadarScreenAddsReleaseRequestShowOption)
        {
            BootstrapRadarScreen(this->container, renderables, configurables);
            EXPECT_EQ(1, configurables.CountDisplays());
            EXPECT_EQ(1, this->container.pluginFunctionHandlers->CountCallbacks());
        }

        TEST_F(ReleaseModuleTest, ItRegistersTheCancelRequestFunctions)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasTagFunction(9015));
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackFunction(5003));
        }

        TEST_F(ReleaseModuleTest, ItRegistersTagItemForDepartureReleaseRequestingController)
        {
            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
            EXPECT_EQ(1, this->container.tagHandler->HasHandlerForItemId(126));
        }

        TEST_F(ReleaseModuleTest, ItAddsColumnsToTheListIfThereAreNone)
        {
            ON_CALL(*this->mockFlightplanList, NumberOfColumns())
                .WillByDefault(Return(0));

            EXPECT_CALL(
                *this->mockFlightplanList,
                AddColumn(
                    "C/S",
                    7,
                    false,
                    MockEuroscopeFlightplanList::EUROSCOPE_PROVIDER,
                    EuroScopePlugIn::TAG_ITEM_TYPE_CALLSIGN,
                    UKControllerPlugin::Euroscope::EuroscopeFlightplanListInterface::UKCP_PROVIDER,
                    9013,
                    UKControllerPlugin::Euroscope::EuroscopeFlightplanListInterface::EUROSCOPE_PROVIDER,
                    EuroScopePlugIn::TAG_ITEM_FUNCTION_NO
                )
            ).Times(1);

            EXPECT_CALL(
                *this->mockFlightplanList,
                AddColumn(
                    "Dept",
                    4,
                    true,
                    MockEuroscopeFlightplanList::EUROSCOPE_PROVIDER,
                    EuroScopePlugIn::TAG_ITEM_TYPE_ORIGIN,
                    UKControllerPlugin::Euroscope::EuroscopeFlightplanListInterface::EUROSCOPE_PROVIDER,
                    EuroScopePlugIn::TAG_ITEM_FUNCTION_NO,
                    UKControllerPlugin::Euroscope::EuroscopeFlightplanListInterface::EUROSCOPE_PROVIDER,
                    EuroScopePlugIn::TAG_ITEM_FUNCTION_NO
                )
            ).Times(1);

            EXPECT_CALL(
                *this->mockFlightplanList,
                AddColumn(
                    "SID",
                    7,
                    false,
                    MockEuroscopeFlightplanList::EUROSCOPE_PROVIDER,
                    EuroScopePlugIn::TAG_ITEM_TYPE_ASSIGNED_SID,
                    UKControllerPlugin::Euroscope::EuroscopeFlightplanListInterface::EUROSCOPE_PROVIDER,
                    EuroScopePlugIn::TAG_ITEM_FUNCTION_NO,
                    UKControllerPlugin::Euroscope::EuroscopeFlightplanListInterface::EUROSCOPE_PROVIDER,
                    EuroScopePlugIn::TAG_ITEM_FUNCTION_NO
                )
            ).Times(1);

            EXPECT_CALL(
                *this->mockFlightplanList,
                AddColumn(
                    "Req Ctrl",
                    10,
                    false,
                    MockEuroscopeFlightplanList::UKCP_PROVIDER,
                    126,
                    UKControllerPlugin::Euroscope::EuroscopeFlightplanListInterface::EUROSCOPE_PROVIDER,
                    EuroScopePlugIn::TAG_ITEM_FUNCTION_NO,
                    UKControllerPlugin::Euroscope::EuroscopeFlightplanListInterface::EUROSCOPE_PROVIDER,
                    EuroScopePlugIn::TAG_ITEM_FUNCTION_NO
                )
            ).Times(1);

            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);

        }

        TEST_F(ReleaseModuleTest, ItDoesntAddColumnsIfPredefined)
        {
            EXPECT_CALL(
                *this->mockFlightplanList,
                AddColumn(
                    testing::_,
                    testing::_,
                    testing::_,
                    testing::_,
                    testing::_,
                    testing::_,
                    testing::_,
                    testing::_,
                    testing::_
                )
            ).Times(0);

            BootstrapPlugin(this->container, this->plugin, this->dependencyLoader);
        }
    }  // namespace Releases
}  // namespace UKControllerPluginTest
