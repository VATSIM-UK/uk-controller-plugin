#include "metar/PressureComponent.h"
#include "metar/PressureQueryMessage.h"

using testing::Test;
using UKControllerPlugin::Metar::PressureComponent;
using UKControllerPlugin::Metar::PressureQueryMessage;
using UKControllerPlugin::Metar::PressureUnit;

namespace UKControllerPluginTest::Metar {

    class PressureQueryMessageTest : public Test
    {
        public:
        PressureQueryMessageTest()
            : hectopascalsComponent(1013, 1011, 29.92, 29.84, PressureUnit::Hectopascals),
              inHgComponent(1013, 1011, 29.92, 29.84, PressureUnit::InHg),
              messageHectopascals("EGKK", hectopascalsComponent), messageInhg("EGLL", inHgComponent)
        {
        }

        PressureComponent hectopascalsComponent;
        PressureComponent inHgComponent;
        PressureQueryMessage messageHectopascals;
        PressureQueryMessage messageInhg;
    };

    TEST_F(PressureQueryMessageTest, ItHasAMessageHandler)
    {
        EXPECT_EQ("UKCP_PRESSURE_MONITOR", messageHectopascals.MessageHandler());
    }

    TEST_F(PressureQueryMessageTest, ItHasASender)
    {
        EXPECT_EQ("UKCP", messageHectopascals.MessageSender());
    }

    TEST_F(PressureQueryMessageTest, ItShowsTheHandler)
    {
        EXPECT_TRUE(messageHectopascals.MessageShowHandler());
    }

    TEST_F(PressureQueryMessageTest, ItMarksItAsUnread)
    {
        EXPECT_TRUE(messageHectopascals.MessageMarkUnread());
    }

    TEST_F(PressureQueryMessageTest, ItOverridesBusy)
    {
        EXPECT_TRUE(messageHectopascals.MessageOverrideBusy());
    }

    TEST_F(PressureQueryMessageTest, ItFlashesTheHandler)
    {
        EXPECT_TRUE(messageHectopascals.MessageFlashHandler());
    }

    TEST_F(PressureQueryMessageTest, ItRequiresConfirmation)
    {
        EXPECT_TRUE(messageHectopascals.MessageRequiresConfirm());
    }

    TEST_F(PressureQueryMessageTest, ItHasAMessageHectopascals)
    {
        EXPECT_EQ("QNH at EGKK is 1013, QFE is 1011.", messageHectopascals.MessageString());
    }

    TEST_F(PressureQueryMessageTest, ItHasAMessageInHg)
    {
        EXPECT_EQ("QNH at EGLL is 29.92, QFE is 29.84.", messageInhg.MessageString());
    }
} // namespace UKControllerPluginTest::Metar
