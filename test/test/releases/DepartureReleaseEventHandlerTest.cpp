#include "pch/pch.h"
#include "releases/DepartureReleaseEventHandler.h"
#include "releases/DepartureReleaseRequest.h"
#include "websocket/WebsocketSubscription.h"
#include "websocket/WebsocketMessage.h"
#include "time/ParseTimeStrings.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"

using ::testing::Test;
using UKControllerPlugin::Releases::DepartureReleaseEventHandler;
using UKControllerPlugin::Releases::DepartureReleaseRequest;
using UKControllerPlugin::Websocket::WebsocketMessage;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Time::ParseTimeString;

namespace UKControllerPluginTest {
    namespace Releases {

        class DepartureReleaseEventHandlerTest : public Test
        {
            public:
                DepartureReleaseEventHandlerTest()
                    : handler(controllers)
                {
                    request = std::make_shared<DepartureReleaseRequest>(
                        1,
                        "BAW123",
                        2,
                        3,
                        std::chrono::system_clock::now()
                    );
                    controllers.AddPosition(
                        std::make_shared<ControllerPosition>(
                            2,
                            "EGFF_APP",
                            125.850,
                            std::vector<std::string>{"EGGD", "EGFF"},
                            true,
                            false
                        )
                    );
                    controllers.AddPosition(
                        std::make_shared<ControllerPosition>(
                            3,
                            "EGFF_TWR",
                            123.450,
                            std::vector<std::string>{"EGGD", "EGFF"},
                            true,
                            false
                        )
                    );
                    handler.AddReleaseRequest(request);
                }

                ControllerPositionCollection controllers;
                std::shared_ptr<DepartureReleaseRequest> request;
                DepartureReleaseEventHandler handler;
        };

