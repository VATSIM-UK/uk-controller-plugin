#include "pch/pch.h"
#include "mock/MockEuroscopeExtractedRouteInterface.h"
#include "intention/DublinAirfieldGroup.h"

using UKControllerPlugin::IntentionCode::DublinAirfieldGroup;
using UKControllerPluginTest::Euroscope::MockEuroscopeExtractedRouteInterface;
using ::testing::StrictMock;

namespace UKControllerPluginTest {
    namespace IntentionCode {
        TEST(DublinAirfieldGroup, HasAirfieldReturnsFalseNotInDublin)
        {
            DublinAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_FALSE(airfieldGroup.HasAirfield("EGGD", wrapperMock));
        }

        TEST(DublinAirfieldGroup, HasAirfieldReturnsFalseNotKnownInDublin)
        {
            DublinAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_FALSE(airfieldGroup.HasAirfield("EIXX", wrapperMock));
        }

        TEST(DublinAirfieldGroup, HasAirfieldReturnsTrueIfKnownInDublin)
        {
            DublinAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_TRUE(airfieldGroup.HasAirfield("EIDW", wrapperMock));
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPluginTest
