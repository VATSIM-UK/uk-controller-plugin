#include "intention/AirfieldIdentifier.h"

using UKControllerPlugin::IntentionCode::AirfieldIdentifier;

namespace UKControllerPluginTest::IntentionCode {
    class AirfieldIdentifierTest : public testing::Test
    {
        public:
        AirfieldIdentifierTest() : code()
        {
        }

        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> flightplan;
        AirfieldIdentifier code;
    };

    TEST_F(AirfieldIdentifierTest, ItReturnsNotFoundNoDestination)
    {
        EXPECT_CALL(flightplan, GetDestination).Times(1).WillOnce(testing::Return(""));

        EXPECT_EQ("--", code.GenerateCode(flightplan));
    }

    TEST_F(AirfieldIdentifierTest, ItReturnsNotFoundSingleCharacterDestination)
    {
        EXPECT_CALL(flightplan, GetDestination).Times(1).WillOnce(testing::Return("E"));

        EXPECT_EQ("--", code.GenerateCode(flightplan));
    }

    TEST_F(AirfieldIdentifierTest, ItReturnsNotFoundDoubleCharacterDestination)
    {
        EXPECT_CALL(flightplan, GetDestination).Times(1).WillOnce(testing::Return("EG"));

        EXPECT_EQ("--", code.GenerateCode(flightplan));
    }

    TEST_F(AirfieldIdentifierTest, ItReturnsNotFoundTripleCharacterDestination)
    {
        EXPECT_CALL(flightplan, GetDestination).Times(1).WillOnce(testing::Return("EGG"));

        EXPECT_EQ("--", code.GenerateCode(flightplan));
    }

    TEST_F(AirfieldIdentifierTest, ItReturnsNotFoundFiveCharacterDestination)
    {
        EXPECT_CALL(flightplan, GetDestination).Times(1).WillOnce(testing::Return("EGGGD"));

        EXPECT_EQ("--", code.GenerateCode(flightplan));
    }

    TEST_F(AirfieldIdentifierTest, ItReturnsLastTwoLettersOfIcao)
    {
        EXPECT_CALL(flightplan, GetDestination).Times(2).WillRepeatedly(testing::Return("EGGD"));

        EXPECT_EQ("GD", code.GenerateCode(flightplan));
    }
} // namespace UKControllerPluginTest::IntentionCode
