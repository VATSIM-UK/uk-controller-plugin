#include "approach/GlideslopeDriftEstimator.h"
#include "runway/Runway.h"

namespace UKControllerPluginTest::Approach {
    class GlideslopeDriftEstimatorTest : public ::testing::Test
    {
        public:
        GlideslopeDriftEstimatorTest() : runway(1, 1, "26L", 257, RunwayPosition())
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
        UKControllerPlugin::Approach::GlideslopeDriftEstimator glideslopeDriftEstimator;
    };

    TEST_F(GlideslopeDriftEstimatorTest, CalculateGlideslopeDrift)
    {
        EuroScopePlugIn::CPosition aircraftPosition;
        // Approx 8DME
        aircraftPosition.m_Latitude = 51.17575;
        aircraftPosition.m_Longitude = 0.00829;
        ON_CALL(radarTarget, GetPosition()).WillByDefault(testing::Return(aircraftPosition));
        ON_CALL(radarTarget, GetAltitude()).WillByDefault(testing::Return(2000));

        const auto result = glideslopeDriftEstimator.CalculateGlideslopeDrift(radarTarget, runway);
        EXPECT_DOUBLE_EQ(-448, result.drift);
        EXPECT_NEAR(0.9515431, result.perpendicularDistanceFromLocaliser, 0.001);
        EXPECT_NEAR(7.0799, result.localiserRange, 0.001);
    }
} // namespace UKControllerPluginTest::Approach
