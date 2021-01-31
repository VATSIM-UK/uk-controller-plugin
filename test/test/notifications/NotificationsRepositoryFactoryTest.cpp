#include "pch/pch.h"
#include "notifications/NotificationsRepositoryFactory.h"
#include "bootstrap/PersistenceContainer.h"
#include "mock/MockApiInterface.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPositionHierarchyFactory.h"
#include "controller/ControllerPosition.h"
#include "api/ApiException.h"

using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Throw;
using UKControllerPlugin::Notifications::UnreadNotificationsValid;
using UKControllerPlugin::Notifications::ControllersValid;
using UKControllerPlugin::Notifications::NotificationValid;
using UKControllerPlugin::Notifications::NotificationsValid;
using UKControllerPlugin::Notifications::Make;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchyFactory;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Notifications::NotificationsRepository;
using UKControllerPlugin::Notifications::Notification;

namespace UKControllerPluginTest {
    namespace Notifications {

        class NotificationsRepositoryFactoryTest : public Test
        {
            public:
                NotificationsRepositoryFactoryTest()
                {
                    std::unique_ptr<NiceMock<MockApiInterface>> apiMock = std::make_unique<NiceMock<MockApiInterface>>();
                    mockApi = apiMock.get();
                    container.api = std::move(apiMock);
                    container.controllerPositions = std::make_unique<ControllerPositionCollection>();
                    std::vector<std::string> handoffs = { "EGKK" };

                    std::unique_ptr<ControllerPosition>position1 = std::make_unique<ControllerPosition>(
                        "EGKK_DEL",
                        121.950,
                        "DEL",
                        handoffs
                    );
                    this->position1 = position1.get();

                    std::unique_ptr<ControllerPosition>position2 = std::make_unique<ControllerPosition>(
                        "EGKK_TWR",
                        124.220,
                        "TWR",
                        handoffs
                    );
                    this->position2 = position2.get();

                    std::unique_ptr<ControllerPosition> position3 = std::make_unique<ControllerPosition>(
                        "EGKK_APP",
                        126.820,
                        "APP",
                        handoffs
                    );
                    this->position3 = position3.get();

                    container.controllerPositions->AddPosition(std::move(position1));
                    container.controllerPositions->AddPosition(std::move(position2));
                    container.controllerPositions->AddPosition(std::move(position3));

                    hierarchyFactory = std::make_unique<ControllerPositionHierarchyFactory>(
                        *container.controllerPositions
                    );
                }

                ControllerPosition * position1;
                ControllerPosition * position2;
               ControllerPosition * position3;
                NiceMock<MockApiInterface> * mockApi;
                std::unique_ptr<ControllerPositionHierarchyFactory> hierarchyFactory;
                PersistenceContainer container;
        };

        TEST_F(NotificationsRepositoryFactoryTest, UnreadNotificationsValidReturnsFalseIfNotArray)
        {
            EXPECT_FALSE(UnreadNotificationsValid(nlohmann::json::object()));
        }

        TEST_F(NotificationsRepositoryFactoryTest, UnreadNotificationsValidReturnsFalseIfNotificationDoesNotContainId)
        {
            nlohmann::json notifications = nlohmann::json::array({
                {
                    {"id", 1}
                },
                {
                    {"notid", 2}
                }
            });
            EXPECT_FALSE(UnreadNotificationsValid(notifications));
        }

        TEST_F(NotificationsRepositoryFactoryTest, UnreadNotificationsValidReturnsFalseIfIdNotANumber)
        {
            nlohmann::json notifications = nlohmann::json::array({
                {
                    {"id", "abc"}
                },
            });
            EXPECT_FALSE(UnreadNotificationsValid(notifications));
        }

        TEST_F(NotificationsRepositoryFactoryTest, UnreadNotificationsValidReturnsTrueIfAllNotificationsValid)
        {
            nlohmann::json notifications = nlohmann::json::array({
                {
                    {"id", 1}
                },
            });
            EXPECT_TRUE(UnreadNotificationsValid(notifications));
        }

        TEST_F(NotificationsRepositoryFactoryTest, ControllersValidReturnsFalseIfHierarchyInvalid)
        {
            nlohmann::json controllers = nlohmann::json::object();
            EXPECT_FALSE(ControllersValid(controllers, *hierarchyFactory));
        }

        TEST_F(NotificationsRepositoryFactoryTest, ControllersValidReturnsTrueIfHierarchyValid)
        {
            nlohmann::json controllers = nlohmann::json::array({"EGKK_DEL", "EGKK_TWR"});
            EXPECT_TRUE(ControllersValid(controllers, *hierarchyFactory));
        }

        TEST_F(NotificationsRepositoryFactoryTest, NotificationValidReturnsTrueIfValid)
        {
            nlohmann::json notification = {
                {"id", 1},
                {"title", "title"},
                {"body", "body"},
                {"valid_from", "2021-01-31 16:15:00"},
                {"valid_to", "2021-01-31 16:20:00"},
                {"controllers", nlohmann::json::array({"EGKK_DEL", "EGKK_TWR"})},
            };
            EXPECT_TRUE(NotificationValid(notification, *hierarchyFactory));
        }

        TEST_F(NotificationsRepositoryFactoryTest, NotificationValidReturnsFalseNotObject)
        {
            EXPECT_FALSE(NotificationValid(nlohmann::json::array(), *hierarchyFactory));
        }

        TEST_F(NotificationsRepositoryFactoryTest, NotificationValidReturnsFalseMissingId)
        {
            nlohmann::json notification = {
                {"title", "title"},
                {"body", "body"},
                {"valid_from", "2021-01-31 16:15:00"},
                {"valid_to", "2021-01-31 16:20:00"},
                {"controllers", nlohmann::json::array({"EGKK_DEL", "EGKK_TWR"})},
            };
            EXPECT_FALSE(NotificationValid(notification, *hierarchyFactory));
        }

        TEST_F(NotificationsRepositoryFactoryTest, NotificationValidReturnsFalseIdNotInteger)
        {
            nlohmann::json notification = {
                {"id", "1"},
                {"title", "title"},
                {"body", "body"},
                {"valid_from", "2021-01-31 16:15:00"},
                {"valid_to", "2021-01-31 16:20:00"},
                {"controllers", nlohmann::json::array({"EGKK_DEL", "EGKK_TWR"})},
            };
            EXPECT_FALSE(NotificationValid(notification, *hierarchyFactory));
        }

        TEST_F(NotificationsRepositoryFactoryTest, NotificationValidReturnsFalseMissingTitle)
        {
            nlohmann::json notification = {
                {"id", 1},
                {"body", "body"},
                {"valid_from", "2021-01-31 16:15:00"},
                {"valid_to", "2021-01-31 16:20:00"},
                {"controllers", nlohmann::json::array({"EGKK_DEL", "EGKK_TWR"})},
            };
            EXPECT_FALSE(NotificationValid(notification, *hierarchyFactory));
        }

        TEST_F(NotificationsRepositoryFactoryTest, NotificationValidReturnsFalseTitleNotString)
        {
            nlohmann::json notification = {
                {"id", 1},
                {"title", 123},
                {"body", "body"},
                {"valid_from", "2021-01-31 16:15:00"},
                {"valid_to", "2021-01-31 16:20:00"},
                {"controllers", nlohmann::json::array({"EGKK_DEL", "EGKK_TWR"})},
            };
            EXPECT_FALSE(NotificationValid(notification, *hierarchyFactory));
        }

        TEST_F(NotificationsRepositoryFactoryTest, NotificationValidReturnsFalseMissingBody)
        {
            nlohmann::json notification = {
                {"id", 1},
                {"title", "title"},
                {"valid_from", "2021-01-31 16:15:00"},
                {"valid_to", "2021-01-31 16:20:00"},
                {"controllers", nlohmann::json::array({"EGKK_DEL", "EGKK_TWR"})},
            };
            EXPECT_FALSE(NotificationValid(notification, *hierarchyFactory));
        }

