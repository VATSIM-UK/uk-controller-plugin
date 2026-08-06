#include "releases/ReleasePendingReminderUserMessage.h"

using UKControllerPlugin::Releases::ReleasePendingReminderUserMessage;

namespace UKControllerPluginTest::Releases {

    class ReleasePendingReminderUserMessageTest : public testing::Test
    {
        public:
        ReleasePendingReminderUserMessageTest() : message("BAW123", "LON_S_CTR") {};

        ReleasePendingReminderUserMessage message;
    };

    TEST_F(ReleasePendingReminderUserMessageTest, ItUsesAHandler)
    {
        EXPECT_TRUE("DEPARTURE_RELEASE" == this->message.MessageHandler());
    }

    TEST_F(ReleasePendingReminderUserMessageTest, ItHasASender)
    {
        EXPECT_TRUE("UKCP" == this->message.MessageSender());
    }

    TEST_F(ReleasePendingReminderUserMessageTest, ItFormatsAMessage)
    {
        EXPECT_EQ("Departure release for BAW123 from LON_S_CTR is still pending.", this->message.MessageString());
    }

    TEST_F(ReleasePendingReminderUserMessageTest, ItShowsHandler)
    {
        EXPECT_TRUE(this->message.MessageShowHandler());
    }

    TEST_F(ReleasePendingReminderUserMessageTest, ItMarksMessageAsUnread)
    {
        EXPECT_TRUE(this->message.MessageMarkUnread());
    }

    TEST_F(ReleasePendingReminderUserMessageTest, ItOverridesBusy)
    {
        EXPECT_TRUE(this->message.MessageOverrideBusy());
    }

    TEST_F(ReleasePendingReminderUserMessageTest, ItFlashesTheHandler)
    {
        EXPECT_TRUE(this->message.MessageFlashHandler());
    }

    TEST_F(ReleasePendingReminderUserMessageTest, ItRequiresConfirm)
    {
        EXPECT_TRUE(this->message.MessageRequiresConfirm());
    }
} // namespace UKControllerPluginTest::Releases