        TEST_F(DepartureReleaseEventHandlerTest, ItHasEventSubscriptions)
        {
            std::set<UKControllerPlugin::Websocket::WebsocketSubscription> expected = {
                {
                    UKControllerPlugin::Websocket::WebsocketSubscription::SUB_TYPE_CHANNEL,
                    "private-departure-releases"
                }
            };
            EXPECT_EQ(expected, handler.GetSubscriptions());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItAcknowledgesTheRequestFromMessage)
        {
            nlohmann::json data;
            data["id"] = 1;

            WebsocketMessage message{
                "departure_release.acknowledged",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_TRUE(request->Acknowledged());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesMissingIdInAcknowledgeMessage)
        {
            nlohmann::json data;
            data["not_id"] = 1;

            WebsocketMessage message{
                "departure_release.acknowledged",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_FALSE(request->Acknowledged());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesIdNotAnIntegerInAcknowledgeMessage)
        {
            nlohmann::json data;
            data["id"] = "abc";

            WebsocketMessage message{
                "departure_release.acknowledged",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_FALSE(request->Acknowledged());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesIdNotValidInAcknowledgeMessage)
        {
            nlohmann::json data;
            data["id"] = 2;

            WebsocketMessage message{
                "departure_release.acknowledged",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_FALSE(request->Acknowledged());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesAcknowledgeMessageNotAnObject)
        {
            nlohmann::json data = nlohmann::json::array();

            WebsocketMessage message{
                "departure_release.acknowledged",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_FALSE(request->Acknowledged());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItRejectsTheRequestFromMessage)
        {
            nlohmann::json data;
            data["id"] = 1;

            WebsocketMessage message{
                "departure_release.rejected",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_TRUE(request->Rejected());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesMissingIdInRejectedMessage)
        {
            nlohmann::json data;
            data["not_id"] = 1;

            WebsocketMessage message{
                "departure_release.rejected",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_FALSE(request->Rejected());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesIdNotAnIntegerInRejectedMessage)
        {
            nlohmann::json data;
            data["id"] = "abc";

            WebsocketMessage message{
                "departure_release.rejected",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_FALSE(request->Rejected());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesIdNotValidInRejectedMessage)
        {
            nlohmann::json data;
            data["id"] = 2;

            WebsocketMessage message{
                "departure_release.rejected",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_FALSE(request->Rejected());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesRejectedMessageNotAnObject)
        {
            nlohmann::json data = nlohmann::json::array();

            WebsocketMessage message{
                "departure_release.rejected",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_FALSE(request->Rejected());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItCancelsTheRequestFromMessage)
        {
            nlohmann::json data;
            data["id"] = 1;

            WebsocketMessage message{
                "departure_release.request_cancelled",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_EQ(nullptr, handler.GetReleaseRequest(1));
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesMissingIdInCancelMessage)
        {
            nlohmann::json data;
            data["not_id"] = 1;

            WebsocketMessage message{
                "departure_release.request_cancelled",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_NE(nullptr, handler.GetReleaseRequest(1));
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesIdNotAnIntegerInCancelMessage)
        {
            nlohmann::json data;
            data["id"] = "abc";

            WebsocketMessage message{
                "departure_release.request_cancelled",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_NE(nullptr, handler.GetReleaseRequest(1));
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesIdNotValidInCancelMessage)
        {
            nlohmann::json data;
            data["id"] = 2;

            WebsocketMessage message{
                "departure_release.request_cancelled",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_NE(nullptr, handler.GetReleaseRequest(1));
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesCancelMessageNotAnObject)
        {
            nlohmann::json data = nlohmann::json::array();

            WebsocketMessage message{
                "departure_release.request_cancelled",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_NE(nullptr, handler.GetReleaseRequest(1));
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItApprovesTheRequestFromMessage)
        {
            nlohmann::json data;
            data["id"] = 1;
            data["expires_at"] = "2021-05-12 20:00:00";
            data["released_at"] = "2021-05-12 19:55:00";

            WebsocketMessage message{
                "departure_release.approved",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_TRUE(request->Approved());
            EXPECT_EQ(ParseTimeString("2021-05-12 19:55:00"), request->ReleasedAtTime());
            EXPECT_EQ(ParseTimeString("2021-05-12 20:00:00"), request->ReleaseExpiryTime());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesMissingIdInApproveMessage)
        {
            nlohmann::json data;
            data["expires_at"] = "2021-05-12 20:00:00";
            data["released_at"] = "2021-05-12 19:55:00";

            WebsocketMessage message{
                "departure_release.approved",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_FALSE(request->Approved());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesIdNotIntegerInApproveMessage)
        {
            nlohmann::json data;
            data["id"] = "abc";
            data["expires_at"] = "2021-05-12 20:00:00";
            data["released_at"] = "2021-05-12 19:55:00";

            WebsocketMessage message{
                "departure_release.approved",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_FALSE(request->Approved());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesIdNotValidInApproveMessage)
        {
            nlohmann::json data;
            data["id"] = 2;
            data["expires_at"] = "2021-05-12 20:00:00";
            data["released_at"] = "2021-05-12 19:55:00";

            WebsocketMessage message{
                "departure_release.approved",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_FALSE(request->Approved());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesMissingExpiresAtInApproveMessage)
        {
            nlohmann::json data;
            data["id"] = 1;
            data["released_at"] = "2021-05-12 19:55:00";

            WebsocketMessage message{
                "departure_release.approved",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_FALSE(request->Approved());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesExpiresAtNotStringInApproveMessage)
        {
            nlohmann::json data;
            data["id"] = 1;
            data["expires_at"] = 123;
            data["released_at"] = "2021-05-12 19:55:00";

            WebsocketMessage message{
                "departure_release.approved",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_FALSE(request->Approved());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesExpiresAtNotValidTimeStampInApproveMessage)
        {
            nlohmann::json data;
            data["id"] = 1;
            data["expires_at"] = "abc";
            data["released_at"] = "2021-05-12 19:55:00";

            WebsocketMessage message{
                "departure_release.approved",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_FALSE(request->Approved());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesMissingIdReleasedAtTimeInApproveMessage)
        {
            nlohmann::json data;
            data["id"] = 1;
            data["expires_at"] = "2021-05-12 20:00:00";

            WebsocketMessage message{
                "departure_release.approved",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_FALSE(request->Approved());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesReleasedAtTimeNotStringInApproveMessage)
        {
            nlohmann::json data;
            data["id"] = 1;
            data["expires_at"] = "2021-05-12 20:00:00";
            data["released_at"] = 123;

            WebsocketMessage message{
                "departure_release.approved",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_FALSE(request->Approved());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesReleasedAtNotValidTimestampInApproveMessage)
        {
            nlohmann::json data;
            data["id"] = 1;
            data["expires_at"] = "2021-05-12 20:00:00";
            data["released_at"] = "abc";

            WebsocketMessage message{
                "departure_release.approved",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_FALSE(request->Approved());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesApproveMessageNotObject)
        {
            nlohmann::json data = nlohmann::json::array();

            WebsocketMessage message{
                "departure_release.approved",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_FALSE(request->Approved());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItCreatesAReleaseFromMessage)
        {
            nlohmann::json data;
            data["id"] = 2;
            data["callsign"] = "BAW123";
            data["requesting_controller"] = 2;
            data["target_controller"] = 3;
            data["expires_at"] = "2021-05-12 19:55:00";

            WebsocketMessage message{
                "departure_release.requested",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            auto release = handler.GetReleaseRequest(2);
            EXPECT_EQ(2, release->Id());
            EXPECT_EQ("BAW123", release->Callsign());
            EXPECT_EQ(2, release->RequestingController());
            EXPECT_EQ(3, release->TargetController());
            EXPECT_EQ(ParseTimeString("2021-05-12 19:55:00"), release->RequestExpiryTime());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItUpdatesAReleaseFromMessage)
        {
            nlohmann::json data;
            data["id"] = 1;
            data["callsign"] = "BAW123";
            data["requesting_controller"] = 2;
            data["target_controller"] = 3;
            data["expires_at"] = "2021-05-12 19:55:00";

            WebsocketMessage message{
                "departure_release.requested",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            auto release = handler.GetReleaseRequest(1);
            EXPECT_EQ(1, release->Id());
            EXPECT_EQ("BAW123", release->Callsign());
            EXPECT_EQ(2, release->RequestingController());
            EXPECT_EQ(3, release->TargetController());
            EXPECT_EQ(ParseTimeString("2021-05-12 19:55:00"), release->RequestExpiryTime());
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesMissingIdInCreateMessage)
        {
            nlohmann::json data;
            data["callsign"] = "BAW123";
            data["requesting_controller"] = 2;
            data["target_controller"] = 3;
            data["expires_at"] = "2021-05-12 19:55:00";

            WebsocketMessage message{
                "departure_release.requested",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_EQ(nullptr, handler.GetReleaseRequest(2));
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesIdNotIntegerInCreateMessage)
        {
            nlohmann::json data;
            data["id"] = "abc";
            data["callsign"] = "BAW123";
            data["requesting_controller"] = 2;
            data["target_controller"] = 3;
            data["expires_at"] = "2021-05-12 19:55:00";

            WebsocketMessage message{
                "departure_release.requested",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_EQ(nullptr, handler.GetReleaseRequest(2));
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesCallsignMissingInCreateMessage)
        {
            nlohmann::json data;
            data["id"] = 2;
            data["requesting_controller"] = 2;
            data["target_controller"] = 3;
            data["expires_at"] = "2021-05-12 19:55:00";

            WebsocketMessage message{
                "departure_release.approved",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_EQ(nullptr, handler.GetReleaseRequest(2));
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesCallsignNotAStringInCreateMessage)
        {
            nlohmann::json data;
            data["id"] = 2;
            data["callsign"] = 54;
            data["requesting_controller"] = 2;
            data["target_controller"] = 3;
            data["expires_at"] = "2021-05-12 19:55:00";

            WebsocketMessage message{
                "departure_release.approved",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_EQ(nullptr, handler.GetReleaseRequest(2));
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesRequestingControllerMissingInCreateMessage)
        {
            nlohmann::json data;
            data["id"] = 2;
            data["callsign"] = "BAW123";
            data["target_controller"] = 3;
            data["expires_at"] = "2021-05-12 19:55:00";

            WebsocketMessage message{
                "departure_release.approved",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_EQ(nullptr, handler.GetReleaseRequest(2));
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesRequestingControllerNotAnIntegerInCreateMessage)
        {
            nlohmann::json data;
            data["id"] = 2;
            data["callsign"] = "BAW123";
            data["requesting_controller"] = "abc";
            data["target_controller"] = 3;
            data["expires_at"] = "2021-05-12 19:55:00";

            WebsocketMessage message{
                "departure_release.approved",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_EQ(nullptr, handler.GetReleaseRequest(2));
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesRequestingControllerNotARealControllerInCreateMessage)
        {
            nlohmann::json data;
            data["id"] = 2;
            data["callsign"] = "BAW123";
            data["requesting_controller"] = 456;
            data["target_controller"] = 3;
            data["expires_at"] = "2021-05-12 19:55:00";

            WebsocketMessage message{
                "departure_release.approved",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_EQ(nullptr, handler.GetReleaseRequest(2));
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesTargetControllerMissingInCreateMessage)
        {
            nlohmann::json data;
            data["id"] = 2;
            data["callsign"] = "BAW123";
            data["requesting_controller"] = 2;
            data["expires_at"] = "2021-05-12 19:55:00";

            WebsocketMessage message{
                "departure_release.approved",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_EQ(nullptr, handler.GetReleaseRequest(2));
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesTargetControllerNotIntInCreateMessage)
        {
            nlohmann::json data;
            data["id"] = 2;
            data["callsign"] = "BAW123";
            data["requesting_controller"] = 2;
            data["target_controller"] = "abc";
            data["expires_at"] = "2021-05-12 19:55:00";

            WebsocketMessage message{
                "departure_release.approved",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_EQ(nullptr, handler.GetReleaseRequest(2));
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesTargetControllerNotARealControllerInCreateMessage)
        {
            nlohmann::json data;
            data["id"] = 2;
            data["callsign"] = "BAW123";
            data["requesting_controller"] = 2;
            data["target_controller"] = 456;
            data["expires_at"] = "2021-05-12 19:55:00";

            WebsocketMessage message{
                "departure_release.approved",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_EQ(nullptr, handler.GetReleaseRequest(2));
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesExpiresAtMissingInCreateMessage)
        {
            nlohmann::json data;
            data["id"] = 2;
            data["callsign"] = "BAW123";
            data["requesting_controller"] = 2;
            data["target_controller"] = 3;

            WebsocketMessage message{
                "departure_release.approved",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_EQ(nullptr, handler.GetReleaseRequest(2));
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesExpiresAtNotStringInCreateMessage)
        {
            nlohmann::json data;
            data["id"] = 2;
            data["callsign"] = "BAW123";
            data["requesting_controller"] = 2;
            data["target_controller"] = 3;
            data["expires_at"] = 123;

            WebsocketMessage message{
                "departure_release.approved",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_EQ(nullptr, handler.GetReleaseRequest(2));
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItHandlesExpiresAtNotValidTimestampInCreateMessage)
        {
            nlohmann::json data;
            data["id"] = 2;
            data["callsign"] = "BAW123";
            data["requesting_controller"] = 2;
            data["target_controller"] = 3;
            data["expires_at"] = "abc";

            WebsocketMessage message{
                "departure_release.approved",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_EQ(nullptr, handler.GetReleaseRequest(2));
        }

        TEST_F(DepartureReleaseEventHandlerTest, ItNotObjectInCreateMessage)
        {
            nlohmann::json data = nlohmann::json::array();

            WebsocketMessage message{
                "departure_release.approved",
                "private-departure-releases",
                data
            };

            handler.ProcessWebsocketMessage(message);
            EXPECT_EQ(nullptr, handler.GetReleaseRequest(2));
        }
    }  // namespace Releases
}  // namespace UKControllerPluginTest