        TEST_F(NotificationsRepositoryFactoryTest, NotificationValidReturnsFalseBodyNotString)
        {
            nlohmann::json notification = {
                {"id", 1},
                {"title", "title"},
                {"body", 123},
                {"valid_from", "2021-01-31 16:15:00"},
                {"valid_to", "2021-01-31 16:20:00"},
                {"controllers", nlohmann::json::array({"EGKK_DEL", "EGKK_TWR"})},
            };
            EXPECT_FALSE(NotificationValid(notification, *hierarchyFactory));
        }

        TEST_F(NotificationsRepositoryFactoryTest, NotificationValidReturnsFalseMissingValidFrom)
        {
            nlohmann::json notification = {
                {"id", 1},
                {"title", "title"},
                {"body", "body"},
                {"valid_to", "2021-01-31 16:20:00"},
                {"controllers", nlohmann::json::array({"EGKK_DEL", "EGKK_TWR"})},
            };
            EXPECT_FALSE(NotificationValid(notification, *hierarchyFactory));
        }

        TEST_F(NotificationsRepositoryFactoryTest, NotificationValidReturnsFalseValidFromNotString)
        {
            nlohmann::json notification = {
                {"id", 1},
                {"title", "title"},
                {"body", "body"},
                {"valid_from", 123},
                {"valid_to", "2021-01-31 16:20:00"},
                {"controllers", nlohmann::json::array({"EGKK_DEL", "EGKK_TWR"})},
            };
            EXPECT_FALSE(NotificationValid(notification, *hierarchyFactory));
        }

        TEST_F(NotificationsRepositoryFactoryTest, NotificationValidReturnsFalseValidFromInvalidTimestamp)
        {
            nlohmann::json notification = {
                {"id", 1},
                {"title", "title"},
                {"body", "body"},
                {"valid_from", "2021:01:31 16:15:00"},
                {"valid_to", "2021-01-31 16:20:00"},
                {"controllers", nlohmann::json::array({"EGKK_DEL", "EGKK_TWR"})},
            };
            EXPECT_FALSE(NotificationValid(notification, *hierarchyFactory));
        }

        TEST_F(NotificationsRepositoryFactoryTest, NotificationValidReturnsFalseMissingValidTo)
        {
            nlohmann::json notification = {
                {"id", 1},
                {"title", "title"},
                {"body", "body"},
                {"valid_from", "2021-01-31 16:15:00"},
                {"controllers", nlohmann::json::array({"EGKK_DEL", "EGKK_TWR"})},
            };
            EXPECT_FALSE(NotificationValid(notification, *hierarchyFactory));
        }

        TEST_F(NotificationsRepositoryFactoryTest, NotificationValidReturnsFalseValidToNotString)
        {
            nlohmann::json notification = {
                {"id", 1},
                {"title", "title"},
                {"body", "body"},
                {"valid_from", "2021-01-31 16:15:00"},
                {"valid_to", 123},
                {"controllers", nlohmann::json::array({"EGKK_DEL", "EGKK_TWR"})},
            };
            EXPECT_FALSE(NotificationValid(notification, *hierarchyFactory));
        }

        TEST_F(NotificationsRepositoryFactoryTest, NotificationValidReturnsFalseValidToNotValidTimestamp)
        {
            nlohmann::json notification = {
                {"id", 1},
                {"title", "title"},
                {"body", "body"},
                {"valid_from", "2021-01-31 16:15:00"},
                {"valid_to", "2021:01:31 16:20:00"},
                {"controllers", nlohmann::json::array({"EGKK_DEL", "EGKK_TWR"})},
            };
            EXPECT_FALSE(NotificationValid(notification, *hierarchyFactory));
        }

