#include "pch/pch.h"
#include "ownership/AirfieldOwnerQueryMessage.h"

using UKControllerPlugin::Ownership::AirfieldOwnerQueryMessage;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Ownership {

        class AirfieldOwnerQueryMessageTest : public Test
        {
            public:
                AirfieldOwnerQueryMessageTest(void)
                    : message("EGKK", "EGKK_TWR", "Testy McTestFace")
                {

                };

                AirfieldOwnerQueryMessage message;
        };

        TEST_F(AirfieldOwnerQueryMessageTest, ItUsesAHandler)
        {
            EXPECT_TRUE("UKCP_Query" == this->message.MessageHandler());
        }

        TEST_F(AirfieldOwnerQueryMessageTest, ItHasASender)
        {
            EXPECT_TRUE("UKCP" == this->message.MessageSender());
        }

        TEST_F(AirfieldOwnerQueryMessageTest, ItFormatsAMessage)
        {
            EXPECT_TRUE("Airfield EGKK is owned by EGKK_TWR (Testy McTestFace)" == this->message.MessageString());
        }

        TEST_F(AirfieldOwnerQueryMessageTest, ItFormatsANotOwnedMessage)
        {
            AirfieldOwnerQueryMessage newMessage("EGKK", "", "");
            EXPECT_TRUE("Airfield EGKK is not owned by any controller" == newMessage.MessageString());
        }

        TEST_F(AirfieldOwnerQueryMessageTest, ItShowsHandler)
        {
            EXPECT_TRUE(this->message.MessageShowHandler());
        }

        TEST_F(AirfieldOwnerQueryMessageTest, ItMarksMessageAsUnread)
        {
            EXPECT_TRUE(this->message.MessageMarkUnread());
        }

        TEST_F(AirfieldOwnerQueryMessageTest, ItOverridesBusy)
        {
            EXPECT_TRUE(this->message.MessageOverrideBusy());
        }

        TEST_F(AirfieldOwnerQueryMessageTest, ItFlashesTheHandler)
        {
            EXPECT_TRUE(this->message.MessageFlashHandler());
        }

        TEST_F(AirfieldOwnerQueryMessageTest, ItDoesntRequireConfirm)
        {
            EXPECT_FALSE(this->message.MessageRequiresConfirm());
        }
    }  // namespace Ownership
}  // namespace UKControllerPluginTest
