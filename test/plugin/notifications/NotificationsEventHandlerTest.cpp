#include "notifications/NotificationsEventHandler.h"
#include "notifications/NotificationsRepository.h"
#include "message/UserMessager.h"
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Test;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Notifications::NotificationsEventHandler;
using UKControllerPlugin::Notifications::NotificationsRepository;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;

namespace UKControllerPluginTest {
    namespace Notifications {

        class NotificationsEventHandlerTest : public Test
        {
            public:
            NotificationsEventHandlerTest() : messager(mockPlugin)
            {
                repository = std::make_shared<NotificationsRepository>();
                handler = std::make_shared<NotificationsEventHandler>(repository, messager);
            }

            std::shared_ptr<ControllerPosition> controller1 =
                std::make_shared<ControllerPosition>(1, "EGLL_N_APP", 199.998, topDown, true, false);
            std::shared_ptr<ControllerPosition> controller2 =
                std::make_shared<ControllerPosition>(2, "EGLL_S_TWR", 199.998, topDown, true, false);
            std::vector<std::string> topDown = {"EGLL"};
            ActiveCallsign callsign = ActiveCallsign("EGLL_S_TWR", "Bobby", *controller1, true);
            ActiveCallsign notUserCallsign = ActiveCallsign("EGLL_S_TWR", "Bobby", *controller1, false);
            UserMessager messager;
            NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
            std::shared_ptr<NotificationsRepository> repository;
            std::shared_ptr<NotificationsEventHandler> handler;
        };

        TEST_F(NotificationsEventHandlerTest, ItDoesNothingIfNotUserPosition)
        {
            EXPECT_CALL(this->mockPlugin, ChatAreaMessage(_, _, _, _, _, _, _, _)).Times(0);

            this->handler->ActiveCallsignAdded(this->notUserCallsign);
        }

        TEST_F(NotificationsEventHandlerTest, ItDoesNothingIfNoRelevantNotifications)
        {
            auto notification = std::make_unique<UKControllerPlugin::Notifications::Notification>(
                1,
                "abc",
                "def",
                std::chrono::system_clock::now(),
                std::chrono::system_clock::now(),
                std::make_unique<UKControllerPlugin::Controller::ControllerPositionHierarchy>(),
                "link");
            repository->Add(std::move(notification));

            EXPECT_CALL(this->mockPlugin, ChatAreaMessage(_, _, _, _, _, _, _, _)).Times(0);

            this->handler->ActiveCallsignAdded(this->callsign);
        }

        TEST_F(NotificationsEventHandlerTest, ItNotifiesForRelevantNotifications)
        {
            // Not relevant
            auto hierarchy = std::make_unique<UKControllerPlugin::Controller::ControllerPositionHierarchy>();
            hierarchy->AddPosition(controller2);
            auto notification1 = std::make_unique<UKControllerPlugin::Notifications::Notification>(
                1,
                "abc",
                "def",
                std::chrono::system_clock::now(),
                std::chrono::system_clock::now(),
                std::move(hierarchy),
                "link");
            repository->Add(std::move(notification1));

            // Relevant but read
            auto hierarchy2 = std::make_unique<UKControllerPlugin::Controller::ControllerPositionHierarchy>();
            hierarchy2->AddPosition(controller1);
            auto notification2 = std::make_unique<UKControllerPlugin::Notifications::Notification>(
                1,
                "abc",
                "def",
                std::chrono::system_clock::now(),
                std::chrono::system_clock::now(),
                std::move(hierarchy2),
                "link");
            notification2->Read();
            repository->Add(std::move(notification2));

            // Relevant and unread
            auto hierarchy3 = std::make_unique<UKControllerPlugin::Controller::ControllerPositionHierarchy>();
            hierarchy3->AddPosition(controller1);
            auto notification3 = std::make_unique<UKControllerPlugin::Notifications::Notification>(
                1,
                "abc",
                "def",
                std::chrono::system_clock::now(),
                std::chrono::system_clock::now(),
                std::move(hierarchy3),
                "link");
            repository->Add(std::move(notification3));

            EXPECT_CALL(
                this->mockPlugin,
                ChatAreaMessage(
                    _,
                    _,
                    "You have 1 unread notification relevant to the position you are controlling.",
                    _,
                    _,
                    _,
                    _,
                    _))
                .Times(1);

            this->handler->ActiveCallsignAdded(this->callsign);
        }
    } // namespace Notifications
} // namespace UKControllerPluginTest
