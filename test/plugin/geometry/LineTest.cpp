#include "geometry/Line.h"

using UKControllerPlugin::Geometry::Line;

namespace UKControllerPluginTest::Geometry {
    class LineTest : public testing::Test
    {
        public:
        LineTest() : line1(Line1()), line2(Line2()), line3(Line3()), line4(Line4())
        {
        }

        [[nodiscard]] static auto Line1() -> Line
        {
            EuroScopePlugIn::CPosition start;
            start.m_Latitude = 2;
            start.m_Longitude = 2;

            EuroScopePlugIn::CPosition end;
            end.m_Latitude = 4;
            end.m_Longitude = 4;

            return {start, end};
        }

        [[nodiscard]] static auto Line2() -> Line
        {
            EuroScopePlugIn::CPosition start;
            start.m_Latitude = 2;
            start.m_Longitude = 4;

            EuroScopePlugIn::CPosition end;
            end.m_Latitude = 4;
            end.m_Longitude = 2;

            return {start, end};
        }

        [[nodiscard]] static auto Line3() -> Line
        {
            EuroScopePlugIn::CPosition start;
            start.m_Latitude = 2;
            start.m_Longitude = 2;

            EuroScopePlugIn::CPosition end;
            end.m_Latitude = 4;
            end.m_Longitude = 4;

            return {start, end};
        }

        [[nodiscard]] static auto Line4() -> Line
        {
            EuroScopePlugIn::CPosition start;
            start.m_Latitude = 3;
            start.m_Longitude = 3;

            EuroScopePlugIn::CPosition end;
            end.m_Latitude = 5;
            end.m_Longitude = 5;

            return {start, end};
        }

        Line line1;
        Line line2;
        Line line3;
        Line line4;
    };

    TEST_F(LineTest, ItHasAStart)
    {
        auto start = line1.Start();
        EXPECT_EQ(2, start.m_Latitude);
        EXPECT_EQ(2, start.m_Longitude);
    }

    TEST_F(LineTest, ItHasAnEnd)
    {
        auto end = line1.End();
        EXPECT_EQ(4, end.m_Latitude);
        EXPECT_EQ(4, end.m_Longitude);
    }

    TEST_F(LineTest, ItIntersects)
    {
        EXPECT_TRUE(line1.IntersectsWith(line2));
    }

    TEST_F(LineTest, ItHasAnIntersection)
    {
        auto intersect = line1.Intersection(line2);
        EXPECT_EQ(3, intersect.m_Latitude);
        EXPECT_EQ(3, intersect.m_Longitude);
    }

    TEST_F(LineTest, ItDoesntHaveAnIntersection)
    {
        EXPECT_FALSE(line3.IntersectsWith(line4));
    }

    TEST_F(LineTest, IntersectionIsZeroWhenNotIntersecting)
    {
        auto intersect = line3.Intersection(line4);
        EXPECT_EQ(0, intersect.m_Latitude);
        EXPECT_EQ(0, intersect.m_Longitude);
    }
} // namespace UKControllerPluginTest::Geometry
