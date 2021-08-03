#include "pch/pch.h"
#include "metar/PressureChangeMessage.h"

using UKControllerPlugin::Metar::PressureChangeMessage;
using testing::Test;

namespace UKControllerPluginTest {
    namespace Metar {

        class PressureChangeMessageTest : public Test
        {
            public:
                PressureChangeMessageTest()
                    : message("EGKK", "1012", "1013")
                {

                }

                PressureChangeMessage message;
        };

        TEST_F(PressureChangeMessageTest, ItHasAMessageHandler)
        {
            EXPECT_EQ("UKCP_QNH", message.MessageHandler());
        }

        TEST_F(PressureChangeMessageTest, ItHasASender)
        {
            EXPECT_EQ("UKCP", message.MessageSender());
        }

        TEST_F(PressureChangeMessageTest, ItShowsTheHandler)
        {
            EXPECT_TRUE(message.MessageShowHandler());
        }

        TEST_F(PressureChangeMessageTest, ItMarksItAsUnread)
        {
            EXPECT_TRUE(message.MessageMarkUnread());
        }

        TEST_F(PressureChangeMessageTest, ItOverridesBusy)
        {
            EXPECT_TRUE(message.MessageOverrideBusy());
        }

        TEST_F(PressureChangeMessageTest, ItFlashesTheHandler)
        {
            EXPECT_TRUE(message.MessageFlashHandler());
        }

        TEST_F(PressureChangeMessageTest, ItRequiresConfirmation)
        {
            EXPECT_TRUE(message.MessageRequiresConfirm());
        }

        TEST_F(PressureChangeMessageTest, ItHasAMessage)
        {
            EXPECT_EQ("New QNH at EGKK, Was: 1012, Now: 1013", message.MessageString());
        }
    }  // namespace Metar
}  // namespace UKControllerPluginTest
