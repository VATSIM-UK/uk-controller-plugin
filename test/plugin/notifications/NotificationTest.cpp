#include "notifications/Notification.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"

using ::testing::Test;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Notifications::Notification;

namespace UKControllerPluginTest {
    namespace Notifications {

        class NotificationTest : public Test
        {
            public:
            NotificationTest()
                : position(std::make_shared<ControllerPosition>(
                      1, "EGKK_TWR", 124.220, std::vector<std::string>{"EGKK"}, true, false)),
                  position2(std::make_shared<ControllerPosition>(
                      2, "EGKK_APP", 126.820, std::vector<std::string>{"EGKK"}, true, false))
            {
            }
            Notification GetNotification(
                std::chrono::system_clock::time_point validFrom, std::chrono::system_clock::time_point validTo)
            {
                std::unique_ptr<ControllerPositionHierarchy> hierarchy =
                    std::make_unique<ControllerPositionHierarchy>();
                hierarchy->AddPosition(position);

                return Notification(3, "Title", "Body", validFrom, validTo, std::move(hierarchy), "Link");
            }

            std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
            std::chrono::system_clock::time_point oneHourAgo = std::chrono::system_clock::now() - std::chrono::hours(1);
            std::chrono::system_clock::time_point oneHoursTime =
                std::chrono::system_clock::now() + std::chrono::hours(1);
            std::shared_ptr<ControllerPosition> position;
            std::shared_ptr<ControllerPosition> position2;
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

        TEST_F(NotificationTest, ItHasALink)
        {
            EXPECT_EQ("Link", this->GetNotification(now, now).Link());
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
            EXPECT_TRUE(this->GetNotification(now, now).IsRelevant(*this->position));
        }

        TEST_F(NotificationTest, ItIsNotRelevant)
        {
            EXPECT_FALSE(this->GetNotification(now, now).IsRelevant(*this->position2));
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
    } // namespace Notifications
} // namespace UKControllerPluginTest
