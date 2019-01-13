#include "pch/pch.h"
#include "offblock/ActualOffBlockTimeBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "tag/TagItemCollection.h"

using UKControllerPlugin::Datablock::ActualOffBlockTimeBootstrap;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Tag::TagItemCollection;
using ::testing::Test;

namespace UKControllerPlugin {
    namespace Datablock {

        class ActualOffBlockTimeBootstrapTest : public Test
        {
            public:

                void SetUp()
                {
                    container.tagHandler.reset(new TagItemCollection);
                    container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
                };

                PersistenceContainer container;
        };

        TEST_F(ActualOffBlockTimeBootstrapTest, AddsHandlerToFlightplanHandlers)
        {
            ActualOffBlockTimeBootstrap::BootstrapPlugin(this->container);
            EXPECT_EQ(1, container.flightplanHandler->CountHandlers());
        }

        TEST_F(ActualOffBlockTimeBootstrapTest, AddsHandlerToTagItemHandlers)
        {
            ActualOffBlockTimeBootstrap::BootstrapPlugin(this->container);
            EXPECT_EQ(1, container.tagHandler->CountHandlers());
        }
    }  // namespace Datablock
}  // namespace UKControllerPlugin
