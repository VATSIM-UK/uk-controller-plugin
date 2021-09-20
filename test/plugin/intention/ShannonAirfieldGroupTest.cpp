#include "intention/ShannonAirfieldGroup.h"

using ::testing::StrictMock;
using UKControllerPlugin::IntentionCode::ShannonAirfieldGroup;
using UKControllerPluginTest::Euroscope::MockEuroscopeExtractedRouteInterface;

namespace UKControllerPluginTest {
    namespace IntentionCode {

        TEST(ShannonAirfieldGroup, ItAppliesToAllControllers)
        {
            ShannonAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_TRUE(airfieldGroup.AppliesToController("BLA", wrapperMock));
        }

        TEST(ShannonAirfieldGroup, HasAirfieldReturnsFalseNotShannon)
        {
            ShannonAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_FALSE(airfieldGroup.HasAirfield("EGGD", wrapperMock));
        }

        TEST(ShannonAirfieldGroup, HasAirfieldReturnsTrueIfAirfieldKnownShannon)
        {
            ShannonAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_TRUE(airfieldGroup.HasAirfield("EINN", wrapperMock));
        }
    } // namespace IntentionCode
} // namespace UKControllerPluginTest
