#include "releases/ReleaseApprovalRemarksUserMessage.h"

using UKControllerPlugin::Releases::ReleaseApprovalRemarksUserMessage;

namespace UKControllerPluginTest::Releases {

    class ReleaseApprovalRemarksUserMessageTest : public testing::Test
    {
        public:
        ReleaseApprovalRemarksUserMessageTest() : message("BAW123", "LON_S_CTR", "Some remarks"){};

        ReleaseApprovalRemarksUserMessage message;
    };

    TEST_F(ReleaseApprovalRemarksUserMessageTest, ItUsesAHandler)
    {
        EXPECT_TRUE("DEPARTURE_RELEASE" == this->message.MessageHandler());
    }

    TEST_F(ReleaseApprovalRemarksUserMessageTest, ItHasASender)
    {
        EXPECT_TRUE("UKCP" == this->message.MessageSender());
    }

    TEST_F(ReleaseApprovalRemarksUserMessageTest, ItFormatsAMessage)
    {
        EXPECT_EQ(
            "Departure release approval remarks by LON_S_CTR for BAW123: Some remarks", this->message.MessageString());
    }

    TEST_F(ReleaseApprovalRemarksUserMessageTest, ItShowsHandler)
    {
        EXPECT_TRUE(this->message.MessageShowHandler());
    }

    TEST_F(ReleaseApprovalRemarksUserMessageTest, ItMarksMessageAsUnread)
    {
        EXPECT_TRUE(this->message.MessageMarkUnread());
    }

    TEST_F(ReleaseApprovalRemarksUserMessageTest, ItOverridesBusy)
    {
        EXPECT_TRUE(this->message.MessageOverrideBusy());
    }

    TEST_F(ReleaseApprovalRemarksUserMessageTest, ItFlashesTheHandler)
    {
        EXPECT_TRUE(this->message.MessageFlashHandler());
    }

    TEST_F(ReleaseApprovalRemarksUserMessageTest, ItRequiresConfirm)
    {
        EXPECT_TRUE(this->message.MessageRequiresConfirm());
    }
} // namespace UKControllerPluginTest::Releases
