#include "metar/PressureChangeMessage.h"
#include "metar/PressureComponent.h"

using testing::Test;
using UKControllerPlugin::Metar::PressureChangeMessage;
using UKControllerPlugin::Metar::PressureComponent;
using UKControllerPlugin::Metar::PressureUnit;

namespace UKControllerPluginTest::Metar {

    class PressureChangeMessageTest : public Test
    {
        public:
        PressureChangeMessageTest()
            : hectopascalsComponentBefore(1013, 1011, 29.92, 29.84, PressureUnit::Hectopascals),
              hectopascalsComponentAfter(1014, 1012, 29.94, 29.86, PressureUnit::Hectopascals),
              inHgComponentBefore(1013, 1011, 29.92, 29.84, PressureUnit::InHg),
              inHgComponentAfter(1014, 1012, 29.94, 29.86, PressureUnit::InHg),
              messageHectopascals("EGKK", hectopascalsComponentBefore, hectopascalsComponentAfter),
              messageInhg("EGLL", inHgComponentBefore, inHgComponentAfter)
        {
        }

        PressureComponent hectopascalsComponentBefore;
        PressureComponent hectopascalsComponentAfter;
        PressureComponent inHgComponentBefore;
        PressureComponent inHgComponentAfter;
        PressureChangeMessage messageHectopascals;
        PressureChangeMessage messageInhg;
    };

    TEST_F(PressureChangeMessageTest, ItHasAMessageHandler)
    {
        EXPECT_EQ("UKCP_PRESSURE_MONITOR", messageHectopascals.MessageHandler());
    }

    TEST_F(PressureChangeMessageTest, ItHasASender)
    {
        EXPECT_EQ("UKCP", messageHectopascals.MessageSender());
    }

    TEST_F(PressureChangeMessageTest, ItShowsTheHandler)
    {
        EXPECT_TRUE(messageHectopascals.MessageShowHandler());
    }

    TEST_F(PressureChangeMessageTest, ItMarksItAsUnread)
    {
        EXPECT_TRUE(messageHectopascals.MessageMarkUnread());
    }

    TEST_F(PressureChangeMessageTest, ItOverridesBusy)
    {
        EXPECT_TRUE(messageHectopascals.MessageOverrideBusy());
    }

    TEST_F(PressureChangeMessageTest, ItFlashesTheHandler)
    {
        EXPECT_TRUE(messageHectopascals.MessageFlashHandler());
    }

    TEST_F(PressureChangeMessageTest, ItRequiresConfirmation)
    {
        EXPECT_TRUE(messageHectopascals.MessageRequiresConfirm());
    }

    TEST_F(PressureChangeMessageTest, ItHasAMessageHectopascals)
    {
        EXPECT_EQ(
            "Pressure change at EGKK. QNH was 1013, now 1014. QFE was 1011, now 1012.",
            messageHectopascals.MessageString());
    }

    TEST_F(PressureChangeMessageTest, ItHasAMessageInHg)
    {
        EXPECT_EQ(
            "Pressure change at EGLL. QNH was 29.92, now 29.94. QFE was 29.84, now 29.86.",
            messageInhg.MessageString());
    }
} // namespace UKControllerPluginTest::Metar
