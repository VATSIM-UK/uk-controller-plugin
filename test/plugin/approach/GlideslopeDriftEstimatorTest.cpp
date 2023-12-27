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
        ASSERT_DOUBLE_EQ(result, 448);
    }
} // namespace UKControllerPluginTest::Approach
