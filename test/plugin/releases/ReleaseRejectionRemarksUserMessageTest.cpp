#include "releases/ReleaseRejectionRemarksUserMessage.h"

using UKControllerPlugin::Releases::ReleaseRejectionRemarksUserMessage;

namespace UKControllerPluginTest::Releases {

    class ReleaseRejectionRemarksUserMessageTest : public testing::Test
    {
        public:
        ReleaseRejectionRemarksUserMessageTest() : message("BAW123", "LON_S_CTR", "Some remarks"){};

        ReleaseRejectionRemarksUserMessage message;
    };

    TEST_F(ReleaseRejectionRemarksUserMessageTest, ItUsesAHandler)
    {
        EXPECT_TRUE("DEPARTURE_RELEASE" == this->message.MessageHandler());
    }

    TEST_F(ReleaseRejectionRemarksUserMessageTest, ItHasASender)
    {
        EXPECT_TRUE("UKCP" == this->message.MessageSender());
    }

    TEST_F(ReleaseRejectionRemarksUserMessageTest, ItFormatsAMessage)
    {
        EXPECT_EQ(
            "Departure release rejection remarks by LON_S_CTR for BAW123: Some remarks", this->message.MessageString());
    }

    TEST_F(ReleaseRejectionRemarksUserMessageTest, ItShowsHandler)
    {
        EXPECT_TRUE(this->message.MessageShowHandler());
    }

    TEST_F(ReleaseRejectionRemarksUserMessageTest, ItMarksMessageAsUnread)
    {
        EXPECT_TRUE(this->message.MessageMarkUnread());
    }

    TEST_F(ReleaseRejectionRemarksUserMessageTest, ItOverridesBusy)
    {
        EXPECT_TRUE(this->message.MessageOverrideBusy());
    }

    TEST_F(ReleaseRejectionRemarksUserMessageTest, ItFlashesTheHandler)
    {
        EXPECT_TRUE(this->message.MessageFlashHandler());
    }

    TEST_F(ReleaseRejectionRemarksUserMessageTest, ItRequiresConfirm)
    {
        EXPECT_TRUE(this->message.MessageRequiresConfirm());
    }
} // namespace UKControllerPluginTest::Releases
