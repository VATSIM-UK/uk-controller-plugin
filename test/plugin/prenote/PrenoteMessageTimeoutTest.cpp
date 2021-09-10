#include "prenote/PrenoteMessage.h"
#include "prenote/PrenoteMessageCollection.h"
#include "prenote/PrenoteMessageTimeout.h"
#include "time/SystemClock.h"

using UKControllerPlugin::Prenote::PrenoteMessage;
using UKControllerPlugin::Prenote::PrenoteMessageCollection;
using UKControllerPlugin::Prenote::PrenoteMessageTimeout;
using UKControllerPlugin::Time::SetTestNow;
using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPluginTest::Prenote {
    class PrenoteMessageTimeoutTest : public ::testing::Test
    {
        public:
        PrenoteMessageTimeoutTest() : messages(std::make_shared<PrenoteMessageCollection>()), timeout(messages)
        {
            SetTestNow(TimeNow());
            message1 = std::make_shared<PrenoteMessage>(
                1, "BAW123", "EGGD", "BADIM1X", "EGLL", 1, 2, TimeNow() + std::chrono::minutes(1));
            message2 = std::make_shared<PrenoteMessage>(
                2, "BAW456", "EGGD", "BADIM1X", "EGLL", 1, 2, TimeNow() - std::chrono::seconds(5));
            message3 = std::make_shared<PrenoteMessage>(
                3, "BAW456", "EGGD", "BADIM1X", "EGLL", 1, 2, TimeNow() - std::chrono::seconds(5));
            message4 = std::make_shared<PrenoteMessage>(
                4, "BAW456", "EGGD", "BADIM1X", "EGLL", 1, 2, TimeNow() - std::chrono::seconds(5));

            messages->Add(message1);
            messages->Add(message2);
            messages->Add(message3);
            messages->Add(message4);
        }

        std::shared_ptr<PrenoteMessageCollection> messages;
        std::shared_ptr<PrenoteMessage> message1;
        std::shared_ptr<PrenoteMessage> message2;
        std::shared_ptr<PrenoteMessage> message3;
        std::shared_ptr<PrenoteMessage> message4;
        PrenoteMessageTimeout timeout;
    };

    TEST_F(PrenoteMessageTimeoutTest, ItPreservesNonAcknowledgedNonExpiredMessages)
    {
        timeout.TimedEventTrigger();
        EXPECT_EQ(message1, messages->GetById(1));
    }

    TEST_F(PrenoteMessageTimeoutTest, ItRemovesNonAcknowledgedExpiredMessages)
    {
        timeout.TimedEventTrigger();
        EXPECT_EQ(nullptr, messages->GetById(2));
    }

    TEST_F(PrenoteMessageTimeoutTest, ItPreservesMessagesAcknowledgedInThePastTenMinutes)
    {
        auto timeBefore = TimeNow();
        SetTestNow(TimeNow() - std::chrono::minutes(9));
        message3->Acknowledge();
        SetTestNow(timeBefore);
        timeout.TimedEventTrigger();
        EXPECT_EQ(message3, messages->GetById(3));
    }

    TEST_F(PrenoteMessageTimeoutTest, ItRemovesMessagesAcknowledgedMoreThanTenMinutesAgo)
    {
        auto timeBefore = TimeNow();
        SetTestNow(TimeNow() - std::chrono::minutes(11));
        message4->Acknowledge();
        SetTestNow(timeBefore);
        timeout.TimedEventTrigger();
        EXPECT_EQ(nullptr, messages->GetById(4));
    }
} // namespace UKControllerPluginTest::Prenote
