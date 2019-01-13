#include "pch/pch.h"
#include "mock/MockEuroscopeExtractedRouteInterface.h"
#include "intention/ShannonAirfieldGroup.h"

using UKControllerPlugin::IntentionCode::ShannonAirfieldGroup;
using UKControllerPluginTest::Euroscope::MockEuroscopeExtractedRouteInterface;
using ::testing::StrictMock;

namespace UKControllerPluginTest {
    namespace IntentionCode {
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
    }  // namespace IntentionCode
}  // namespace UKControllerPluginTest
