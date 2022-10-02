#include "offblock/EstimatedDepartureTimeBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "tag/TagItemCollection.h"

using UKControllerPlugin::Datablock::EstimatedDepartureTimeBootstrap;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Tag::TagItemCollection;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Datablock {

        class EstimatedDepartureTimeBootstrapTest : public Test
        {
            public:

                void SetUp()
                {
                    container.tagHandler.reset(new TagItemCollection);
                    container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
                };

                PersistenceContainer container;
        };

        TEST_F(EstimatedDepartureTimeBootstrapTest, AddsHandlerToTagItemHandlers)
        {
            EstimatedDepartureTimeBootstrap::BootstrapPlugin(this->container);
            EXPECT_EQ(1, container.tagHandler->CountHandlers());
        }

        TEST_F(EstimatedDepartureTimeBootstrapTest, AddsHandlerWithCorrectId)
        {
            EstimatedDepartureTimeBootstrap::BootstrapPlugin(this->container);
            EXPECT_TRUE(container.tagHandler->HasHandlerForItemId(EstimatedDepartureTimeBootstrap::tagItemId));
        }

        TEST_F(EstimatedDepartureTimeBootstrapTest, AddsHandlerToFlightplanHandlers)
        {
            EstimatedDepartureTimeBootstrap::BootstrapPlugin(this->container);
            EXPECT_EQ(1, container.flightplanHandler->CountHandlers());
        }
    }  // namespace Datablock
} // namespace UKControllerPluginTest
