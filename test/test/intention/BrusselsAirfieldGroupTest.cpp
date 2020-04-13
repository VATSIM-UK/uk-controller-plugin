#include "pch/pch.h"
#include "intention/BrusselsAirfieldGroup.h"
#include "mock/MockEuroscopeExtractedRouteInterface.h"

using UKControllerPlugin::IntentionCode::BrusselsAirfieldGroup;
using UKControllerPluginTest::Euroscope::MockEuroscopeExtractedRouteInterface;
using ::testing::StrictMock;
using ::testing::Return;

namespace UKControllerPluginTest {
    namespace IntentionCode {

        TEST(BrusselsAirfieldGroup, ItDoesntApplyToIrishAirfields)
        {
            BrusselsAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_FALSE(airfieldGroup.AppliesToController("EGAA_APP", wrapperMock));
        }

        TEST(BrusselsAirfieldGroup, ItDoesntApplyToScottishMilAirfields)
        {
            BrusselsAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_FALSE(airfieldGroup.AppliesToController("EGQX_APP", wrapperMock));
        }

        TEST(BrusselsAirfieldGroup, ItDoesntApplyToScottishAirfields)
        {
            BrusselsAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_FALSE(airfieldGroup.AppliesToController("EGPF_APP", wrapperMock));
        }

        TEST(BrusselsAirfieldGroup, ItDoesntApplyToScottishTc)
        {
            BrusselsAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_FALSE(airfieldGroup.AppliesToController("STC_A_CTR", wrapperMock));
        }

        TEST(BrusselsAirfieldGroup, ItDoesntApplyToScottishAc)
        {
            BrusselsAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_FALSE(airfieldGroup.AppliesToController("SCO_M_CTR", wrapperMock));
        }

        TEST(BrusselsAirfieldGroup, ItAppliesToEveryoneElse)
        {
            BrusselsAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_TRUE(airfieldGroup.AppliesToController("BLA", wrapperMock));
        }

        TEST(BrusselsAirfieldGroup, HasAirfieldReturnsFalseNotInBrussels)
        {
            BrusselsAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_FALSE(airfieldGroup.HasAirfield("EGGD", wrapperMock));
        }

        TEST(BrusselsAirfieldGroup, HasAirfieldReturnsFalseNotKnownInBrussels)
        {
            BrusselsAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_FALSE(airfieldGroup.HasAirfield("EBXX", wrapperMock));
        }

        TEST(BrusselsAirfieldGroup, HasAirfieldReturnsFalseIfKnownNotViaKoksy)
        {
            BrusselsAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_CALL(wrapperMock, GetPointsNumber())
                .Times(4)
                .WillRepeatedly(Return(3));

            EXPECT_CALL(wrapperMock, GetPointName(0))
                .Times(1)
                .WillOnce(Return("EGKK"));

            EXPECT_CALL(wrapperMock, GetPointName(1))
                .Times(1)
                .WillOnce(Return("REDFA"));

            EXPECT_CALL(wrapperMock, GetPointName(2))
                .Times(1)
                .WillOnce(Return("EBBR"));

            EXPECT_FALSE(airfieldGroup.HasAirfield("EBBR", wrapperMock));
        }

        TEST(BrusselsAirfieldGroup, HasAirfieldReturnsTrueIfKnownViaKoksy)
        {
            BrusselsAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_CALL(wrapperMock, GetPointsNumber())
                .Times(2)
                .WillRepeatedly(Return(3));

            EXPECT_CALL(wrapperMock, GetPointName(0))
                .Times(1)
                .WillOnce(Return("EGKK"));

            EXPECT_CALL(wrapperMock, GetPointName(1))
                .Times(1)
                .WillOnce(Return("KOK"));

            EXPECT_TRUE(airfieldGroup.HasAirfield("EBBR", wrapperMock));
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPluginTest
