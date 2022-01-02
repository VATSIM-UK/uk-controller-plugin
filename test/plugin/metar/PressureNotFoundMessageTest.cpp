#include "metar/PressureNotFoundMessage.h"

using testing::Test;
using UKControllerPlugin::Metar::PressureNotFoundMessage;

namespace UKControllerPluginTest::Metar {

    class PressureNotFoundMessageTest : public Test
    {
        public:
        PressureNotFoundMessageTest() : message("EGKK")
        {
        }

        PressureNotFoundMessage message;
    };

    TEST_F(PressureNotFoundMessageTest, ItHasAMessageHandler)
    {
        EXPECT_EQ("UKCP_PRESSURE_MONITOR", message.MessageHandler());
    }

    TEST_F(PressureNotFoundMessageTest, ItHasASender)
    {
        EXPECT_EQ("UKCP", message.MessageSender());
    }

    TEST_F(PressureNotFoundMessageTest, ItShowsTheHandler)
    {
        EXPECT_TRUE(message.MessageShowHandler());
    }

    TEST_F(PressureNotFoundMessageTest, ItMarksItAsUnread)
    {
        EXPECT_TRUE(message.MessageMarkUnread());
    }

    TEST_F(PressureNotFoundMessageTest, ItOverridesBusy)
    {
        EXPECT_TRUE(message.MessageOverrideBusy());
    }

    TEST_F(PressureNotFoundMessageTest, ItFlashesTheHandler)
    {
        EXPECT_TRUE(message.MessageFlashHandler());
    }

    TEST_F(PressureNotFoundMessageTest, ItRequiresConfirmation)
    {
        EXPECT_TRUE(message.MessageRequiresConfirm());
    }

    TEST_F(PressureNotFoundMessageTest, ItHasAMessage)
    {
        EXPECT_EQ("Pressure information is not available for EGKK.", message.MessageString());
    }
} // namespace UKControllerPluginTest::Metar
