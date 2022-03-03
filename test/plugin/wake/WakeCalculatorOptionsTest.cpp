#include "wake/WakeCalculatorOptions.h"

using UKControllerPlugin::Wake::WakeCalculatorOptions;

namespace UKControllerPluginTest::Wake {
    class WakeCalculatorOptionsTest : public testing::Test
    {
        public:
        WakeCalculatorOptions options;
    };

    TEST_F(WakeCalculatorOptionsTest, LeadAircraftStartsBlank)
    {
        EXPECT_TRUE(options.LeadAircraft().empty());
    }

    TEST_F(WakeCalculatorOptionsTest, FollowingAircraftStartsBlank)
    {
        EXPECT_TRUE(options.FollowingAircraft().empty());
    }

    TEST_F(WakeCalculatorOptionsTest, SchemeStartsEmpty)
    {
        EXPECT_TRUE(options.Scheme().empty());
    }

    TEST_F(WakeCalculatorOptionsTest, SchemeMapperStartsEmpty)
    {
        EXPECT_EQ(nullptr, options.SchemeMapper());
    }

    TEST_F(WakeCalculatorOptionsTest, IntemediateStartsFalse)
    {
        EXPECT_FALSE(options.Intermediate());
    }

    TEST_F(WakeCalculatorOptionsTest, ItSetsLeadAircraft)
    {
        options.LeadAircraft("BAW123");
        EXPECT_EQ("BAW123", options.LeadAircraft());
    }

    TEST_F(WakeCalculatorOptionsTest, ItSetsFollowingAircraft)
    {
        options.FollowingAircraft("BAW123");
        EXPECT_EQ("BAW123", options.FollowingAircraft());
    }

    TEST_F(WakeCalculatorOptionsTest, ItSetsScheme)
    {
        options.Scheme("ABC");
        EXPECT_EQ("ABC", options.Scheme());
    }

    TEST_F(WakeCalculatorOptionsTest, ItSetsIntermediate)
    {
        options.Intermediate(true);
        EXPECT_TRUE(options.Intermediate());
    }

    TEST_F(WakeCalculatorOptionsTest, ItSetsMapper)
    {
        auto mapper = std::make_shared<MockWakeCategoryMapper>();
        options.SchemeMapper(mapper);
        EXPECT_EQ(mapper, options.SchemeMapper());
    }

    TEST_F(WakeCalculatorOptionsTest, ArrivalsStartsFalse)
    {
        EXPECT_FALSE(options.Arrivals());
    }

    TEST_F(WakeCalculatorOptionsTest, DeparturesStartsTrue)
    {
        EXPECT_TRUE(options.Departures());
    }

    TEST_F(WakeCalculatorOptionsTest, ArrivalsCanBeToggled)
    {
        options.ToggleArrivals();
        EXPECT_TRUE(options.Arrivals());
        EXPECT_FALSE(options.Departures());
    }
} // namespace UKControllerPluginTest::Wake
