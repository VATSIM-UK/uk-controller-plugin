#include "pch/pch.h"
#include "notifications/UnreadNotificationsMessage.h"

using ::testing::Test;
using UKControllerPlugin::Notifications::UnreadNotificationsMessage;

namespace UKControllerPluginTest {
    namespace Notifications {

        class UnreadNotificationsMessageTest : public Test
        {
            public:
                UnreadNotificationsMessageTest()
                    : message(2)
                {

                }

                UnreadNotificationsMessage message;
        };

        TEST_F(UnreadNotificationsMessageTest, ItHasAMessageHandler)
        {
            EXPECT_EQ("UKCP_NOTIFICATION", message.MessageHandler());
        }

        TEST_F(UnreadNotificationsMessageTest, ItHasASender)
        {
            EXPECT_EQ("UKCP", message.MessageSender());
        }

        TEST_F(UnreadNotificationsMessageTest, ItShowsTheHandler)
        {
            EXPECT_TRUE(message.MessageShowHandler());
        }

        TEST_F(UnreadNotificationsMessageTest, ItMarksItAsUnread)
        {
            EXPECT_TRUE(message.MessageMarkUnread());
        }

        TEST_F(UnreadNotificationsMessageTest, ItOverridesBusy)
        {
            EXPECT_TRUE(message.MessageOverrideBusy());
        }

        TEST_F(UnreadNotificationsMessageTest, ItFlashesTheHandler)
        {
            EXPECT_TRUE(message.MessageFlashHandler());
        }

        TEST_F(UnreadNotificationsMessageTest, ItRequiresConfirmation)
        {
            EXPECT_TRUE(message.MessageRequiresConfirm());
        }

        TEST_F(UnreadNotificationsMessageTest, ItHasAMessageForMultipleNotifications)
        {
            EXPECT_EQ(
                "You have 2 unread notifications relevant to the position you are controlling.",
                message.MessageString()
            );
        }

        TEST_F(UnreadNotificationsMessageTest, ItHasAMessageForSingleNotifications)
        {
            UnreadNotificationsMessage messageSingle(1);
            EXPECT_EQ(
                "You have 1 unread notification relevant to the position you are controlling.",
                messageSingle.MessageString()
            );
        }
    }  // namespace Notifications
}  // namespace UKControllerPluginTest
