#include "headings/Heading.h"

using UKControllerPlugin::Headings::Heading;

namespace UKControllerPluginTest::Headings {
    class HeadingTest : public testing::Test
    {
    };
    
    TEST_F(HeadingTest, ItKnowsNorthExplicit)
    {
        EXPECT_EQ(360, Heading::North);
    }
    
    TEST_F(HeadingTest, ItKnowsNorthShort)
    {
        EXPECT_EQ(360, Heading::N);
    }
    
    TEST_F(HeadingTest, ItKnowsNorthEastExplicit)
    {
        EXPECT_EQ(45, Heading::NorthEast);
    }
    
    TEST_F(HeadingTest, ItKnowsNorthEastShort)
    {
        EXPECT_EQ(45, Heading::NE);
    }
    
    TEST_F(HeadingTest, ItKnowsEastExplicit)
    {
        EXPECT_EQ(90, Heading::East);
    }
    
    TEST_F(HeadingTest, ItKnowsEastShort)
    {
        EXPECT_EQ(90, Heading::E);
    }
    
    TEST_F(HeadingTest, ItKnowsSouthEastExplicit)
    {
        EXPECT_EQ(135, Heading::SouthEast);
    }
    
    TEST_F(HeadingTest, ItKnowsSouthEastShort)
    {
        EXPECT_EQ(135, Heading::SE);
    }
    
    TEST_F(HeadingTest, ItKnowsSouthExplicit)
    {
        EXPECT_EQ(180, Heading::South);
    }
    
    TEST_F(HeadingTest, ItKnowsSouthShort)
    {
        EXPECT_EQ(180, Heading::S);
    }
    
    TEST_F(HeadingTest, ItKnowsSouthWestExplicit)
    {
        EXPECT_EQ(225, Heading::SouthWest);
    }
    
    TEST_F(HeadingTest, ItKnowsSouthWestShort)
    {
        EXPECT_EQ(225, Heading::SW);
    }
    
    TEST_F(HeadingTest, ItKnowsWestExplicit)
    {
        EXPECT_EQ(270, Heading::West);
    }
    
    TEST_F(HeadingTest, ItKnowsWestShort)
    {
        EXPECT_EQ(270, Heading::W);
    }
    
    TEST_F(HeadingTest, ItKnowsNorthWestExplicit)
    {
        EXPECT_EQ(315, Heading::NorthWest);
    }
    
    TEST_F(HeadingTest, ItKnowsNorthWestShort)
    {
        EXPECT_EQ(315, Heading::NW);
    }
    
    TEST_F(HeadingTest, LessThanReturnsTrueForHeadingLessThanHeading)
    {
        EXPECT_TRUE(Heading::SE < Heading::NW);
    }
    
    TEST_F(HeadingTest, LessThanReturnsFalseForHeadingNotLessThanHeading)
    {
        EXPECT_FALSE(Heading::NW < Heading::SW);
    }
    
    TEST_F(HeadingTest, GreaterThanEqualToReturnsTrueForHeadingGreaterThanHeading)
    {
        EXPECT_TRUE(Heading::SE >= Heading::E);
    }
    
    TEST_F(HeadingTest, GreaterThanEqualToReturnsTrueForHeadingEqualToHeading)
    {
        EXPECT_TRUE(Heading::E >= Heading::E);
    }
    
    TEST_F(HeadingTest, GreaterThanEqualToReturnsFalseForHeadingLessThanHeading)
    {
        EXPECT_FALSE(Heading::SW >= Heading::W);
    }
    
    TEST_F(HeadingTest, LessThanReturnsTrueForDoubleLessThanHeading)
    {
        EXPECT_TRUE(180.0 < Heading::NW);
    }
    
    TEST_F(HeadingTest, LessThanReturnsFalseForDoubleNotLessThanHeading)
    {
        EXPECT_FALSE(315.0 < Heading::SW);
    }
    
    TEST_F(HeadingTest, GreaterThanEqualToReturnsTrueForDoubleGreaterThanHeading)
    {
        EXPECT_TRUE(180.0 >= Heading::E);
    }
    
    TEST_F(HeadingTest, GreaterThanEqualToReturnsTrueForDoubleEqualToHeading)
    {
        EXPECT_TRUE(90.0 >= Heading::E);
    }

    TEST_F(HeadingTest, GreaterThanEqualToReturnsFalseForDoubleLessThanHeading)
    {
        EXPECT_FALSE(225.0 >= Heading::W);
    }
} // namespace UKControllerPluginTest::Headings
