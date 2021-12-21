#include "missedapproach/MissedApproach.h"
#include "missedapproach/MissedApproachAcknowledgedUserMessage.h"

using UKControllerPlugin::MissedApproach::MissedApproach;
using UKControllerPlugin::MissedApproach::MissedApproachAcknowledgedUserMessage;

namespace UKControllerPluginTest::MissedApproach {

    class MissedApproachAcknowledgedUserMessageTest : public testing::Test
    {
        public:
        MissedApproachAcknowledgedUserMessageTest()
            : missed(1, "BAW123", std::chrono::system_clock::now(), true), message(missed){};

        class MissedApproach missed;
        MissedApproachAcknowledgedUserMessage message;
    };

    TEST_F(MissedApproachAcknowledgedUserMessageTest, ItUsesAHandler)
    {
        EXPECT_TRUE("MISSED_APPROACH" == this->message.MessageHandler());
    }

    TEST_F(MissedApproachAcknowledgedUserMessageTest, ItHasASender)
    {
        EXPECT_TRUE("UKCP" == this->message.MessageSender());
    }

    TEST_F(MissedApproachAcknowledgedUserMessageTest, ItFormatsAMessageWithNoRemarks)
    {
        this->missed.Acknowledge("EGLL_N_APP", "");
        EXPECT_EQ("Missed approach for BAW123 acknowledged by EGLL_N_APP.", this->message.MessageString());
    }

    TEST_F(MissedApproachAcknowledgedUserMessageTest, ItFormatsAMessageWithRemarks)
    {
        this->missed.Acknowledge("EGLL_N_APP", "Hi.");
        EXPECT_EQ("Missed approach for BAW123 acknowledged by EGLL_N_APP. Remarks: Hi.", this->message.MessageString());
    }

    TEST_F(MissedApproachAcknowledgedUserMessageTest, ItShowsHandler)
    {
        EXPECT_TRUE(this->message.MessageShowHandler());
    }

    TEST_F(MissedApproachAcknowledgedUserMessageTest, ItMarksMessageAsUnread)
    {
        EXPECT_TRUE(this->message.MessageMarkUnread());
    }

    TEST_F(MissedApproachAcknowledgedUserMessageTest, ItOverridesBusy)
    {
        EXPECT_TRUE(this->message.MessageOverrideBusy());
    }

    TEST_F(MissedApproachAcknowledgedUserMessageTest, ItFlashesTheHandler)
    {
        EXPECT_TRUE(this->message.MessageFlashHandler());
    }

    TEST_F(MissedApproachAcknowledgedUserMessageTest, ItRequiresConfirm)
    {
        EXPECT_TRUE(this->message.MessageRequiresConfirm());
    }
} // namespace UKControllerPluginTest::MissedApproach
