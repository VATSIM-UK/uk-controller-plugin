#include "pch/pch.h"
#include "notifications/Notification.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"

using ::testing::Test;
using UKControllerPlugin::Notifications::Notification;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;

namespace UKControllerPluginTest {
    namespace Notifications {

        class NotificationTest : public Test
        {
            public:
                Notification GetNotification(
                    std::chrono::system_clock::time_point validFrom,
                    std::chrono::system_clock::time_point validTo
                )
                {
                    ControllerPositionHierarchy hierarchy;
                    hierarchy.AddPosition(position);

                    return Notification(
                        3,
                        "Title",
                        "Body",
                        validFrom,
                        validTo,
                        hierarchy
                    );
                }

                std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
                std::chrono::system_clock::time_point oneHourAgo = std::chrono::system_clock::now() - std::chrono::hours(1);
                std::chrono::system_clock::time_point oneHoursTime = std::chrono::system_clock::now() + std::chrono::hours(1);
                ControllerPosition position = ControllerPosition("EGKK_TWR", 124.220, "TWR", { "EGKK" });
                ControllerPosition position2 = ControllerPosition("EGKK_APP", 126.820, "APP", { "EGKK" });
        };

        TEST_F(NotificationTest, ItHasAnId)
        {
            EXPECT_EQ(3, this->GetNotification(now, now).Id());
        }

        TEST_F(NotificationTest, ItHasATitle)
        {
            EXPECT_EQ("Title", this->GetNotification(now, now).Title());
        }

        TEST_F(NotificationTest, ItHasABody)
        {
            EXPECT_EQ("Body", this->GetNotification(now, now).Body());
        }

        TEST_F(NotificationTest, ItIsNotActiveIfItOccursInTheFuture)
        {
            EXPECT_FALSE(this->GetNotification(oneHoursTime, oneHoursTime).Active());
        }

        TEST_F(NotificationTest, ItIsNotActiveIfItsAlreadyPassed)
        {
            EXPECT_FALSE(this->GetNotification(oneHourAgo, now).Active());
        }
        
        TEST_F(NotificationTest, ItIsActive)
        {
            EXPECT_TRUE(this->GetNotification(oneHourAgo, oneHoursTime).Active());
        }

        TEST_F(NotificationTest, ItIsRelevant)
        {
            EXPECT_TRUE(this->GetNotification(now, now).IsRelevant(this->position));
        }

        TEST_F(NotificationTest, ItIsNotRelevant)
        {
            EXPECT_FALSE(this->GetNotification(now, now).IsRelevant(this->position2));
        }

        TEST_F(NotificationTest, ItDefaultsToNotRead)
        {
            EXPECT_FALSE(this->GetNotification(now, now).IsRead());
        }

        TEST_F(NotificationTest, ItCanBeRead)
        {
            Notification notification = this->GetNotification(now, now);
            notification.Read();
            EXPECT_TRUE(notification.IsRead());
        }
    }  // namespace Notifications
}  // namespace UKControllerPluginTest
