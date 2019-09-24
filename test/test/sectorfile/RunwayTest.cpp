#include "pch/pch.h"
#include "sectorfile/Runway.h"

using UKControllerPlugin::SectorFile::Runway;
using testing::Test;

namespace UKControllerPluginTest {
    namespace SectorFile {

        class RunwayTest : public Test
        {
            public:

                RunwayTest()
                    : runway ("EGCC", "23L", 123, false, false)
                {

                }

            Runway runway;
        };

        TEST_F(RunwayTest, ItSetsBaseProperties)
        {
            EXPECT_EQ("EGCC", this->runway.airfield);
            EXPECT_EQ("23L", this->runway.identifier);
            EXPECT_EQ(123, this->runway.heading);
            EXPECT_FALSE(this->runway.ActiveForDepartures());
            EXPECT_FALSE(this->runway.ActiveForArrivals());
        }

        TEST_F(RunwayTest, ItCanBeMadeActiveForDepartures)
        {
            this->runway.SetActiveForDepartures(true);
            EXPECT_TRUE(this->runway.Active());
        }

        TEST_F(RunwayTest, IsActiveIfActiveForDepartures)
        {
            this->runway.SetActiveForDepartures(true);
            EXPECT_TRUE(this->runway.Active());
        }

        TEST_F(RunwayTest, ItCanBeMadeActiveForArrivals)
        {
            this->runway.SetActiveForArrivals(true);
            EXPECT_TRUE(this->runway.Active());
        }

        TEST_F(RunwayTest, IsActiveIfActiveForArrivals)
        {
            this->runway.SetActiveForArrivals(true);
            EXPECT_TRUE(this->runway.Active());
        }
    }  // namespace Airfield
}  // namespace UKControllerPluginTest