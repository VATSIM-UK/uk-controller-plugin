#include "geometry/DistanceRadiusToScreenRadius.h"

using UKControllerPlugin::Geometry::ScreenRadiusFromDistance;

namespace UKControllerPluginTest::Geometry {
    class DistanceRadiusToScreenRadiusTest : public testing::Test
    {
        public:
        DistanceRadiusToScreenRadiusTest()
        {
            ON_CALL(radarScreen, GetRadarViewport).WillByDefault(testing::Return(RECT{0, 0, 500, 500}));
            EuroScopePlugIn::CPosition topPosition;
            topPosition.m_Latitude = 50;
            topPosition.m_Longitude = 50;

            EuroScopePlugIn::CPosition bottomPosition;
            topPosition.m_Latitude = 60;
            topPosition.m_Longitude = 60;

            EXPECT_CALL(radarScreen, ConvertScreenPointToCoordinate(PointEq(POINT{0, 0})))
                .Times(1)
                .WillOnce(testing::Return(topPosition));

            EXPECT_CALL(radarScreen, ConvertScreenPointToCoordinate(PointEq(POINT{0, 500})))
                .Times(1)
                .WillOnce(testing::Return(bottomPosition));
        }

        testing::NiceMock<Euroscope::MockEuroscopeRadarScreenLoopbackInterface> radarScreen;
    };

    TEST_F(DistanceRadiusToScreenRadiusTest, ItConvertsADistanceToRadiusOnScreen)
    {
        EXPECT_FLOAT_EQ(6.0683856, ScreenRadiusFromDistance(55, radarScreen));
    }
} // namespace UKControllerPluginTest::Geometry
