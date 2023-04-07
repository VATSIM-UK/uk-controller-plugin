#include "sectorfile/Runway.h"

using testing::Test;
using UKControllerPlugin::SectorFile::Runway;

namespace UKControllerPluginTest {
    namespace SectorFile {

        class SectorfileRunwayTest : public Test
        {
            public:
            SectorfileRunwayTest() : runway("EGCC", "23L", 123, false, false)
            {
            }

            Runway runway;
        };

        TEST_F(SectorfileRunwayTest, ItSetsBaseProperties)
        {
            EXPECT_EQ("EGCC", this->runway.airfield);
            EXPECT_EQ("23L", this->runway.identifier);
            EXPECT_EQ(123, this->runway.heading);
            EXPECT_FALSE(this->runway.ActiveForDepartures());
            EXPECT_FALSE(this->runway.ActiveForArrivals());
        }

        TEST_F(SectorfileRunwayTest, ItCanBeMadeActiveForDepartures)
        {
            this->runway.SetActiveForDepartures(true);
            EXPECT_TRUE(this->runway.Active());
        }

        TEST_F(SectorfileRunwayTest, IsActiveIfActiveForDepartures)
        {
            this->runway.SetActiveForDepartures(true);
            EXPECT_TRUE(this->runway.Active());
        }

        TEST_F(SectorfileRunwayTest, ItCanBeMadeActiveForArrivals)
        {
            this->runway.SetActiveForArrivals(true);
            EXPECT_TRUE(this->runway.Active());
        }

        TEST_F(SectorfileRunwayTest, IsActiveIfActiveForArrivals)
        {
            this->runway.SetActiveForArrivals(true);
            EXPECT_TRUE(this->runway.Active());
        }
    } // namespace SectorFile
} // namespace UKControllerPluginTest
