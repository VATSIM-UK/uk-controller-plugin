#include "pch/pch.h"
#include "airfield/Runway.h"

using UKControllerPlugin::Airfield::Runway;
using testing::Test;

namespace UKControllerPluginTest {
    namespace Airfield {

        class RunwayTest : public Test
        {
            public:

                RunwayTest()
                    : runway ("sectorident", "EGCC", "23L", 123)
                {

                }

            Runway runway;
        };

        TEST_F(RunwayTest, ItSetsBaseProperties)
        {
            EXPECT_EQ("sectorident", this->runway.sectorfileIdentifier);
            EXPECT_EQ("EGCC", this->runway.airfield);
            EXPECT_EQ("23L", this->runway.identifier);
            EXPECT_EQ(123, this->runway.heading);
        }

        TEST_F(RunwayTest, ItStartsInactive)
        {
            EXPECT_FALSE(this->runway.Active());
        }

        TEST_F(RunwayTest, ItCanBeMadeActiveForDepartures)
        {
            this->runway.SetActiveForDepartures(true);
            EXPECT_TRUE(this->runway.ActiveForDepartures());
            EXPECT_TRUE(this->runway.Active());
        }

        TEST_F(RunwayTest, ItCanBeMadeActiveForArrivals)
        {
            this->runway.SetActiveForArrivals(true);
            EXPECT_TRUE(this->runway.ActiveForArrivals());
            EXPECT_TRUE(this->runway.Active());
        }
    }  // namespace Airfield
}  // namespace UKControllerPluginTest