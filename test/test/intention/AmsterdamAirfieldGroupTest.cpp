#include "pch/pch.h"
#include "intention/AirfieldGroup.h"
#include "mock/MockEuroscopeExtractedRouteInterface.h"
#include "intention/AmsterdamAirfieldGroup.h"

using UKControllerPlugin::IntentionCode::AmsterdamAirfieldGroup;
using UKControllerPluginTest::Euroscope::MockEuroscopeExtractedRouteInterface;
using ::testing::StrictMock;
using ::testing::Return;

namespace UKControllerPluginTest {
    namespace IntentionCode {
        TEST(AmsterdamAirfieldGroup, HasAirfieldReturnsFalseNotInAmsterdam)
        {
            AmsterdamAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_FALSE(airfieldGroup.HasAirfield("EGGD", wrapperMock));
        }

        TEST(AmsterdamAirfieldGroup, HasAirfieldReturnsFalseNotKnownInAmsterdam)
        {
            AmsterdamAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_FALSE(airfieldGroup.HasAirfield("EHXX", wrapperMock));
        }

        TEST(AmsterdamAirfieldGroup, HasAirfieldReturnsTrueIfAirfieldKnown)
        {
            AmsterdamAirfieldGroup airfieldGroup;
            StrictMock<MockEuroscopeExtractedRouteInterface> wrapperMock;

            EXPECT_TRUE(airfieldGroup.HasAirfield("EHAM", wrapperMock));
        }

        TEST(AmsterdamAirfieldGroup, HasAirfieldReturnsSecondaryCodeIfViaKoksy)
        {
            AmsterdamAirfieldGroup airfieldGroup;
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

            EXPECT_EQ(0, airfieldGroup.GetIntentionCodeForGroup("EHAM", wrapperMock).compare("AS"));
        }

        TEST(AmsterdamAirfieldGroup, HasAirfieldReturnsMainCodeIfNotViaKoksy)
        {
            AmsterdamAirfieldGroup airfieldGroup;
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
                .WillOnce(Return("EHAM"));

            EXPECT_EQ(0, airfieldGroup.GetIntentionCodeForGroup("EHAM", wrapperMock).compare("AM"));
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPluginTest
