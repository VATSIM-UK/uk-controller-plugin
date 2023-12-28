#include "approach/GlideslopeDeviationEstimator.h"
#include "runway/Runway.h"

namespace UKControllerPluginTest::Approach {
    class GlideslopeDeviationEstimatorTest : public ::testing::Test
    {
        public:
        GlideslopeDeviationEstimatorTest() : runway(1, 1, "EGKK", "26L", 257, RunwayPosition(), 196, 3.0)
        {
        }

        [[nodiscard]] auto RunwayPosition() -> EuroScopePlugIn::CPosition
        {
            EuroScopePlugIn::CPosition pos;
            pos.m_Latitude = 51.150675;
            pos.m_Longitude = -0.171925;
            return pos;
        }

        UKControllerPlugin::Runway::Runway runway;
        testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface> radarTarget;
        UKControllerPlugin::Approach::GlideslopeDeviationEstimator glideslopeDeviationEstimator;
    };

    TEST_F(GlideslopeDeviationEstimatorTest, CalculateGlideslopeDeviation)
    {
        EuroScopePlugIn::CPosition aircraftPosition;
        // Approx 8DME
        aircraftPosition.m_Latitude = 51.17575;
        aircraftPosition.m_Longitude = 0.00829;
        ON_CALL(radarTarget, GetPosition()).WillByDefault(testing::Return(aircraftPosition));
        ON_CALL(radarTarget, GetAltitude()).WillByDefault(testing::Return(2000));

        const auto result = glideslopeDeviationEstimator.CalculateGlideslopeDeviation(radarTarget, runway);
        EXPECT_DOUBLE_EQ(-448, result.deviation);
        EXPECT_NEAR(0.9515431, result.perpendicularDistanceFromLocaliser, 0.001);
        EXPECT_NEAR(7.0799, result.localiserRange, 0.001);
    }
} // namespace UKControllerPluginTest::Approach
