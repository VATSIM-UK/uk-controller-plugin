#include "pch/pch.h"
#include "notifications/NotificationsRepository.h"

using ::testing::Test;
using UKControllerPlugin::Notifications::NotificationsRepository;

namespace UKControllerPluginTest {
    namespace Notifications {

        class NotificationsRepositoryTest : public Test
        {
            public:

                NotificationsRepository repository;
        };

        TEST_F(NotificationsRepositoryTest, ItStartsEmpty)
        {
            EXPECT_EQ(0, repository.Count());
        }

        TEST_F(NotificationsRepositoryTest, ItAddsNotifications)
        {
            repository.Add(std::make_unique<UKControllerPlugin::Notifications::Notification>(
                1,
                "abc",
                "def",
                std::chrono::system_clock::now(),
                std::chrono::system_clock::now(),
                std::make_unique<UKControllerPlugin::Controller::ControllerPositionHierarchy>(),
                "link"
            ));
            repository.Add(std::make_unique<UKControllerPlugin::Notifications::Notification>(
                1,
                "abc",
                "def",
                std::chrono::system_clock::now(),
                std::chrono::system_clock::now(),
                std::make_unique<UKControllerPlugin::Controller::ControllerPositionHierarchy>(),
                "link"
            ));
            EXPECT_EQ(2, repository.Count());
        }

        TEST_F(NotificationsRepositoryTest, ItReturnsNotifications)
        {
            auto notification = std::make_unique<UKControllerPlugin::Notifications::Notification>(
                1,
                "abc",
                "def",
                std::chrono::system_clock::now(),
                std::chrono::system_clock::now(),
                std::make_unique<UKControllerPlugin::Controller::ControllerPositionHierarchy>(),
                "link"
            );
            auto rawPtr = notification.get();
            repository.Add(std::move(notification));
            EXPECT_EQ(rawPtr, repository.Get(1));
        }

        TEST_F(NotificationsRepositoryTest, ItReturnsNullPointerOnInvalidNotification)
        {
            repository.Add(std::make_unique<UKControllerPlugin::Notifications::Notification>(
                1,
                "abc",
                "def",
                std::chrono::system_clock::now(),
                std::chrono::system_clock::now(),
                std::make_unique<UKControllerPlugin::Controller::ControllerPositionHierarchy>(),
                "link"
            ));
            EXPECT_EQ(nullptr, repository.Get(2));
        }
    }  // namespace Notifications
}  // namespace UKControllerPluginTest
