#include "controller/ControllerPosition.h"
#include "prenote/PrenoteMessage.h"
#include "time/SystemClock.h"

using ::testing::Test;

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Prenote::PrenoteMessage;
using UKControllerPlugin::Time::SetTestNow;
using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPluginTest::Prenote {
    class PrenoteMessageTest : public Test
    {
        public:
        PrenoteMessageTest()
        {
            SetTestNow(std::chrono::system_clock::now());
            sendingPosition = std::make_shared<ControllerPosition>(
                1, "EGKK_TWR", 124.225, std::vector<std::string>{"EGKK"}, true, false);
            receivingPosition = std::make_shared<ControllerPosition>(
                2, "EGKK_F_APP", 124.225, std::vector<std::string>{"EGKK"}, true, false);
            message = std::make_unique<PrenoteMessage>(
                1, "BAW123", "EGGD", "BADIM1X", "EGLL", sendingPosition, receivingPosition, TimeNow());
        }
        std::shared_ptr<ControllerPosition> sendingPosition;
        std::shared_ptr<ControllerPosition> receivingPosition;
        std::unique_ptr<PrenoteMessage> message;
    };

    TEST_F(PrenoteMessageTest, ItHasAnId)
    {
        EXPECT_EQ(1, message->GetId());
    }

    TEST_F(PrenoteMessageTest, ItHasACallsign)
    {
        EXPECT_EQ("BAW123", message->GetCallsign());
    }

    TEST_F(PrenoteMessageTest, ItHasADepartureAirfield)
    {
        EXPECT_EQ("EGGD", message->GetDepartureAirfield());
    }

    TEST_F(PrenoteMessageTest, ItHasADepartureSid)
    {
        EXPECT_EQ("BADIM1X", message->GetSid());
    }

    TEST_F(PrenoteMessageTest, ItHasAnArrivalAirfield)
    {
        EXPECT_EQ("EGLL", message->GetDestinationAirfield());
    }

    TEST_F(PrenoteMessageTest, ItHasASendingController)
    {
        EXPECT_EQ(sendingPosition, message->GetSendingController());
    }

    TEST_F(PrenoteMessageTest, ItHasATargetController)
    {
        EXPECT_EQ(receivingPosition, message->GetTargetController());
    }

    TEST_F(PrenoteMessageTest, ItHasASendingControllerId)
    {
        EXPECT_EQ(1, message->GetSendingControllerId());
    }

    TEST_F(PrenoteMessageTest, ItHasATargetControllerId)
    {
        EXPECT_EQ(2, message->GetTargetControllerId());
    }

    TEST_F(PrenoteMessageTest, ItHasAnExpiry)
    {
        EXPECT_EQ(TimeNow(), message->GetExpiresAt());
    }

    TEST_F(PrenoteMessageTest, ItStartsUnacknowledged)
    {
        EXPECT_FALSE(message->IsAcknowledged());
    }

    TEST_F(PrenoteMessageTest, ItCanBeAcknowledged)
    {
        message->Acknowledge();
        EXPECT_TRUE(message->IsAcknowledged());
    }

    TEST_F(PrenoteMessageTest, AcknowledgingSetsTheAcknowledgementTime)
    {
        message->Acknowledge();
        EXPECT_EQ(TimeNow(), message->GetAcknowledgedAt());
    }

    TEST_F(PrenoteMessageTest, AcknowledgingSetsTheCreatedTime)
    {
        EXPECT_EQ(TimeNow(), message->GetCreatedAt());
    }
} // namespace UKControllerPluginTest::Prenote
