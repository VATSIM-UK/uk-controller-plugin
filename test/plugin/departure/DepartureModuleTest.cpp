#include "departure/DepartureModule.h"
#include "departure/AircraftDepartedEvent.h"
#include "departure/UserShouldClearDepartureDataEvent.h"
#include "departure/UserShouldClearDepartureDataMonitor.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "mock/MockDepartureHandoffResolver.h"
#include "timedevent/TimedEventCollection.h"
#include "bootstrap/PersistenceContainer.h"
#include "push/PushEventProcessorCollection.h"
#include "tag/TagItemCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "controller/HandoffEventHandlerCollection.h"
#include "dialog/DialogManager.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "ownership/AirfieldServiceProviderCollection.h"
#include "test/EventBusTestCase.h"
#include "eventhandler/EventBus.h"
#include "eventhandler/EventStream.h"
#include <gmock/gmock-nice-strict.h>

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::HandoffEventHandlerCollection;
using UKControllerPlugin::Departure::BootstrapPlugin;
using UKControllerPlugin::Departure::BootstrapRadarScreen;
using UKControllerPlugin::Departure::UserShouldClearDepartureDataEvent;
using UKControllerPlugin::Departure::UserShouldClearDepartureDataMonitor;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Push::PushEventProcessorCollection;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPluginTest::Dependency::MockDependencyLoader;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginUtils::EventHandler::EventBus;
using UKControllerPluginUtils::EventHandler::EventStream;

namespace UKControllerPluginTest::Departure {

    class DepartureModuleTest : public UKControllerPluginUtilsTest::EventBusTestCase
    {
        public:
        DepartureModuleTest()
        {
            container.tagHandler = std::make_unique<TagItemCollection>();
            container.pluginFunctionHandlers = std::make_unique<FunctionCallEventHandler>();
            container.timedHandler = std::make_unique<TimedEventCollection>();
            container.flightplanHandler =
                std::make_unique<UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection>();
            container.departureHandoffResolver =
                std::make_shared<testing::NiceMock<Handoff::MockDepartureHandoffResolver>>();
            container.airfieldOwnership =
                std::make_shared<UKControllerPlugin::Ownership::AirfieldServiceProviderCollection>();
        }

        PersistenceContainer container;
        RadarRenderableCollection renderables;
        ConfigurableDisplayCollection configurables;
        UKControllerPlugin::Euroscope::AsrEventHandlerCollection asr;
    };

    TEST_F(DepartureModuleTest, PluginRegistersDepartureMonitorForFlightplanEvents)
    {
        BootstrapPlugin(this->container);
        EXPECT_EQ(1, this->container.flightplanHandler->CountHandlers());
    }

    TEST_F(DepartureModuleTest, PluginRegistersShouldClearDepartureDataMonitorForDepartedEvents)
    {
        BootstrapPlugin(this->container);
        AssertSingleEventHandlerRegistrationForEvent<UKControllerPlugin::Departure::AircraftDepartedEvent>();
        AssertHandlerRegisteredForEvent<
            UserShouldClearDepartureDataMonitor,
            UKControllerPlugin::Departure::AircraftDepartedEvent>(
            UKControllerPluginUtils::EventHandler::EventHandlerFlags::Sync);
    }

    TEST_F(DepartureModuleTest, PluginRegistersDepartureMonitorForTimedEvents)
    {
        BootstrapPlugin(this->container);
        EXPECT_EQ(1, this->container.timedHandler->CountHandlers());
        EXPECT_EQ(1, this->container.timedHandler->CountHandlersForFrequency(10));
    }

    TEST_F(DepartureModuleTest, RadarScreenAddsRenderable)
    {
        BootstrapRadarScreen(this->container, renderables, configurables, asr);
        EXPECT_EQ(1, renderables.CountRenderers());
    }

    TEST_F(DepartureModuleTest, RadarScreenAddsToAsr)
    {
        BootstrapRadarScreen(this->container, renderables, configurables, asr);
        EXPECT_EQ(1, asr.CountHandlers());
    }

    TEST_F(DepartureModuleTest, RadarScreenAddsReleaseRequestShowOption)
    {
        BootstrapRadarScreen(this->container, renderables, configurables, asr);
        EXPECT_EQ(1, configurables.CountDisplays());
        EXPECT_EQ(1, this->container.pluginFunctionHandlers->CountCallbacks());
        EXPECT_TRUE(
            this->container.pluginFunctionHandlers->HasCallbackByDescription("Toggle Departure Coordination List"));
    }
} // namespace UKControllerPluginTest::Departure