        TEST_F(NotificationsRepositoryFactoryTest, NotificationValidReturnsFalseMissingControllers)
        {
            nlohmann::json notification = {
                {"id", 1},
                {"title", "title"},
                {"body", "body"},
                {"valid_from", "2021-01-31 16:15:00"},
                {"valid_to", "2021-01-31 16:20:00"},
            };
            EXPECT_FALSE(NotificationValid(notification, *hierarchyFactory));
        }

        TEST_F(NotificationsRepositoryFactoryTest, NotificationValidReturnsFalseControllersInvalid)
        {
            nlohmann::json notification = {
                {"id", 1},
                {"title", "title"},
                {"body", "body"},
                {"valid_from", "2021-01-31 16:15:00"},
                {"valid_to", "2021-01-31 16:20:00"},
                {"controllers", 123},
            };
            EXPECT_FALSE(NotificationValid(notification, *hierarchyFactory));
        }

        
        TEST_F(NotificationsRepositoryFactoryTest, NotificationValidReturnsTrueIfAllValid)
        {
            nlohmann::json notifications = {
                {"id", 1},
                {"title", "title"},
                {"body", "body"},
                {"valid_from", "2021-01-31 16:15:00"},
                {"valid_to", "2021-01-31 16:20:00"},
                {"controllers", nlohmann::json::array({"EGKK_DEL", "EGKK_TWR"})},
            };
            EXPECT_TRUE(NotificationValid(notifications, *hierarchyFactory));
        }

        TEST_F(NotificationsRepositoryFactoryTest, NotificationValidReturnsFalseIfOneInvalid)
        {
            nlohmann::json notifications = nlohmann::json::array({
                {
                    {"id", 1},
                    {"title", "title"},
                    {"body", "body"},
                    {"valid_from", "2021-01-31 16:15:00"},
                    {"valid_to", "2021-01-31 16:20:00"},
                    {"controllers", nlohmann::json::array({"EGKK_DEL", "EGKK_TWR"})},
                },
                {
                    {"id", "abc"}, // Invalid id
                    {"title", "title"},
                    {"body", "body"},
                    {"valid_from", "2021-01-31 16:15:00"},
                    {"valid_to", "2021-01-31 16:20:00"},
                    {"controllers", nlohmann::json::array({"EGKK_DEL", "EGKK_TWR"})},
                }
            });
            EXPECT_FALSE(NotificationsValid(notifications, *hierarchyFactory));
        }

        TEST_F(NotificationsRepositoryFactoryTest, NotificationValidReturnsFalseIfNotArray)
        {
            EXPECT_FALSE(NotificationsValid(nlohmann::json::object(), *hierarchyFactory));
        }

        TEST_F(NotificationsRepositoryFactoryTest, MakeHandlesExceptionFromRequestingNotifications)
        {
            ON_CALL(*mockApi, GetAllNotifications())
                .WillByDefault(Throw(ApiException("foo")));

            NotificationsRepository repo = Make(container);
            EXPECT_EQ(0, repo.Count());
        }

        TEST_F(NotificationsRepositoryFactoryTest, MakeHandlesInvalidNotifications)
        {
            nlohmann::json notifications = nlohmann::json::array({
                {
                    {"notid", 1}
                }
            });

            ON_CALL(*mockApi, GetAllNotifications())
                .WillByDefault(Return(notifications));

            NotificationsRepository repo = Make(container);
            EXPECT_EQ(0, repo.Count());
        }

        TEST_F(NotificationsRepositoryFactoryTest, MakeHandlesExceptionFromRequestingUnreadNotifications)
        {
            ON_CALL(*mockApi, GetAllNotifications())
                .WillByDefault(Return(nlohmann::json::array()));

            ON_CALL(*mockApi, GetUnreadNotifications())
                .WillByDefault(Throw(ApiException("foo")));

            NotificationsRepository repo = Make(container);
            EXPECT_EQ(0, repo.Count());
        }

