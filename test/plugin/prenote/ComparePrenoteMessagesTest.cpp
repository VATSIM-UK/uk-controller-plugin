#include "prenote/ComparePrenoteMessages.h"
#include "prenote/PrenoteMessage.h"
#include "time/SystemClock.h"

using UKControllerPlugin::Prenote::ComparePrenoteMessages;
using UKControllerPlugin::Prenote::PrenoteMessage;
using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPluginTest::Prenote {
    class ComparePrenoteMessagesTest : public ::testing::Test
    {
        public:
        ComparePrenoteMessagesTest()
        {
            message1 = std::make_shared<PrenoteMessage>(1, "BAW123", "EGGD", "BADIM1X", "EGLL", 1, 2, TimeNow());
            message2 = std::make_shared<PrenoteMessage>(5, "BAW456", "EGGD", "BADIM1X", "EGLL", 1, 2, TimeNow());
        }

        ComparePrenoteMessages compare;
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
