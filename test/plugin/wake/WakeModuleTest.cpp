#include "aircraft/CallsignSelectionListFactory.h"
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "list/PopupListFactory.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "radarscreen/MenuToggleableDisplayFactory.h"
#include "plugin/FunctionCallEventHandler.h"
#include "tag/TagItemCollection.h"
#include "wake/WakeCategoryMapperCollection.h"
#include "wake/WakeModule.h"
#include "wake/WakeSchemeCollection.h"

using ::testing::NiceMock;
using ::testing::Test;
using UKControllerPlugin::Aircraft::CallsignSelectionListFactory;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::List::PopupListFactory;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::RadarScreen::MenuToggleableDisplayFactory;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPlugin::Wake::BootstrapPlugin;
using UKControllerPlugin::Wake::BootstrapRadarScreen;
using UKControllerPluginTest::Dependency::MockDependencyLoader;

namespace UKControllerPluginTest::Wake {

    class WakeModuleTest : public Test
    {
        public:
        WakeModuleTest() : toggleableFactory(functionCalls, configurableDisplays)
        {
            container.flightplanHandler = std::make_unique<FlightPlanEventHandlerCollection>();
            container.tagHandler = std::make_unique<TagItemCollection>();
            container.popupListFactory = std::make_unique<PopupListFactory>(functionCalls, mockPlugin);
            container.callsignSelectionListFactory =
                std::make_unique<CallsignSelectionListFactory>(*container.popupListFactory);
        }

        ConfigurableDisplayCollection configurableDisplays;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> mockPlugin;
        AsrEventHandlerCollection asrEventHandlers;
        RadarRenderableCollection radarRenderables;
        FunctionCallEventHandler functionCalls;
        MenuToggleableDisplayFactory toggleableFactory;
        PersistenceContainer container;
        NiceMock<MockDependencyLoader> dependencies;
    };

    TEST_F(WakeModuleTest, ItRegistersTheSchemeCollection)
    {
        EXPECT_CALL(dependencies, LoadDependency("DEPENDENCY_WAKE_SCHEME", nlohmann::json::array()))
            .Times(1)
            .WillOnce(testing::Return(nlohmann::json::array()));

        BootstrapPlugin(this->container, this->dependencies);

        EXPECT_EQ(0, this->container.wakeSchemes->Count());
    }

    TEST_F(WakeModuleTest, ItRegistersTheCategoryMapperCollection)
    {
        EXPECT_CALL(dependencies, LoadDependency("DEPENDENCY_WAKE_SCHEME", nlohmann::json::array()))
            .Times(1)
            .WillOnce(testing::Return(nlohmann::json::array()));

        BootstrapPlugin(this->container, this->dependencies);

        EXPECT_EQ(0, this->container.wakeCategoryMappers->Count());
    }

    TEST_F(WakeModuleTest, ItAddsToFlightplanHandler)
    {
        BootstrapPlugin(this->container, this->dependencies);
        EXPECT_EQ(1, this->container.flightplanHandler->CountHandlers());
    }

    TEST_F(WakeModuleTest, ItAddsToTagHandler)
    {
        BootstrapPlugin(this->container, this->dependencies);
        EXPECT_EQ(5, this->container.tagHandler->CountHandlers());
    }

    TEST_F(WakeModuleTest, ItAddsToTagHandlerWithCorrectIdAircraftTypeUkCategory)
    {
        BootstrapPlugin(this->container, this->dependencies);
        EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(105));
    }

    TEST_F(WakeModuleTest, ItAddsToTagHandlerWithCorrectIdStandalone)
    {
        BootstrapPlugin(this->container, this->dependencies);
        EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(112));
    }

    TEST_F(WakeModuleTest, ItAddsToTagHandlerWithCorrectIdRecat)
    {
        BootstrapPlugin(this->container, this->dependencies);
        EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(113));
    }

    TEST_F(WakeModuleTest, ItAddsToTagHandlerWithCorrectIdUKRecatCombined)
    {
        BootstrapPlugin(this->container, this->dependencies);
        EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(114));
    }

    TEST_F(WakeModuleTest, ItAddsToTagHandlerWithCorrectIdAircraftTypeRecat)
    {
        BootstrapPlugin(this->container, this->dependencies);
        EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(115));
    }

    TEST_F(WakeModuleTest, ItRegistersCalculatorForRenderers)
    {
        BootstrapRadarScreen(this->container, radarRenderables, asrEventHandlers, toggleableFactory);
        EXPECT_EQ(1, radarRenderables.CountRenderers());
        EXPECT_EQ(1, radarRenderables.CountRenderersInPhase(radarRenderables.afterLists));
    }

    TEST_F(WakeModuleTest, ItRegistersCalculatorForAsrEvents)
    {
        BootstrapRadarScreen(this->container, radarRenderables, asrEventHandlers, toggleableFactory);
        EXPECT_EQ(1, asrEventHandlers.CountHandlers());
    }

    TEST_F(WakeModuleTest, ItRegistersLeadAircraftCallback)
    {
        BootstrapRadarScreen(this->container, radarRenderables, asrEventHandlers, toggleableFactory);
        EXPECT_TRUE(functionCalls.HasCallbackByDescription("Wake Calculator Lead Aircraft"));
    }

    TEST_F(WakeModuleTest, ItRegistersFollowingAircraftCallback)
    {
        BootstrapRadarScreen(this->container, radarRenderables, asrEventHandlers, toggleableFactory);
        EXPECT_TRUE(functionCalls.HasCallbackByDescription("Wake Calculator Following Aircraft"));
    }

    TEST_F(WakeModuleTest, ItRegistersSchemeCallback)
    {
        BootstrapRadarScreen(this->container, radarRenderables, asrEventHandlers, toggleableFactory);
        EXPECT_TRUE(functionCalls.HasCallbackByDescription("Wake Calculator Scheme"));
    }

    TEST_F(WakeModuleTest, ItRegistersToggleableDisplay)
    {
        BootstrapRadarScreen(this->container, radarRenderables, asrEventHandlers, toggleableFactory);
        EXPECT_TRUE(functionCalls.HasCallbackByDescription("Toggle Wake Turbulence Calculator"));
    }
} // namespace UKControllerPluginTest::Wake