        TEST_F(NotificationsRepositoryFactoryTest, MakeLoadsNotificationsFromApi)
        {
            nlohmann::json notifications = nlohmann::json::array({
                {
                    {"id", 1},
                    {"title", "title"},
                    {"body", "body"},
                    {"valid_from", "2021-01-31 16:15:00"},
                    {"valid_to", "2021-01-31 16:20:00"},
                    {"controllers", nlohmann::json::array({"EGKK_DEL", "EGKK_TWR"})},
                },
                {
                    {"id", 2},
                    {"title", "title2"},
                    {"body", "body2"},
                    {"valid_from", "2021-01-31 16:20:00"},
                    {"valid_to", "2021-01-31 16:25:00"},
                    {"controllers", nlohmann::json::array({"EGKK_APP"})},
                }
            });

            ON_CALL(*mockApi, GetAllNotifications())
                .WillByDefault(Return(notifications));

            nlohmann::json unreadNotifications = nlohmann::json::array({
                {
                    {"id", 1}
                }
            });

            ON_CALL(*mockApi, GetUnreadNotifications())
                .WillByDefault(Return(unreadNotifications));

            NotificationsRepository repo = Make(container);
            EXPECT_EQ(2, repo.Count());

            Notification * const notification1 = repo.Get(1);
            EXPECT_EQ(1, notification1->Id());
            EXPECT_EQ("title", notification1->Title());
            EXPECT_EQ("body", notification1->Body());
            EXPECT_FALSE(notification1->IsRead());
            EXPECT_TRUE(notification1->IsRelevant(*position1));
            EXPECT_TRUE(notification1->IsRelevant(*position2));
            EXPECT_FALSE(notification1->IsRelevant(*position3));

            Notification * const notification2 = repo.Get(2);
            EXPECT_EQ(2, notification2->Id());
            EXPECT_EQ("title2", notification2->Title());
            EXPECT_EQ("body2", notification2->Body());
            EXPECT_TRUE(notification2->IsRead());
            EXPECT_FALSE(notification2->IsRelevant(*position1));
            EXPECT_FALSE(notification2->IsRelevant(*position2));
            EXPECT_TRUE(notification2->IsRelevant(*position3));
        }

        TEST_F(NotificationsRepositoryFactoryTest, MakeHandlesInvalidUnreadNotifications)
        {
            nlohmann::json notifications = nlohmann::json::array({
                {
                    {"id", 1},
                    {"title", "title"},
                    {"body", "body"},
                    {"valid_from", "2021-01-31 16:15:00"},
                    {"valid_to", "2021-01-31 16:20:00"},
                    {"controllers", nlohmann::json::array({"EGKK_DEL", "EGKK_TWR"})},
                },
                {
                    {"id", 2},
                    {"title", "title2"},
                    {"body", "body2"},
                    {"valid_from", "2021-01-31 16:20:00"},
                    {"valid_to", "2021-01-31 16:25:00"},
                    {"controllers", nlohmann::json::array({"EGKK_APP"})},
                }
            });

            ON_CALL(*mockApi, GetAllNotifications())
                .WillByDefault(Return(notifications));

            nlohmann::json unreadNotifications = nlohmann::json::object();

            ON_CALL(*mockApi, GetUnreadNotifications())
                .WillByDefault(Return(unreadNotifications));

            NotificationsRepository repo = Make(container);
            EXPECT_EQ(2, repo.Count());

            Notification * const notification = repo.Get(2);
            EXPECT_EQ(2, notification->Id());
            EXPECT_EQ("title2", notification->Title());
            EXPECT_EQ("body2", notification->Body());
            EXPECT_FALSE(notification->IsRead());
            EXPECT_FALSE(notification->IsRelevant(*position1));
            EXPECT_FALSE(notification->IsRelevant(*position2));
            EXPECT_TRUE(notification->IsRelevant(*position3));
        }
    }  // namespace Notifications
}  // namespace UKControllerPluginTest
