#include "controller/ControllerPosition.h"
#include "prenote/ComparePrenoteMessages.h"
#include "prenote/PrenoteMessage.h"
#include "time/SystemClock.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Prenote::ComparePrenoteMessages;
using UKControllerPlugin::Prenote::PrenoteMessage;
using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPluginTest::Prenote {
    class ComparePrenoteMessagesTest : public ::testing::Test
    {
        public:
        ComparePrenoteMessagesTest()
        {
            sendingPosition = std::make_shared<ControllerPosition>(
                1, "EGKK_TWR", 124.225, std::vector<std::string>{"EGKK"}, true, false);
            receivingPosition = std::make_shared<ControllerPosition>(
                2, "EGKK_F_APP", 124.225, std::vector<std::string>{"EGKK"}, true, false);
            message1 = std::make_shared<PrenoteMessage>(
                1, "BAW123", "EGGD", "BADIM1X", "EGLL", sendingPosition, receivingPosition, TimeNow());
            message2 = std::make_shared<PrenoteMessage>(
                5, "BAW456", "EGGD", "BADIM1X", "EGLL", sendingPosition, receivingPosition, TimeNow());
        }

        ComparePrenoteMessages compare;
        std::shared_ptr<ControllerPosition> sendingPosition;
        std::shared_ptr<ControllerPosition> receivingPosition;
        std::shared_ptr<PrenoteMessage> message1;
        std::shared_ptr<PrenoteMessage> message2;
    };

    TEST_F(ComparePrenoteMessagesTest, LessThanIntReturnsTrueIfLessThan)
    {
        EXPECT_TRUE(compare(message1, 3));
    }

    TEST_F(ComparePrenoteMessagesTest, LessThanIntReturnsFalseIfNotLessThan)
    {
        EXPECT_FALSE(compare(message2, 3));
    }

    TEST_F(ComparePrenoteMessagesTest, LessThanMessageReturnsTrueIfLessThan)
    {
        EXPECT_TRUE(compare(3, message2));
    }

    TEST_F(ComparePrenoteMessagesTest, LessThanMessageReturnsFalseIfNotLessThan)
    {
        EXPECT_FALSE(compare(3, message1));
    }

    TEST_F(ComparePrenoteMessagesTest, LessThanCompareMessagesReturnsTrueIfLessThan)
    {
        EXPECT_TRUE(compare(message1, message2));
    }

    TEST_F(ComparePrenoteMessagesTest, LessThanCompareMessagesReturnsFalseIfNotLessThan)
    {
        EXPECT_FALSE(compare(message2, message1));
    }
} // namespace UKControllerPluginTest::Prenote
