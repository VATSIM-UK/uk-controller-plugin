#include "offblock/EstimatedOffBlockTimeBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "tag/TagItemCollection.h"

using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Datablock::EstimatedOffBlockTimeBootstrap;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Tag::TagItemCollection;

namespace UKControllerPluginTest {
    namespace Datablock {

        class EstimatedOffBlockTimeBootstrapTest : public Test
        {
            public:
            void SetUp()
            {
                container.tagHandler.reset(new TagItemCollection);
                container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
            };

            PersistenceContainer container;
        };

        TEST_F(EstimatedOffBlockTimeBootstrapTest, AddsHandlerToTagItemHandlers)
        {
            EstimatedOffBlockTimeBootstrap::BootstrapPlugin(this->container);
            EXPECT_EQ(1, container.tagHandler->CountHandlers());
        }

        TEST_F(EstimatedOffBlockTimeBootstrapTest, AddsHandlerWithCorrectId)
        {
            EstimatedOffBlockTimeBootstrap::BootstrapPlugin(this->container);
            EXPECT_TRUE(container.tagHandler->HasHandlerForItemId(EstimatedOffBlockTimeBootstrap::tagItemId));
        }
    } // namespace Datablock
} // namespace UKControllerPluginTest
