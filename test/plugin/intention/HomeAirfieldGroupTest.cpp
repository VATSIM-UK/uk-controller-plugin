#include "pch/pch.h"
#include "mock/MockEuroscopeExtractedRouteInterface.h"
#include "intention/HomeAirfieldGroup.h"

using UKControllerPlugin::IntentionCode::HomeAirfieldGroup;
using UKControllerPluginTest::Euroscope::MockEuroscopeExtractedRouteInterface;
using ::testing::StrictMock;
using ::testing::Return;

namespace UKControllerPluginTest {
    namespace IntentionCode {

        TEST(HomeAirfieldGroup, ItAppliesToAllControllers)
        {
            HomeAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_TRUE(airfieldGroup.AppliesToController("BLA", wrapperMock));
        }

        TEST(HomeAirfieldGroup, HasAirfieldReturnsFalseNotInHome)
        {
            HomeAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_FALSE(airfieldGroup.HasAirfield("EIDW", wrapperMock));
        }

        TEST(HomeAirfieldGroup, HasAirfieldReturnsTrueIfKnownInHome)
        {
            HomeAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_TRUE(airfieldGroup.HasAirfield("EGGD", wrapperMock));
        }

        TEST(HomeAirfieldGroup, GetIntentionCodeForGroupReturnsLastTwoLettersOfIcao)
        {
            HomeAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_EQ(0, airfieldGroup.GetIntentionCodeForGroup("EGGD", wrapperMock).compare("GD"));
        }

        TEST(HomeAirfieldGroup, GetIntentionCodeForGroupReturnsLastTwoLettersOfIcaoWhenMalformed)
        {
            HomeAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_EQ(0, airfieldGroup.GetIntentionCodeForGroup("EGD", wrapperMock).compare("GD"));
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPluginTest
