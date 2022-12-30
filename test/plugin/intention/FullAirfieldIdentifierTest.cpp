#include "intention/FullAirfieldIdentifier.h"

using UKControllerPlugin::IntentionCode::FullAirfieldIdentifier;

namespace UKControllerPluginTest::IntentionCode {
    class FullAirfieldIdentifierTest : public testing::Test
    {
        public:
        FullAirfieldIdentifierTest() : code()
        {
        }

        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> flightplan;
        FullAirfieldIdentifier code;
    };

    TEST_F(FullAirfieldIdentifierTest, ItReturnsNotFoundNoDestination)
    {
        EXPECT_CALL(flightplan, GetDestination).Times(1).WillOnce(testing::Return(""));

        EXPECT_EQ("--", code.GenerateCode(flightplan));
    }

    TEST_F(FullAirfieldIdentifierTest, ItReturnsNotFoundSingleCharacterDestination)
    {
        EXPECT_CALL(flightplan, GetDestination).Times(1).WillOnce(testing::Return("E"));

        EXPECT_EQ("--", code.GenerateCode(flightplan));
    }

    TEST_F(FullAirfieldIdentifierTest, ItReturnsNotFoundDoubleCharacterDestination)
    {
        EXPECT_CALL(flightplan, GetDestination).Times(1).WillOnce(testing::Return("EG"));

        EXPECT_EQ("--", code.GenerateCode(flightplan));
    }

    TEST_F(FullAirfieldIdentifierTest, ItReturnsNotFoundTripleCharacterDestination)
    {
        EXPECT_CALL(flightplan, GetDestination).Times(1).WillOnce(testing::Return("EGG"));

        EXPECT_EQ("--", code.GenerateCode(flightplan));
    }

    TEST_F(FullAirfieldIdentifierTest, ItReturnsNotFoundFiveCharacterDestination)
    {
        EXPECT_CALL(flightplan, GetDestination).Times(1).WillOnce(testing::Return("EGGGD"));

        EXPECT_EQ("--", code.GenerateCode(flightplan));
    }

    TEST_F(FullAirfieldIdentifierTest, ItReturnsIcao)
    {
        EXPECT_CALL(flightplan, GetDestination).Times(2).WillOnce(testing::Return("EGGD"));

        EXPECT_EQ("EGGD", code.GenerateCode(flightplan));
    }
} // namespace UKControllerPluginTest::IntentionCode
