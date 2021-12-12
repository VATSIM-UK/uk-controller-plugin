#include "airfield/AirfieldModel.h"
#include "controller/ControllerPositionHierarchy.h"
#include "ownership/AirfieldsOwnedQueryMessage.h"

using ::testing::Test;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Ownership::AirfieldsOwnedQueryMessage;

namespace UKControllerPluginTest {
    namespace Ownership {

        class AirfieldsOwnedQueryMessageTest : public Test
        {
            public:
            AirfieldsOwnedQueryMessageTest(void)
                : message(
                      {std::make_shared<AirfieldModel>(1, "EGKK", std::make_unique<ControllerPositionHierarchy>()),
                       std::make_shared<AirfieldModel>(2, "EGLL", std::make_unique<ControllerPositionHierarchy>()),
                       std::make_shared<AirfieldModel>(3, "EGLC", std::make_unique<ControllerPositionHierarchy>())},
                      "LON_S_CTR"){

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
    } // namespace Ownership
} // namespace UKControllerPluginTest
