#include "pch/pch.h"
#include "controller/AirfieldsOwnedQueryMessage.h"
#include "airfield/Airfield.h"

using UKControllerPlugin::Controller::AirfieldsOwnedQueryMessage;
using UKControllerPlugin::Airfield::Airfield;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Controller {

        class AirfieldsOwnedQueryMessageTest : public Test
        {
            public:
                AirfieldsOwnedQueryMessageTest(void)
                    : message({Airfield("EGKK", {}), Airfield("EGLL", {}), Airfield("EGLC", {})}, "LON_S_CTR")
                {

                };

                AirfieldsOwnedQueryMessage message;
        };

        TEST_F(AirfieldsOwnedQueryMessageTest, ItUsesAHandler)
        {
            EXPECT_TRUE("UKCP_Query" == this->message.MessageHandler());
        }

        TEST_F(AirfieldsOwnedQueryMessageTest, ItHasASender)
        {
            EXPECT_TRUE("UKCP" == this->message.MessageSender());
        }

        TEST_F(AirfieldsOwnedQueryMessageTest, ItFormatsAMessage)
        {
            EXPECT_TRUE("LON_S_CTR owns the following airfields: EGKK, EGLL, EGLC" == this->message.MessageString());
        }

        TEST_F(AirfieldsOwnedQueryMessageTest, ItFormatsANotOwnedMessage)
        {
            AirfieldsOwnedQueryMessage newMessage({}, "LON_S_CTR");
            EXPECT_TRUE("LON_S_CTR does not own any airfields" == newMessage.MessageString());
        }

        TEST_F(AirfieldsOwnedQueryMessageTest, ItShowsHandler)
        {
            EXPECT_TRUE(this->message.MessageShowHandler());
        }

        TEST_F(AirfieldsOwnedQueryMessageTest, ItMarksMessageAsUnread)
        {
            EXPECT_TRUE(this->message.MessageMarkUnread());
        }

        TEST_F(AirfieldsOwnedQueryMessageTest, ItOverridesBusy)
        {
            EXPECT_TRUE(this->message.MessageOverrideBusy());
        }

        TEST_F(AirfieldsOwnedQueryMessageTest, ItFlashesTheHandler)
        {
            EXPECT_TRUE(this->message.MessageFlashHandler());
        }

        TEST_F(AirfieldsOwnedQueryMessageTest, ItDoesntRequireConfirm)
        {
            EXPECT_FALSE(this->message.MessageRequiresConfirm());
        }
    }  // namespace Controller
}  // namespace UKControllerPluginTest
