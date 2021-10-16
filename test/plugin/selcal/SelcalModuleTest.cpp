#include "bootstrap/PersistenceContainer.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "selcal/SelcalModule.h"
#include "tag/TagItemCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Selcal::BootstrapPlugin;
using UKControllerPlugin::Tag::TagItemCollection;

namespace UKControllerPluginTest::Selcal {
    class SelcalModuleTest : public testing::Test
    {
        public:
        SelcalModuleTest()
        {
            container.flightplanHandler = std::make_unique<FlightPlanEventHandlerCollection>();
            container.tagHandler = std::make_unique<TagItemCollection>();
        }
        PersistenceContainer container;
    };

    TEST_F(SelcalModuleTest, ItRegistersSelcalTagItem)
    {
        BootstrapPlugin(container);
        EXPECT_EQ(2, container.tagHandler->CountHandlers());
        EXPECT_TRUE(container.tagHandler->HasHandlerForItemId(128));
    }

    TEST_F(SelcalModuleTest, ItRegistersSelcalWithSeparatorTagItem)
    {
        BootstrapPlugin(container);
        EXPECT_EQ(2, container.tagHandler->CountHandlers());
        EXPECT_TRUE(container.tagHandler->HasHandlerForItemId(129));
    }

    TEST_F(SelcalModuleTest, ItRegistersFlightplanEvents)
    {
        BootstrapPlugin(container);
        EXPECT_EQ(1, container.flightplanHandler->CountHandlers());
    }
} // namespace UKControllerPluginTest::Selcal
