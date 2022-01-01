#include "api/ApiException.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "dialog/DialogManager.h"
#include "releases/DepartureReleaseColours.h"
#include "releases/DepartureReleaseEventHandler.h"
#include "releases/DepartureReleaseRequest.h"
#include "releases/DepartureReleaseRequestView.h"
#include "time/ParseTimeStrings.h"
#include "time/SystemClock.h"
#include "tag/TagData.h"

using testing::NiceMock;
using testing::Test;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Push::PushEvent;
using UKControllerPlugin::Releases::CompareDepartureReleases;
using UKControllerPlugin::Releases::DepartureReleaseEventHandler;
using UKControllerPlugin::Releases::DepartureReleaseRequest;
using UKControllerPlugin::Time::ParseTimeString;
using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPluginTest::Releases {

    class DepartureReleaseEventHandlerTest : public Test
    {
        public:
        DepartureReleaseEventHandlerTest()
            : dialogManager(dialogProvider),
              handler(api, mockTaskRunner, mockPlugin, controllers, activeCallsigns, dialogManager, windows, 3, 4)
        {
            request = std::make_shared<DepartureReleaseRequest>(
                1, "BAW123", 3, 2, std::chrono::system_clock::now() + std::chrono::minutes(5));

            // Add position 1
            position1 = std::make_shared<ControllerPosition>(
                2, "EGFF_APP", 125.850, std::vector<std::string>{"EGGD", "EGFF"}, true, true);
            controllers.AddPosition(position1);
            controller1Callsign = std::make_shared<ActiveCallsign>("EGFF_APP", "Test 1", *position1, false);

            // Add position 2
            position2 = std::make_shared<ControllerPosition>(
                3, "EGFF_TWR", 123.450, std::vector<std::string>{"EGGD", "EGFF"}, false, false);
            controllers.AddPosition(position2);
            controller2Callsign = std::make_shared<ActiveCallsign>("EGFF_1_TWR", "Test 2", *position2, false);

            // Add position 3
            position3 = std::make_shared<ControllerPosition>(
                4, "LON_W_CTR", 126.020, std::vector<std::string>{"EGGD", "EGFF"}, true, true);
            controllers.AddPosition(position3);

            ON_CALL(this->mockFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));

            this->pluginReturnedFlightplan = std::make_shared<NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(*this->pluginReturnedFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));

            UKControllerPlugin::Time::SetTestNow(std::chrono::system_clock::now());

            this->dialogManager.AddDialog(this->dialogDataRequest);
            this->dialogManager.AddDialog(this->dialogDataApprove);
            this->dialogManager.AddDialog(this->dialogDataReject);
        }

        UKControllerPlugin::Tag::TagData GetTagData(int tagItemid)
        {
            return UKControllerPlugin::Tag::TagData(
                this->mockFlightplan,
                this->mockRadarTarget,
                tagItemid,
                EuroScopePlugIn::TAG_DATA_CORRELATED,
                this->itemString,
                &this->euroscopeColourCode,
                &this->tagColour,
                &this->fontSize);
        }

        double fontSize = 24.1;
        COLORREF tagColour = RGB(255, 255, 255);
        int euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_ASSUMED;
        char itemString[16] = "";
        DialogData dialogDataRequest = {IDD_DEPARTURE_RELEASE_REQUEST, "", nullptr, NULL, nullptr};
        DialogData dialogDataApprove = {IDD_DEPARTURE_RELEASE_APPROVE, "", nullptr, NULL, nullptr};
        DialogData dialogDataReject = {IDD_DEPARTURE_RELEASE_REJECT, "", nullptr, NULL, nullptr};
        NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> mockFlightplan;
        NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface> mockRadarTarget;
        std::shared_ptr<NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> pluginReturnedFlightplan;
        NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> mockPlugin;
        std::shared_ptr<ControllerPosition> position1;
        std::shared_ptr<ControllerPosition> position2;
        std::shared_ptr<ControllerPosition> position3;
        std::shared_ptr<ActiveCallsign> controller1Callsign;
        std::shared_ptr<ActiveCallsign> controller2Callsign;
        UKControllerPlugin::Controller::ActiveCallsignCollection activeCallsigns;
        TaskManager::MockTaskRunnerInterface mockTaskRunner;
        NiceMock<Dialog::MockDialogProvider> dialogProvider;
        NiceMock<Api::MockApiInterface> api;
        NiceMock<Windows::MockWinApi> windows;
        UKControllerPlugin::Dialog::DialogManager dialogManager;
        ControllerPositionCollection controllers;
        std::shared_ptr<DepartureReleaseRequest> request;
        DepartureReleaseEventHandler handler;
    };

    TEST_F(DepartureReleaseEventHandlerTest, ItHasEventSubscriptions)
    {
        std::set<UKControllerPlugin::Push::PushEventSubscription> expected = {
            {UKControllerPlugin::Push::PushEventSubscription::SUB_TYPE_CHANNEL, "private-departure-releases"}};
        EXPECT_EQ(expected, handler.GetPushEventSubscriptions());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItAcknowledgesTheRequestFromMessage)
    {
        handler.AddReleaseRequest(request);
        nlohmann::json data;
        data["id"] = 1;

        PushEvent message{"departure_release.acknowledged", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_TRUE(request->Acknowledged());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesMissingIdInAcknowledgeMessage)
    {
        nlohmann::json data;
        data["not_id"] = 1;

        PushEvent message{"departure_release.acknowledged", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_FALSE(request->Acknowledged());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesIdNotAnIntegerInAcknowledgeMessage)
    {
        nlohmann::json data;
        data["id"] = "abc";

        PushEvent message{"departure_release.acknowledged", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_FALSE(request->Acknowledged());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesIdNotValidInAcknowledgeMessage)
    {
        nlohmann::json data;
        data["id"] = 2;

        PushEvent message{"departure_release.acknowledged", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_FALSE(request->Acknowledged());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesAcknowledgeMessageNotAnObject)
    {
        nlohmann::json data = nlohmann::json::array();

        PushEvent message{"departure_release.acknowledged", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_FALSE(request->Acknowledged());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItRejectsTheRequestFromMessage)
    {
        handler.AddReleaseRequest(request);
        nlohmann::json data;
        data["id"] = 1;
        data["remarks"] = "Some remarks";

        PushEvent message{"departure_release.rejected", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_TRUE(request->Rejected());
        EXPECT_EQ("Some remarks", request->Remarks());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntPlayASoundOnRejectionIfUserNotActive)
    {
        handler.AddReleaseRequest(request);
        nlohmann::json data;
        data["id"] = 1;
        data["remarks"] = "Some remarks";

        PushEvent message{"departure_release.rejected", "private-departure-releases", data};

        EXPECT_CALL(this->windows, PlayWave(testing::_)).Times(0);

        handler.ProcessPushEvent(message);
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntPlayASoundOnRejectionIfUserIsNotRequestingController)
    {
        activeCallsigns.AddUserCallsign(*controller1Callsign);
        handler.AddReleaseRequest(request);
        nlohmann::json data;
        data["id"] = 1;
        data["remarks"] = "Some remarks";

        PushEvent message{"departure_release.rejected", "private-departure-releases", data};

        EXPECT_CALL(this->windows, PlayWave(testing::_)).Times(0);

        handler.ProcessPushEvent(message);
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItPlaysASoundOnRejectionIfUserIsRequestingController)
    {
        activeCallsigns.AddUserCallsign(*controller2Callsign);
        handler.AddReleaseRequest(request);
        nlohmann::json data;
        data["id"] = 1;
        data["remarks"] = "Some remarks";

        PushEvent message{"departure_release.rejected", "private-departure-releases", data};

        EXPECT_CALL(this->windows, PlayWave(MAKEINTRESOURCE(WAVE_DEP_RLS_REJ))).Times(1);

        handler.ProcessPushEvent(message);
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesMissingIdInRejectedMessage)
    {
        nlohmann::json data;
        data["not_id"] = 1;
        data["remarks"] = "Some remarks";

        PushEvent message{"departure_release.rejected", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_FALSE(request->Rejected());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesIdNotAnIntegerInRejectedMessage)
    {
        nlohmann::json data;
        data["id"] = "abc";
        data["remarks"] = "Some remarks";

        PushEvent message{"departure_release.rejected", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_FALSE(request->Rejected());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesIdNotValidInRejectedMessage)
    {
        nlohmann::json data;
        data["id"] = 2;
        data["remarks"] = "Some remarks";

        PushEvent message{"departure_release.rejected", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_FALSE(request->Rejected());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesRemarksMissingInRejectedMessage)
    {
        nlohmann::json data;
        data["id"] = 1;
        data["not_remarks"] = "Some remarks";

        PushEvent message{"departure_release.rejected", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_FALSE(request->Rejected());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesRemarksNotStringInRejectedMessage)
    {
        nlohmann::json data;
        data["id"] = 1;
        data["remarks"] = 123;

        PushEvent message{"departure_release.rejected", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_FALSE(request->Rejected());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesRejectedMessageNotAnObject)
    {
        nlohmann::json data = nlohmann::json::array();

        PushEvent message{"departure_release.rejected", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_FALSE(request->Rejected());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItCancelsTheRequestFromMessage)
    {
        handler.AddReleaseRequest(request);
        nlohmann::json data;
        data["id"] = 1;

        PushEvent message{"departure_release.request_cancelled", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_EQ(nullptr, handler.GetReleaseRequest(1));
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesMissingIdInCancelMessage)
    {
        handler.AddReleaseRequest(request);
        nlohmann::json data;
        data["not_id"] = 1;

        PushEvent message{"departure_release.request_cancelled", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_NE(nullptr, handler.GetReleaseRequest(1));
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesIdNotAnIntegerInCancelMessage)
    {
        handler.AddReleaseRequest(request);
        nlohmann::json data;
        data["id"] = "abc";

        PushEvent message{"departure_release.request_cancelled", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_NE(nullptr, handler.GetReleaseRequest(1));
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesIdNotValidInCancelMessage)
    {
        handler.AddReleaseRequest(request);
        nlohmann::json data;
        data["id"] = 2;

        PushEvent message{"departure_release.request_cancelled", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_NE(nullptr, handler.GetReleaseRequest(1));
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesCancelMessageNotAnObject)
    {
        handler.AddReleaseRequest(request);
        nlohmann::json data = nlohmann::json::array();

        PushEvent message{"departure_release.request_cancelled", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_NE(nullptr, handler.GetReleaseRequest(1));
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItApprovesTheRequestFromMessage)
    {
        handler.AddReleaseRequest(request);
        nlohmann::json data;
        data["id"] = 1;
        data["expires_at"] = "2021-05-12 20:00:00";
        data["released_at"] = "2021-05-12 19:55:00";
        data["remarks"] = "remarks";

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_TRUE(request->Approved());
        EXPECT_FALSE(request->ApprovedWithNoExpiry());
        EXPECT_EQ(ParseTimeString("2021-05-12 19:55:00"), request->ReleasedAtTime());
        EXPECT_EQ(ParseTimeString("2021-05-12 20:00:00"), request->ReleaseExpiryTime());
        EXPECT_EQ("remarks", request->Remarks());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItApprovesTheRequestFromMessageWithNoExpiry)
    {
        handler.AddReleaseRequest(request);
        nlohmann::json data;
        data["id"] = 1;
        data["expires_at"] = nlohmann::json::value_t::null;
        data["released_at"] = "2021-05-12 19:55:00";
        data["remarks"] = "remarks";

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_TRUE(request->ApprovedWithNoExpiry());
        EXPECT_EQ(ParseTimeString("2021-05-12 19:55:00"), request->ReleasedAtTime());
        EXPECT_EQ("remarks", request->Remarks());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntPlayASoundOnApproveIfUserNotActive)
    {
        handler.AddReleaseRequest(request);
        nlohmann::json data;
        data["id"] = 1;
        data["expires_at"] = "2021-05-12 20:00:00";
        data["released_at"] = "2021-05-12 19:55:00";
        data["remarks"] = "remarks";

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        EXPECT_CALL(this->windows, PlayWave(testing::_)).Times(0);

        handler.ProcessPushEvent(message);
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntPlayASoundOnApproveIfUserNotRequestingController)
    {
        activeCallsigns.AddUserCallsign(*controller1Callsign);
        handler.AddReleaseRequest(request);
        nlohmann::json data;
        data["id"] = 1;
        data["expires_at"] = "2021-05-12 20:00:00";
        data["released_at"] = "2021-05-12 19:55:00";
        data["remarks"] = "remarks";

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        EXPECT_CALL(this->windows, PlayWave(testing::_)).Times(0);

        handler.ProcessPushEvent(message);
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItPlaysASoundOnApproveIfUserRequestingController)
    {
        activeCallsigns.AddUserCallsign(*controller2Callsign);
        handler.AddReleaseRequest(request);
        nlohmann::json data;
        data["id"] = 1;
        data["expires_at"] = "2021-05-12 20:00:00";
        data["released_at"] = "2021-05-12 19:55:00";
        data["remarks"] = "remarks";

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        EXPECT_CALL(this->windows, PlayWave(MAKEINTRESOURCE(WAVE_DEP_RLS_ACCEPT))).Times(1);

        handler.ProcessPushEvent(message);
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesMissingIdInApproveMessage)
    {
        nlohmann::json data;
        data["expires_at"] = "2021-05-12 20:00:00";
        data["released_at"] = "2021-05-12 19:55:00";
        data["remarks"] = "remarks";

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_FALSE(request->Approved());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesIdNotIntegerInApproveMessage)
    {
        nlohmann::json data;
        data["id"] = "abc";
        data["expires_at"] = "2021-05-12 20:00:00";
        data["released_at"] = "2021-05-12 19:55:00";
        data["remarks"] = "remarks";

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_FALSE(request->Approved());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesIdNotValidInApproveMessage)
    {
        nlohmann::json data;
        data["id"] = 2;
        data["expires_at"] = "2021-05-12 20:00:00";
        data["released_at"] = "2021-05-12 19:55:00";
        data["remarks"] = "remarks";

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_FALSE(request->Approved());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesMissingExpiresAtInApproveMessage)
    {
        nlohmann::json data;
        data["id"] = 1;
        data["released_at"] = "2021-05-12 19:55:00";
        data["remarks"] = "remarks";

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_FALSE(request->Approved());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesExpiresAtNotStringInApproveMessage)
    {
        nlohmann::json data;
        data["id"] = 1;
        data["expires_at"] = 123;
        data["released_at"] = "2021-05-12 19:55:00";
        data["remarks"] = "remarks";

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_FALSE(request->Approved());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesExpiresAtNotValidTimeStampInApproveMessage)
    {
        nlohmann::json data;
        data["id"] = 1;
        data["expires_at"] = "abc";
        data["released_at"] = "2021-05-12 19:55:00";
        data["remarks"] = "remarks";

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_FALSE(request->Approved());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesMissingIdReleasedAtTimeInApproveMessage)
    {
        nlohmann::json data;
        data["id"] = 1;
        data["expires_at"] = "2021-05-12 20:00:00";
        data["remarks"] = "remarks";

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_FALSE(request->Approved());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesReleasedAtTimeNotStringInApproveMessage)
    {
        nlohmann::json data;
        data["id"] = 1;
        data["expires_at"] = "2021-05-12 20:00:00";
        data["released_at"] = 123;
        data["remarks"] = "remarks";

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_FALSE(request->Approved());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesReleasedAtNotValidTimestampInApproveMessage)
    {
        nlohmann::json data;
        data["id"] = 1;
        data["expires_at"] = "2021-05-12 20:00:00";
        data["released_at"] = "abc";
        data["remarks"] = "remarks";

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_FALSE(request->Approved());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesMissingRemarksInApproveMessage)
    {
        nlohmann::json data;
        data["id"] = 1;
        data["expires_at"] = "2021-05-12 20:00:00";
        data["released_at"] = "2021-05-12 20:00:00";
        data["not_remarks"] = "remarks";

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_FALSE(request->Approved());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesRemarksNotStringInApproveMessage)
    {
        nlohmann::json data;
        data["id"] = 1;
        data["expires_at"] = "2021-05-12 20:00:00";
        data["released_at"] = "2021-05-12 20:00:00";
        data["remarks"] = 123;

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_FALSE(request->Approved());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesApproveMessageNotObject)
    {
        nlohmann::json data = nlohmann::json::array();

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
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

        PushEvent message{"departure_release.requested", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        auto release = handler.GetReleaseRequest(2);
        EXPECT_EQ(2, release->Id());
        EXPECT_EQ("BAW123", release->Callsign());
        EXPECT_EQ(2, release->RequestingController());
        EXPECT_EQ(3, release->TargetController());
        EXPECT_EQ(ParseTimeString("2021-05-12 19:55:00"), release->RequestExpiryTime());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntPlaySoundOnRequestIfUserNotActive)
    {
        nlohmann::json data;
        data["id"] = 2;
        data["callsign"] = "BAW123";
        data["requesting_controller"] = 2;
        data["target_controller"] = 3;
        data["expires_at"] = "2021-05-12 19:55:00";

        PushEvent message{"departure_release.requested", "private-departure-releases", data};

        EXPECT_CALL(this->windows, PlayWave(testing::_)).Times(0);

        handler.ProcessPushEvent(message);
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntPlaySoundOnRequestIfUserNotTargetController)
    {
        activeCallsigns.AddUserCallsign(*controller1Callsign);
        nlohmann::json data;
        data["id"] = 2;
        data["callsign"] = "BAW123";
        data["requesting_controller"] = 2;
        data["target_controller"] = 3;
        data["expires_at"] = "2021-05-12 19:55:00";

        PushEvent message{"departure_release.requested", "private-departure-releases", data};

        EXPECT_CALL(this->windows, PlayWave(testing::_)).Times(0);

        handler.ProcessPushEvent(message);
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItPlaysSoundOnRequestIfUserIsTargetController)
    {
        activeCallsigns.AddUserCallsign(*controller2Callsign);
        nlohmann::json data;
        data["id"] = 2;
        data["callsign"] = "BAW123";
        data["requesting_controller"] = 2;
        data["target_controller"] = 3;
        data["expires_at"] = "2021-05-12 19:55:00";

        PushEvent message{"departure_release.requested", "private-departure-releases", data};

        EXPECT_CALL(this->windows, PlayWave(MAKEINTRESOURCE(WAVE_DEP_RLS_REQ))).Times(1);

        handler.ProcessPushEvent(message);
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItUpdatesAReleaseFromMessage)
    {
        nlohmann::json data;
        data["id"] = 1;
        data["callsign"] = "BAW123";
        data["requesting_controller"] = 2;
        data["target_controller"] = 3;
        data["expires_at"] = "2021-05-12 19:55:00";

        PushEvent message{"departure_release.requested", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        auto release = handler.GetReleaseRequest(1);
        EXPECT_EQ(1, release->Id());
        EXPECT_EQ("BAW123", release->Callsign());
        EXPECT_EQ(2, release->RequestingController());
        EXPECT_EQ(3, release->TargetController());
        EXPECT_EQ(ParseTimeString("2021-05-12 19:55:00"), release->RequestExpiryTime());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItRemovesReleasesForSameControllerAndCallsignOnRequest)
    {
        nlohmann::json data;
        data["id"] = 2;
        data["callsign"] = "BAW123";
        data["requesting_controller"] = 3;
        data["target_controller"] = 2;
        data["expires_at"] = "2021-05-12 19:55:00";

        PushEvent message{"departure_release.requested", "private-departure-releases", data};
        handler.AddReleaseRequest(this->request);

        handler.ProcessPushEvent(message);
        EXPECT_EQ(nullptr, handler.GetReleaseRequest(1));
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntRemoveReleaseOnRequestIfDifferentTargetController)
    {
        nlohmann::json data;
        data["id"] = 2;
        data["callsign"] = "BAW123";
        data["requesting_controller"] = 2;
        data["target_controller"] = 3;
        data["expires_at"] = "2021-05-12 19:55:00";

        PushEvent message{"departure_release.requested", "private-departure-releases", data};
        handler.AddReleaseRequest(this->request);

        handler.ProcessPushEvent(message);
        EXPECT_NE(nullptr, handler.GetReleaseRequest(1));
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntRemoveReleaseOnRequestIfDifferentCallsign)
    {
        nlohmann::json data;
        data["id"] = 2;
        data["callsign"] = "BAW456";
        data["requesting_controller"] = 3;
        data["target_controller"] = 2;
        data["expires_at"] = "2021-05-12 19:55:00";

        PushEvent message{"departure_release.requested", "private-departure-releases", data};
        handler.AddReleaseRequest(this->request);

        handler.ProcessPushEvent(message);
        EXPECT_NE(nullptr, handler.GetReleaseRequest(1));
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesMissingIdInCreateMessage)
    {
        nlohmann::json data;
        data["callsign"] = "BAW123";
        data["requesting_controller"] = 2;
        data["target_controller"] = 3;
        data["expires_at"] = "2021-05-12 19:55:00";

        PushEvent message{"departure_release.requested", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
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

        PushEvent message{"departure_release.requested", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_EQ(nullptr, handler.GetReleaseRequest(2));
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesCallsignMissingInCreateMessage)
    {
        nlohmann::json data;
        data["id"] = 2;
        data["requesting_controller"] = 2;
        data["target_controller"] = 3;
        data["expires_at"] = "2021-05-12 19:55:00";

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
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

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_EQ(nullptr, handler.GetReleaseRequest(2));
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesRequestingControllerMissingInCreateMessage)
    {
        nlohmann::json data;
        data["id"] = 2;
        data["callsign"] = "BAW123";
        data["target_controller"] = 3;
        data["expires_at"] = "2021-05-12 19:55:00";

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
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

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
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

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_EQ(nullptr, handler.GetReleaseRequest(2));
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesTargetControllerMissingInCreateMessage)
    {
        nlohmann::json data;
        data["id"] = 2;
        data["callsign"] = "BAW123";
        data["requesting_controller"] = 2;
        data["expires_at"] = "2021-05-12 19:55:00";

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
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

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
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

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_EQ(nullptr, handler.GetReleaseRequest(2));
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesExpiresAtMissingInCreateMessage)
    {
        nlohmann::json data;
        data["id"] = 2;
        data["callsign"] = "BAW123";
        data["requesting_controller"] = 2;
        data["target_controller"] = 3;

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
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

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
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

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_EQ(nullptr, handler.GetReleaseRequest(2));
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItNotObjectInCreateMessage)
    {
        nlohmann::json data = nlohmann::json::array();

        PushEvent message{"departure_release.approved", "private-departure-releases", data};

        handler.ProcessPushEvent(message);
        EXPECT_EQ(nullptr, handler.GetReleaseRequest(2));
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntRemovePendingReleases)
    {
        std::shared_ptr<DepartureReleaseRequest> relevantRelease =
            std::make_shared<DepartureReleaseRequest>(4, "BAW999", 5, 6, TimeNow() + std::chrono::minutes(5));
        handler.AddReleaseRequest(relevantRelease);
        handler.TimedEventTrigger();
        EXPECT_EQ(relevantRelease, handler.GetReleaseRequest(4));
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItRemovesPendingReleasesThatHaveExpired)
    {
        std::shared_ptr<DepartureReleaseRequest> relevantRelease =
            std::make_shared<DepartureReleaseRequest>(4, "BAW999", 5, 6, TimeNow() - std::chrono::seconds(5));
        handler.AddReleaseRequest(relevantRelease);
        handler.TimedEventTrigger();
        EXPECT_EQ(nullptr, handler.GetReleaseRequest(4));
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntRemoveRecentlyExpiredApprovals)
    {
        std::shared_ptr<DepartureReleaseRequest> relevantRelease =
            std::make_shared<DepartureReleaseRequest>(4, "BAW999", 5, 6, TimeNow() - std::chrono::seconds(1));
        relevantRelease->Approve(std::chrono::system_clock::now(), TimeNow() - std::chrono::seconds(80), "remarks");
        handler.AddReleaseRequest(relevantRelease);
        handler.TimedEventTrigger();
        EXPECT_EQ(relevantRelease, handler.GetReleaseRequest(4));
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItExpiresRemovesThatHaveApprovalExpired)
    {
        std::shared_ptr<DepartureReleaseRequest> relevantRelease =
            std::make_shared<DepartureReleaseRequest>(4, "BAW999", 5, 6, TimeNow() - std::chrono::seconds(1));
        relevantRelease->Approve(std::chrono::system_clock::now(), TimeNow() - std::chrono::seconds(91), "remarks");
        handler.AddReleaseRequest(relevantRelease);
        handler.TimedEventTrigger();
        EXPECT_EQ(nullptr, handler.GetReleaseRequest(4));
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntRemoveRecentlyRejectedReleases)
    {
        std::shared_ptr<DepartureReleaseRequest> relevantRelease =
            std::make_shared<DepartureReleaseRequest>(4, "BAW999", 5, 6, TimeNow() - std::chrono::seconds(1));
        relevantRelease->Reject("remarks");
        handler.AddReleaseRequest(relevantRelease);
        handler.TimedEventTrigger();
        EXPECT_EQ(relevantRelease, handler.GetReleaseRequest(4));
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItExpiresReleasesThatHaveRejectionExpired)
    {
        std::shared_ptr<DepartureReleaseRequest> relevantRelease =
            std::make_shared<DepartureReleaseRequest>(4, "BAW999", 5, 6, TimeNow() + std::chrono::seconds(5));
        relevantRelease->Approve(std::chrono::system_clock::now(), TimeNow() - std::chrono::seconds(91), "remarks");
        handler.AddReleaseRequest(relevantRelease);
        handler.TimedEventTrigger();
        EXPECT_EQ(nullptr, handler.GetReleaseRequest(4));
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntOpenRequestDialogIfUserNotActive)
    {
        EXPECT_CALL(this->dialogProvider, OpenDialog(testing::_, testing::_)).Times(0);

        handler.OpenRequestDialog(this->mockFlightplan, this->mockRadarTarget, "", {});
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntOpenRequestDialogIfUserCantRequestReleases)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller2Callsign);
        EXPECT_CALL(this->dialogProvider, OpenDialog(testing::_, testing::_)).Times(0);

        handler.OpenRequestDialog(this->mockFlightplan, this->mockRadarTarget, "", {});
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItOpensRequestDialog)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);
        EXPECT_CALL(this->dialogProvider, OpenDialog(this->dialogDataRequest, testing::_)).Times(1);

        handler.OpenRequestDialog(this->mockFlightplan, this->mockRadarTarget, "", {});
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntOpenDecisionMenuIfNoReleasePresentForCallsign)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);
        EXPECT_CALL(this->mockPlugin, TriggerPopupList(testing::_, testing::_, testing::_)).Times(0);

        EXPECT_CALL(this->mockPlugin, SetEuroscopeSelectedFlightplanReference(testing::_)).Times(0);

        handler.OpenDecisionMenu(this->mockFlightplan, this->mockRadarTarget, "", {});
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntOpenDecisionMenuIfReleaseForCallsignAlreadyActioned)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);
        this->handler.AddReleaseRequest(this->request);
        this->request->Reject("remarks");

        EXPECT_CALL(this->mockPlugin, TriggerPopupList(testing::_, testing::_, testing::_)).Times(0);

        EXPECT_CALL(this->mockPlugin, SetEuroscopeSelectedFlightplanReference(testing::_)).Times(0);

        handler.OpenDecisionMenu(this->mockFlightplan, this->mockRadarTarget, "", {});
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntOpenDecisionMenuIfNoUserCallsign)
    {
        this->handler.AddReleaseRequest(this->request);

        EXPECT_CALL(this->mockPlugin, TriggerPopupList(testing::_, testing::_, testing::_)).Times(0);

        EXPECT_CALL(this->mockPlugin, SetEuroscopeSelectedFlightplanReference(testing::_)).Times(0);

        handler.OpenDecisionMenu(this->mockFlightplan, this->mockRadarTarget, "", {});
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntOpenDecisionMenuIfUserCallsignCannotActionReleases)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller2Callsign);
        this->handler.AddReleaseRequest(this->request);

        EXPECT_CALL(this->mockPlugin, TriggerPopupList(testing::_, testing::_, testing::_)).Times(0);

        EXPECT_CALL(this->mockPlugin, SetEuroscopeSelectedFlightplanReference(testing::_)).Times(0);

        handler.OpenDecisionMenu(this->mockFlightplan, this->mockRadarTarget, "", {});
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItOpensDecisionMenu)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);
        this->handler.AddReleaseRequest(this->request);

        EXPECT_CALL(this->mockPlugin, SetEuroscopeSelectedFlightplanReference(testing::Ref(this->mockFlightplan)))
            .Times(1);

        UKControllerPlugin::Plugin::PopupMenuItem menuItemApprove;
        menuItemApprove.firstValue = "Approve";
        menuItemApprove.secondValue = "";
        menuItemApprove.callbackFunctionId = 3;
        menuItemApprove.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        menuItemApprove.disabled = false;
        menuItemApprove.fixedPosition = false;

        UKControllerPlugin::Plugin::PopupMenuItem menuItemReject;
        menuItemReject.firstValue = "Reject";
        menuItemReject.secondValue = "";
        menuItemReject.callbackFunctionId = 3;
        menuItemReject.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        menuItemReject.disabled = false;
        menuItemReject.fixedPosition = false;

        UKControllerPlugin::Plugin::PopupMenuItem menuItemAcknowledge;
        menuItemAcknowledge.firstValue = "Acknowledge";
        menuItemAcknowledge.secondValue = "";
        menuItemAcknowledge.callbackFunctionId = 3;
        menuItemAcknowledge.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        menuItemAcknowledge.disabled = false;
        menuItemAcknowledge.fixedPosition = false;

        RECT expectedArea = {0, 0, 400, 600};
        EXPECT_CALL(this->mockPlugin, TriggerPopupList(RectEq(expectedArea), "Departure Release Decision", 1)).Times(1);

        EXPECT_CALL(this->mockPlugin, AddItemToPopupList(menuItemApprove)).Times(1);

        EXPECT_CALL(this->mockPlugin, AddItemToPopupList(menuItemReject)).Times(1);

        EXPECT_CALL(this->mockPlugin, AddItemToPopupList(menuItemAcknowledge)).Times(1);

        handler.OpenDecisionMenu(this->mockFlightplan, this->mockRadarTarget, "", {0, 0});
    }

    TEST_F(DepartureReleaseEventHandlerTest, ReleaseDecisionMadeDoesNothingIfPluginDoesntReturnFlightplan)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);
        this->handler.AddReleaseRequest(this->request);

        ON_CALL(this->mockPlugin, GetSelectedFlightplan).WillByDefault(testing::Return(nullptr));

        EXPECT_CALL(this->dialogProvider, OpenDialog(this->dialogDataApprove, testing::_)).Times(0);

        handler.ReleaseDecisionMade(1, "Approve", {});
    }

    TEST_F(DepartureReleaseEventHandlerTest, ReleaseDecisionMadeDoesNothingIfNoReleaseToProcess)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);

        ON_CALL(this->mockPlugin, GetSelectedFlightplan).WillByDefault(Return(this->pluginReturnedFlightplan));

        EXPECT_CALL(this->dialogProvider, OpenDialog(this->dialogDataApprove, testing::_)).Times(0);

        handler.ReleaseDecisionMade(1, "Approve", {});
    }

    TEST_F(DepartureReleaseEventHandlerTest, ReleaseDecisionMadeDoesNothingIfNoUserCallsign)
    {
        this->handler.AddReleaseRequest(this->request);

        ON_CALL(this->mockPlugin, GetSelectedFlightplan).WillByDefault(Return(this->pluginReturnedFlightplan));

        EXPECT_CALL(this->dialogProvider, OpenDialog(this->dialogDataApprove, testing::_)).Times(0);

        handler.ReleaseDecisionMade(1, "Approve", {});
    }

    TEST_F(DepartureReleaseEventHandlerTest, ReleaseDecisionMadeDoesNothingIfUserCannotDecide)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller2Callsign);
        this->handler.AddReleaseRequest(this->request);

        ON_CALL(this->mockPlugin, GetSelectedFlightplan).WillByDefault(Return(this->pluginReturnedFlightplan));

        EXPECT_CALL(this->dialogProvider, OpenDialog(this->dialogDataApprove, testing::_)).Times(0);

        handler.ReleaseDecisionMade(1, "Approve", {});
    }

    TEST_F(DepartureReleaseEventHandlerTest, ReleaseDecisionMadeOpensApprovalDialog)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);
        this->handler.AddReleaseRequest(this->request);

        ON_CALL(this->mockPlugin, GetSelectedFlightplan).WillByDefault(Return(this->pluginReturnedFlightplan));

        EXPECT_CALL(this->dialogProvider, OpenDialog(this->dialogDataApprove, testing::_)).Times(1);

        handler.ReleaseDecisionMade(1, "Approve", {});
    }

    TEST_F(DepartureReleaseEventHandlerTest, ReleaseDecisionMadeOpensRejectionDialog)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);
        this->handler.AddReleaseRequest(this->request);

        ON_CALL(this->mockPlugin, GetSelectedFlightplan).WillByDefault(Return(this->pluginReturnedFlightplan));

        EXPECT_CALL(this->dialogProvider, OpenDialog(this->dialogDataReject, testing::_)).Times(1);

        handler.ReleaseDecisionMade(1, "Reject", {});
    }

    TEST_F(DepartureReleaseEventHandlerTest, ReleaseDecisionMadeAcknowledgesRelease)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);
        this->handler.AddReleaseRequest(this->request);

        ON_CALL(this->mockPlugin, GetSelectedFlightplan).WillByDefault(Return(this->pluginReturnedFlightplan));

        EXPECT_CALL(this->api, AcknowledgeDepartureReleaseRequest(1, 2)).Times(1);

        handler.ReleaseDecisionMade(1, "Acknowledge", {});

        EXPECT_TRUE(this->request->Acknowledged());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ReleaseDecisionHandlesApiExceptionOnAcknowledge)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);
        this->handler.AddReleaseRequest(this->request);

        ON_CALL(this->mockPlugin, GetSelectedFlightplan).WillByDefault(Return(this->pluginReturnedFlightplan));

        EXPECT_CALL(this->api, AcknowledgeDepartureReleaseRequest(1, 2))
            .Times(1)
            .WillOnce(testing::Throw(UKControllerPlugin::Api::ApiException("foo")));

        handler.ReleaseDecisionMade(1, "Acknowledge", {});

        EXPECT_FALSE(this->request->Acknowledged());
    }

    TEST_F(DepartureReleaseEventHandlerTest, RequestReleaseDoesNothingIfControllerNotFound)
    {
        EXPECT_CALL(this->api, RequestDepartureRelease("BAW123", 2, 55, 300)).Times(0);

        this->handler.RequestRelease("BAW123", 55);
    }

    TEST_F(DepartureReleaseEventHandlerTest, RequestReleaseDoesNothingIfTargetControllerCannotReceiveReleases)
    {
        EXPECT_CALL(this->api, RequestDepartureRelease("BAW123", 2, 3, 300)).Times(0);

        this->handler.RequestRelease("BAW123", 3);
    }

    TEST_F(DepartureReleaseEventHandlerTest, RequestReleaseDoesNothingIfControllerNotActive)
    {
        EXPECT_CALL(this->api, RequestDepartureRelease("BAW123", 2, 4, 300)).Times(0);

        this->handler.RequestRelease("BAW123", 4);
    }

    TEST_F(DepartureReleaseEventHandlerTest, RequestReleaseDoesNothingIfControllerCannotRequestReleases)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller2Callsign);
        EXPECT_CALL(this->api, RequestDepartureRelease("BAW123", 2, 4, 300)).Times(0);

        this->handler.RequestRelease("BAW123", 4);
    }

    TEST_F(DepartureReleaseEventHandlerTest, RequestReleaseHandlesNoIdFromResponseData)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);

        nlohmann::json responseData;
        EXPECT_CALL(this->api, RequestDepartureRelease("BAW123", 2, 4, 300))
            .Times(1)
            .WillOnce(testing::Return(responseData));

        this->handler.RequestRelease("BAW123", 4);
        EXPECT_EQ(nullptr, this->handler.GetReleaseRequest(22));
    }

    TEST_F(DepartureReleaseEventHandlerTest, RequestReleaseHandlesIdNotIntgerFromResponseData)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);

        nlohmann::json responseData = {{"id", "abc"}};
        EXPECT_CALL(this->api, RequestDepartureRelease("BAW123", 2, 4, 300))
            .Times(1)
            .WillOnce(testing::Return(responseData));

        this->handler.RequestRelease("BAW123", 4);
        EXPECT_EQ(nullptr, this->handler.GetReleaseRequest(22));
    }

    TEST_F(DepartureReleaseEventHandlerTest, RequestReleaseHandlesApiException)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);

        EXPECT_CALL(this->api, RequestDepartureRelease("BAW123", 2, 4, 300))
            .Times(1)
            .WillOnce(testing::Throw(UKControllerPlugin::Api::ApiException("foo")));

        this->handler.RequestRelease("BAW123", 4);
        EXPECT_EQ(nullptr, this->handler.GetReleaseRequest(22));
    }

    TEST_F(DepartureReleaseEventHandlerTest, RequestReleaseRequestsRelease)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);

        nlohmann::json responseData = {{"id", 22}};
        EXPECT_CALL(this->api, RequestDepartureRelease("BAW123", 2, 4, 300))
            .Times(1)
            .WillOnce(testing::Return(responseData));

        this->handler.RequestRelease("BAW123", 4);
        auto release = this->handler.GetReleaseRequest(22);
        EXPECT_NE(nullptr, release);
        EXPECT_EQ(22, release->Id());
        EXPECT_EQ("BAW123", release->Callsign());
        EXPECT_EQ(2, release->RequestingController());
        EXPECT_EQ(4, release->TargetController());
        EXPECT_EQ(UKControllerPlugin::Time::TimeNow() + std::chrono::seconds(300), release->RequestExpiryTime());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntApproveReleasesIfNoneFound)
    {
        EXPECT_CALL(this->api, ApproveDepartureReleaseRequest).Times(0);

        this->handler.ApproveRelease(1, TimeNow(), 60, "remarks");
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntApproveReleasesIfNoUserCallsign)
    {
        this->handler.AddReleaseRequest(this->request);
        EXPECT_CALL(this->api, ApproveDepartureReleaseRequest).Times(0);

        this->handler.ApproveRelease(1, TimeNow(), 60, "remarks");
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntApproveReleasesIfUserCallsignCantApproveReleases)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller2Callsign);
        this->handler.AddReleaseRequest(this->request);
        EXPECT_CALL(this->api, ApproveDepartureReleaseRequest).Times(0);

        this->handler.ApproveRelease(1, TimeNow(), 60, "remarks");
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesApiExceptionApprovingReleases)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);
        this->handler.AddReleaseRequest(this->request);
        EXPECT_CALL(this->api, ApproveDepartureReleaseRequest(1, 2, UKControllerPlugin::Time::TimeNow(), 60, "remarks"))
            .Times(1)
            .WillOnce(testing::Throw(UKControllerPlugin::Api::ApiException("foo")));

        this->handler.ApproveRelease(1, TimeNow(), 60, "remarks");
        EXPECT_FALSE(this->handler.GetReleaseRequest(1)->Approved());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItApprovesReleases)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);
        this->handler.AddReleaseRequest(this->request);
        EXPECT_CALL(this->api, ApproveDepartureReleaseRequest(1, 2, UKControllerPlugin::Time::TimeNow(), 60, "remarks"))
            .Times(1);

        this->handler.ApproveRelease(1, TimeNow(), 60, "remarks");
        auto release = this->handler.GetReleaseRequest(1);
        EXPECT_TRUE(release->Approved());
        EXPECT_FALSE(release->ApprovedWithNoExpiry());
        EXPECT_EQ(UKControllerPlugin::Time::TimeNow(), release->ReleasedAtTime());
        EXPECT_EQ(UKControllerPlugin::Time::TimeNow() + std::chrono::seconds(60), release->ReleaseExpiryTime());
        EXPECT_EQ("remarks", release->Remarks());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItApprovesReleasesWithNoExpiry)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);
        this->handler.AddReleaseRequest(this->request);
        EXPECT_CALL(this->api, ApproveDepartureReleaseRequest(1, 2, UKControllerPlugin::Time::TimeNow(), -1, "remarks"))
            .Times(1);

        this->handler.ApproveRelease(1, TimeNow(), -1, "remarks");
        auto release = this->handler.GetReleaseRequest(1);
        EXPECT_TRUE(release->Approved());
        EXPECT_TRUE(release->ApprovedWithNoExpiry());
        EXPECT_EQ(UKControllerPlugin::Time::TimeNow(), release->ReleasedAtTime());
        EXPECT_EQ("remarks", release->Remarks());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntRejectReleasesIfNoneFound)
    {
        EXPECT_CALL(this->api, RejectDepartureReleaseRequest).Times(0);

        this->handler.RejectRelease(1, "remarks");
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntRejectReleasesIfNoUserCallsign)
    {
        this->handler.AddReleaseRequest(this->request);
        EXPECT_CALL(this->api, RejectDepartureReleaseRequest).Times(0);

        this->handler.RejectRelease(1, "remarks");
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntRejectReleasesIfUserCallsignCantRejectReleases)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller2Callsign);
        this->handler.AddReleaseRequest(this->request);
        EXPECT_CALL(this->api, RejectDepartureReleaseRequest).Times(0);

        this->handler.RejectRelease(1, "remarks");
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesApiExceptionRejectingReleases)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);
        this->handler.AddReleaseRequest(this->request);
        EXPECT_CALL(this->api, RejectDepartureReleaseRequest(1, 2, "remarks"))
            .Times(1)
            .WillOnce(testing::Throw(UKControllerPlugin::Api::ApiException("foo")));

        this->handler.RejectRelease(1, "remarks");
        EXPECT_FALSE(this->handler.GetReleaseRequest(1)->Rejected());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItRejectsReleases)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);
        this->handler.AddReleaseRequest(this->request);
        EXPECT_CALL(this->api, RejectDepartureReleaseRequest(1, 2, "remarks")).Times(1);

        this->handler.RejectRelease(1, "remarks");
        auto release = this->handler.GetReleaseRequest(1);
        EXPECT_TRUE(release->Rejected());
        EXPECT_EQ("remarks", release->Remarks());
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseStatusIndicatorHasATagItemDescription)
    {
        EXPECT_EQ("Departure Release Status Indicator", this->handler.GetTagItemDescription(124));
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseStatusIndicatorReturnsNormalIfNoReleases)
    {
        this->handler.AddReleaseRequest(std::make_shared<DepartureReleaseRequest>(
            1, "BAW456", 3, 2, std::chrono::system_clock::now() + std::chrono::minutes(5)));
        UKControllerPlugin::Tag::TagData data = this->GetTagData(124);
        this->handler.SetTagItemData(data);
        EXPECT_EQ("", data.GetItemString());
        EXPECT_EQ(this->tagColour, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseStatusIndicatorReturnsPendingIfReleaseOutstanding)
    {
        auto request2 =
            std::make_shared<DepartureReleaseRequest>(2, "BAW123", 2, 3, TimeNow() + std::chrono::minutes(5));
        request2->Approve(TimeNow(), TimeNow() + std::chrono::seconds(25), "remarks");

        this->handler.AddReleaseRequest(request);
        this->handler.AddReleaseRequest(request2);

        UKControllerPlugin::Tag::TagData data = this->GetTagData(124);
        this->handler.SetTagItemData(data);
        EXPECT_EQ("1/2", data.GetItemString());
        EXPECT_EQ(UKControllerPlugin::Releases::statusIndicatorReleasePending, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseStatusIndicatorReturnsApprovedIfAllRequestsApproved)
    {
        auto request2 =
            std::make_shared<DepartureReleaseRequest>(2, "BAW123", 2, 3, TimeNow() + std::chrono::minutes(5));
        request2->Approve(TimeNow(), TimeNow() + std::chrono::seconds(25), "remarks");
        request->Approve(TimeNow(), TimeNow() + std::chrono::seconds(25), "remarks");

        this->handler.AddReleaseRequest(request);
        this->handler.AddReleaseRequest(request2);

        UKControllerPlugin::Tag::TagData data = this->GetTagData(124);
        this->handler.SetTagItemData(data);
        EXPECT_EQ("2/2", data.GetItemString());
        EXPECT_EQ(UKControllerPlugin::Releases::statusIndicatorReleased, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseStatusIndicatorPrefersApprovedOverAcknowledged)
    {
        auto request2 =
            std::make_shared<DepartureReleaseRequest>(2, "BAW123", 2, 3, TimeNow() + std::chrono::minutes(5));
        request2->Approve(TimeNow(), TimeNow() + std::chrono::seconds(25), "remarks");
        request2->Acknowledge();
        request->Acknowledge();
        request->Approve(TimeNow(), TimeNow() + std::chrono::seconds(25), "remarks");

        this->handler.AddReleaseRequest(request);
        this->handler.AddReleaseRequest(request2);

        UKControllerPlugin::Tag::TagData data = this->GetTagData(124);
        this->handler.SetTagItemData(data);
        EXPECT_EQ("2/2", data.GetItemString());
        EXPECT_EQ(UKControllerPlugin::Releases::statusIndicatorReleased, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseStatusIndicatorReturnsPendingTimeIfWaitingForReleaseTime)
    {
        auto request2 =
            std::make_shared<DepartureReleaseRequest>(2, "BAW123", 2, 3, TimeNow() + std::chrono::minutes(5));
        request2->Approve(TimeNow(), TimeNow() + std::chrono::seconds(25), "remarks");
        request->Approve(TimeNow() + std::chrono::seconds(5), TimeNow() + std::chrono::seconds(25), "remarks");

        this->handler.AddReleaseRequest(request);
        this->handler.AddReleaseRequest(request2);

        UKControllerPlugin::Tag::TagData data = this->GetTagData(124);
        this->handler.SetTagItemData(data);
        EXPECT_EQ("2/2", data.GetItemString());
        EXPECT_EQ(UKControllerPlugin::Releases::statusIndicatorReleasedAwaitingTime, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseStatusIndicatorReturnsExpiredIfAReleaseHasExpired)
    {
        auto request2 =
            std::make_shared<DepartureReleaseRequest>(2, "BAW123", 2, 3, TimeNow() + std::chrono::minutes(5));
        request2->Approve(TimeNow(), TimeNow() + std::chrono::seconds(25), "remarks");
        request->Approve(TimeNow() - std::chrono::seconds(35), TimeNow() - std::chrono::seconds(25), "remarks");

        this->handler.AddReleaseRequest(request);
        this->handler.AddReleaseRequest(request2);

        UKControllerPlugin::Tag::TagData data = this->GetTagData(124);
        this->handler.SetTagItemData(data);
        EXPECT_EQ("1/2", data.GetItemString());
        EXPECT_EQ(UKControllerPlugin::Releases::statusIndicatorReleaseExpired, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseStatusIndicatorDisplaysAcknowledgedIfAllAcknowledged)
    {
        auto request2 =
            std::make_shared<DepartureReleaseRequest>(2, "BAW123", 2, 3, TimeNow() + std::chrono::minutes(5));
        request2->Acknowledge();
        request->Acknowledge();

        this->handler.AddReleaseRequest(request);
        this->handler.AddReleaseRequest(request2);

        UKControllerPlugin::Tag::TagData data = this->GetTagData(124);
        this->handler.SetTagItemData(data);
        EXPECT_EQ("0/2", data.GetItemString());
        EXPECT_EQ(UKControllerPlugin::Releases::statusIndicatorReleaseAcknowledged, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseStatusIndicatorReturnsExpiredIfAReleaseRequestHasExpired)
    {
        auto request2 =
            std::make_shared<DepartureReleaseRequest>(2, "BAW123", 2, 3, TimeNow() - std::chrono::minutes(5));
        request->Approve(TimeNow() + std::chrono::seconds(15), TimeNow() + std::chrono::seconds(25), "remarks");

        this->handler.AddReleaseRequest(request);
        this->handler.AddReleaseRequest(request2);

        UKControllerPlugin::Tag::TagData data = this->GetTagData(124);
        this->handler.SetTagItemData(data);
        EXPECT_EQ("0/2", data.GetItemString());
        EXPECT_EQ(UKControllerPlugin::Releases::statusIndicatorReleaseExpired, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseStatusIndicatorReturnsRejectedIfAReleaseHasBeenRejected)
    {
        auto request2 =
            std::make_shared<DepartureReleaseRequest>(2, "BAW123", 2, 3, TimeNow() + std::chrono::minutes(5));
        request2->Approve(TimeNow(), TimeNow() + std::chrono::seconds(25), "remarks");
        request->Reject("remarks");

        this->handler.AddReleaseRequest(request);
        this->handler.AddReleaseRequest(request2);

        UKControllerPlugin::Tag::TagData data = this->GetTagData(124);
        this->handler.SetTagItemData(data);
        EXPECT_EQ("1/2", data.GetItemString());
        EXPECT_EQ(UKControllerPlugin::Releases::statusIndicatorReleaseRejected, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseCountdownTimerHasATagItemDescription)
    {
        EXPECT_EQ("Departure Release Countdown", this->handler.GetTagItemDescription(125));
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseStatusCountdownTimerDisplaysReleasedAlmostExpired)
    {
        UKControllerPlugin::Tag::TagData data = this->GetTagData(125);
        request->Approve(TimeNow(), TimeNow() + std::chrono::seconds(10), "remarks");
        this->handler.AddReleaseRequest(request);

        this->handler.SetTagItemData(data);
        EXPECT_EQ("0:10", data.GetItemString());
        EXPECT_EQ(UKControllerPlugin::Releases::releaseTimerExpired, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseStatusCountdownTimerDisplaysReleasedTimeCritical)
    {
        UKControllerPlugin::Tag::TagData data = this->GetTagData(125);
        request->Approve(TimeNow(), TimeNow() + std::chrono::seconds(30), "remarks");
        this->handler.AddReleaseRequest(request);

        this->handler.SetTagItemData(data);
        EXPECT_EQ("0:30", data.GetItemString());
        EXPECT_EQ(UKControllerPlugin::Releases::releaseTimerClose, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseStatusCountdownTimerDisplaysReleasedPlentyOfTime)
    {
        UKControllerPlugin::Tag::TagData data = this->GetTagData(125);
        request->Approve(TimeNow(), TimeNow() + std::chrono::seconds(90), "remarks");
        this->handler.AddReleaseRequest(request);

        this->handler.SetTagItemData(data);
        EXPECT_EQ("1:30", data.GetItemString());
        EXPECT_EQ(UKControllerPlugin::Releases::releaseTimerPlentyOfTime, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseStatusCountdownTimerDisplaysClosestExpiryTime)
    {
        UKControllerPlugin::Tag::TagData data = this->GetTagData(125);
        request->Approve(TimeNow(), TimeNow() + std::chrono::seconds(90), "remarks");
        auto request2 =
            std::make_shared<DepartureReleaseRequest>(2, "BAW123", 2, 3, TimeNow() + std::chrono::minutes(5));
        request2->Approve(TimeNow(), TimeNow() + std::chrono::seconds(25), "remarks");
        this->handler.AddReleaseRequest(request);
        this->handler.AddReleaseRequest(request2);

        this->handler.SetTagItemData(data);
        EXPECT_EQ("0:25", data.GetItemString());
        EXPECT_EQ(UKControllerPlugin::Releases::releaseTimerClose, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseStatusCountdownTimerIgnoresNonExpiringApprovals)
    {
        UKControllerPlugin::Tag::TagData data = this->GetTagData(125);
        request->Approve(TimeNow(), TimeNow() + std::chrono::seconds(90), "remarks");
        auto request2 =
            std::make_shared<DepartureReleaseRequest>(2, "BAW123", 2, 3, TimeNow() + std::chrono::minutes(5));
        request2->Approve(TimeNow(), "remarks");
        this->handler.AddReleaseRequest(request);
        this->handler.AddReleaseRequest(request2);

        this->handler.SetTagItemData(data);
        EXPECT_EQ("1:30", data.GetItemString());
        EXPECT_EQ(UKControllerPlugin::Releases::releaseTimerPlentyOfTime, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseStatusCountdownTimerDisplaysNothingIfNoExpires)
    {
        UKControllerPlugin::Tag::TagData data = this->GetTagData(125);
        request->Approve(TimeNow(), "remarks");
        auto request2 =
            std::make_shared<DepartureReleaseRequest>(2, "BAW123", 2, 3, TimeNow() + std::chrono::minutes(5));
        request2->Approve(TimeNow(), "remarks");
        this->handler.AddReleaseRequest(request);
        this->handler.AddReleaseRequest(request2);

        this->handler.SetTagItemData(data);
        EXPECT_EQ("", data.GetItemString());
        EXPECT_EQ(this->tagColour, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseStatusCountdownTimerDisplaysTimeUntilRelease)
    {
        UKControllerPlugin::Tag::TagData data = this->GetTagData(125);
        request->Approve(TimeNow(), TimeNow() + std::chrono::seconds(90), "remarks");
        auto request2 =
            std::make_shared<DepartureReleaseRequest>(2, "BAW123", 2, 3, TimeNow() + std::chrono::minutes(5));
        request2->Approve(TimeNow() + std::chrono::seconds(50), TimeNow() + std::chrono::seconds(90), "remarks");

        this->handler.AddReleaseRequest(request);
        this->handler.AddReleaseRequest(request2);

        this->handler.SetTagItemData(data);
        EXPECT_EQ("0:50", data.GetItemString());
        EXPECT_EQ(UKControllerPlugin::Releases::statusIndicatorReleasedAwaitingTime, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseStatusCountdownTimerDisplaysFurthestTimeUntilRelease)
    {
        UKControllerPlugin::Tag::TagData data = this->GetTagData(125);
        request->Approve(TimeNow() + std::chrono::seconds(65), TimeNow() + std::chrono::seconds(90), "remarks");
        auto request2 =
            std::make_shared<DepartureReleaseRequest>(2, "BAW123", 2, 3, TimeNow() + std::chrono::minutes(5));
        request2->Approve(TimeNow() + std::chrono::seconds(50), TimeNow() + std::chrono::seconds(90), "remarks");

        this->handler.AddReleaseRequest(request);
        this->handler.AddReleaseRequest(request2);

        this->handler.SetTagItemData(data);
        EXPECT_EQ("1:05", data.GetItemString());
        EXPECT_EQ(UKControllerPlugin::Releases::statusIndicatorReleasedAwaitingTime, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseStatusCountdownTimerDoesNothingIfOneNotApproved)
    {
        UKControllerPlugin::Tag::TagData data = this->GetTagData(125);
        auto request2 =
            std::make_shared<DepartureReleaseRequest>(2, "BAW123", 2, 3, TimeNow() + std::chrono::minutes(5));
        request2->Approve(TimeNow() - std::chrono::seconds(50), TimeNow() + std::chrono::seconds(90), "remarks");

        this->handler.AddReleaseRequest(request);
        this->handler.AddReleaseRequest(request2);

        this->handler.SetTagItemData(data);
        EXPECT_EQ("", data.GetItemString());
        EXPECT_EQ(tagColour, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseStatusCountdownTimerDoesNothingIfApprovalExpired)
    {
        UKControllerPlugin::Tag::TagData data = this->GetTagData(125);
        auto request2 =
            std::make_shared<DepartureReleaseRequest>(2, "BAW123", 2, 3, TimeNow() + std::chrono::minutes(5));
        request2->Approve(TimeNow() - std::chrono::seconds(50), TimeNow() - std::chrono::seconds(10), "remarks");

        this->handler.AddReleaseRequest(request2);

        this->handler.SetTagItemData(data);
        EXPECT_EQ("", data.GetItemString());
        EXPECT_EQ(tagColour, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseStatusCountdownTimerIgnoresOtherCallsignsReleases)
    {
        UKControllerPlugin::Tag::TagData data = this->GetTagData(125);
        auto request2 =
            std::make_shared<DepartureReleaseRequest>(2, "BAW456", 2, 3, TimeNow() + std::chrono::minutes(5));
        request2->Approve(TimeNow() - std::chrono::seconds(50), TimeNow() - std::chrono::seconds(10), "remarks");

        this->handler.AddReleaseRequest(request2);

        this->handler.SetTagItemData(data);
        EXPECT_EQ("", data.GetItemString());
        EXPECT_EQ(tagColour, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItTriggersTheRequestView)
    {
        auto request2 =
            std::make_shared<DepartureReleaseRequest>(2, "BAW456", 2, 3, TimeNow() + std::chrono::minutes(5));

        this->handler.AddReleaseRequest(request);
        this->handler.AddReleaseRequest(request2);

        this->handler.ShowStatusDisplay(this->mockFlightplan, this->mockRadarTarget, "", {101, 202});
        EXPECT_EQ(1, this->handler.GetReleasesToDisplay().size());
        EXPECT_EQ(1, (*this->handler.GetReleasesToDisplay().cbegin())->Id());
        EXPECT_TRUE(UKControllerPlugin::Releases::DepartureReleaseRequestView::ShouldDisplay());
        auto coords = UKControllerPlugin::Releases::DepartureReleaseRequestView::GetCoordinates();
        EXPECT_EQ(101, coords.x);
        EXPECT_EQ(202, coords.y);
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesNothingIfNoReleasesToDisplay)
    {
        auto request2 =
            std::make_shared<DepartureReleaseRequest>(2, "BAW456", 2, 3, TimeNow() + std::chrono::minutes(5));

        this->handler.AddReleaseRequest(request2);

        this->handler.ShowStatusDisplay(this->mockFlightplan, this->mockRadarTarget, "", {101, 202});
        EXPECT_EQ(0, this->handler.GetReleasesToDisplay().size());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntDisplayCancellationListIfUserDoesNotHaveActiveCallsign)
    {
        EXPECT_CALL(this->mockPlugin, TriggerPopupList(testing::_, testing::_, testing::_)).Times(0);

        this->handler.AddReleaseRequest(request);

        this->handler.SelectReleaseRequestToCancel(this->mockFlightplan, this->mockRadarTarget, "", {101, 202});
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDisplaysTheCancellationList)
    {
        auto request2 = std::make_shared<DepartureReleaseRequest>(
            3,
            "BAW456",
            3,
            4,
            TimeNow() + std::chrono::minutes(5)); // Wont show, wrong callsign

        auto request3 = std::make_shared<DepartureReleaseRequest>(
            4,
            "BAW123",
            2,
            3,
            TimeNow() + std::chrono::minutes(5)); // Wont show, wrong requesting controller

        auto request4 = std::make_shared<DepartureReleaseRequest>(
            5,
            "BAW123",
            3,
            4,
            TimeNow() + std::chrono::minutes(5)); // Will show

        this->handler.AddReleaseRequest(request4);
        this->handler.AddReleaseRequest(request3);
        this->handler.AddReleaseRequest(request2);
        this->handler.AddReleaseRequest(request);

        this->activeCallsigns.AddUserCallsign(*this->controller2Callsign);

        UKControllerPlugin::Plugin::PopupMenuItem menuItemCardiffRadar;
        menuItemCardiffRadar.firstValue = "EGFF_APP";
        menuItemCardiffRadar.secondValue = "";
        menuItemCardiffRadar.callbackFunctionId = 4;
        menuItemCardiffRadar.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        menuItemCardiffRadar.disabled = false;
        menuItemCardiffRadar.fixedPosition = false;

        UKControllerPlugin::Plugin::PopupMenuItem menuItemLondonControl;
        menuItemLondonControl.firstValue = "LON_W_CTR";
        menuItemLondonControl.secondValue = "";
        menuItemLondonControl.callbackFunctionId = 4;
        menuItemLondonControl.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        menuItemLondonControl.disabled = false;
        menuItemLondonControl.fixedPosition = false;

        RECT expectedArea = {0, 0, 400, 600};
        EXPECT_CALL(this->mockPlugin, TriggerPopupList(RectEq(expectedArea), "Cancel Departure Release", 1)).Times(1);

        EXPECT_CALL(this->mockPlugin, AddItemToPopupList(menuItemCardiffRadar)).Times(1);

        EXPECT_CALL(this->mockPlugin, AddItemToPopupList(menuItemLondonControl)).Times(1);

        this->handler.SelectReleaseRequestToCancel(this->mockFlightplan, this->mockRadarTarget, "", {0, 0});
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntCancelTheRequestIfNoSelectedFlightplan)
    {
        EXPECT_CALL(this->api, CancelDepartureReleaseRequest(testing::_)).Times(0);

        ON_CALL(this->mockPlugin, GetSelectedFlightplan).WillByDefault(testing::Return(nullptr));

        this->handler.AddReleaseRequest(request);
        this->activeCallsigns.AddUserCallsign(*this->controller2Callsign);
        this->handler.RequestCancelled(4, "EGFF_APP", {});
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntCancelTheRequestIfWrongCallsign)
    {
        EXPECT_CALL(this->api, CancelDepartureReleaseRequest(testing::_)).Times(0);

        ON_CALL(this->mockPlugin, GetSelectedFlightplan).WillByDefault(Return(this->pluginReturnedFlightplan));

        auto request2 =
            std::make_shared<DepartureReleaseRequest>(2, "BAW456", 3, 2, TimeNow() + std::chrono::minutes(5));

        this->handler.AddReleaseRequest(request2);
        this->activeCallsigns.AddUserCallsign(*this->controller2Callsign);
        this->handler.RequestCancelled(4, "EGFF_APP", {});
        EXPECT_NE(nullptr, this->handler.GetReleaseRequest(2));
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItDoesntCancelTheRequestIfWrongController)
    {
        EXPECT_CALL(this->api, CancelDepartureReleaseRequest(testing::_)).Times(0);

        ON_CALL(this->mockPlugin, GetSelectedFlightplan).WillByDefault(Return(this->pluginReturnedFlightplan));

        this->handler.AddReleaseRequest(request);
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);
        this->handler.RequestCancelled(4, "LON_W_CTR", {});
        EXPECT_NE(nullptr, this->handler.GetReleaseRequest(1));
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItCancelsTheRequest)
    {
        EXPECT_CALL(this->api, CancelDepartureReleaseRequest(1)).Times(1);

        ON_CALL(this->mockPlugin, GetSelectedFlightplan).WillByDefault(Return(this->pluginReturnedFlightplan));

        this->handler.AddReleaseRequest(request);
        this->activeCallsigns.AddUserCallsign(*this->controller2Callsign);
        this->handler.RequestCancelled(4, "EGFF_APP", {});
        EXPECT_EQ(nullptr, this->handler.GetReleaseRequest(1));
    }

    TEST_F(DepartureReleaseEventHandlerTest, ItHandlesApiExceptionWhenCancellingTheRequest)
    {
        EXPECT_CALL(this->api, CancelDepartureReleaseRequest(1))
            .Times(1)
            .WillOnce(testing::Throw(UKControllerPlugin::Api::ApiException("foo")));

        ON_CALL(this->mockPlugin, GetSelectedFlightplan).WillByDefault(Return(this->pluginReturnedFlightplan));

        this->handler.AddReleaseRequest(request);
        this->activeCallsigns.AddUserCallsign(*this->controller2Callsign);
        this->handler.RequestCancelled(4, "EGFF_APP", {});
        EXPECT_NE(nullptr, this->handler.GetReleaseRequest(1));
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseRequestingControllerATagItemDescription)
    {
        EXPECT_EQ("Departure Release Requesting Controller", this->handler.GetTagItemDescription(126));
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseRequestingControllerSetsLeadControllerCallsign)
    {
        UKControllerPlugin::Tag::TagData data = this->GetTagData(126);
        this->handler.AddReleaseRequest(request);
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);
        this->activeCallsigns.AddCallsign(*this->controller2Callsign);

        this->handler.SetTagItemData(data);
        EXPECT_EQ("EGFF_1_TWR", data.GetItemString());
        EXPECT_EQ(tagColour, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseRequestingControllerSetsFallbackCallsign)
    {
        UKControllerPlugin::Tag::TagData data = this->GetTagData(126);
        this->handler.AddReleaseRequest(request);
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);

        this->handler.SetTagItemData(data);
        EXPECT_EQ("EGFF_TWR", data.GetItemString());
        EXPECT_EQ(tagColour, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseRequestingControllerDoesNothingIfNoUserCallsign)
    {
        UKControllerPlugin::Tag::TagData data = this->GetTagData(126);
        this->handler.AddReleaseRequest(request);

        this->handler.SetTagItemData(data);
        EXPECT_EQ("", data.GetItemString());
        EXPECT_EQ(tagColour, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, DepartureReleaseRequestingControllerDoesNothingIfNoRelease)
    {
        UKControllerPlugin::Tag::TagData data = this->GetTagData(126);
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);

        this->handler.SetTagItemData(data);
        EXPECT_EQ("", data.GetItemString());
        EXPECT_EQ(tagColour, data.GetTagColour());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ReleasesRequiringUserDecisionReturnsNothingIfUserNotActive)
    {
        EXPECT_TRUE(this->handler.GetReleasesRequiringUsersDecision().empty());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ReleasesRequiringUserDecisionReturnsNothingIfNoReleases)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);
        EXPECT_TRUE(this->handler.GetReleasesRequiringUsersDecision().empty());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ReleasesRequiringUserIgnoresDecidedReleases)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);
        this->handler.AddReleaseRequest(request);
        this->request->Reject("remarks");

        EXPECT_TRUE(this->handler.GetReleasesRequiringUsersDecision().empty());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ReleasesRequiringUserIgnoresReleasesForOtherControllers)
    {
        this->activeCallsigns.AddUserCallsign(*this->controller2Callsign);
        this->handler.AddReleaseRequest(request);

        EXPECT_TRUE(this->handler.GetReleasesRequiringUsersDecision().empty());
    }

    TEST_F(DepartureReleaseEventHandlerTest, ReleasesRequiringUserReturnsReleasesInOrder)
    {
        auto request2 =
            std::make_shared<DepartureReleaseRequest>(3, "BAW456", 3, 2, TimeNow() + std::chrono::minutes(5));

        auto request3 =
            std::make_shared<DepartureReleaseRequest>(5, "BAW789", 3, 2, TimeNow() + std::chrono::minutes(5));

        this->activeCallsigns.AddUserCallsign(*this->controller1Callsign);
        this->handler.AddReleaseRequest(request3);
        this->handler.AddReleaseRequest(request);
        this->handler.AddReleaseRequest(request2);

        const auto releasesRequringUser = this->handler.GetReleasesRequiringUsersDecision();
        EXPECT_EQ(3, releasesRequringUser.size());
        auto it = releasesRequringUser.cbegin();
        EXPECT_EQ(1, (*it)->Id());
        EXPECT_EQ("BAW123", (*it)->Callsign());
        ++it;
        EXPECT_EQ(3, (*it)->Id());
        EXPECT_EQ("BAW456", (*it)->Callsign());
        ++it;
        EXPECT_EQ(5, (*it)->Id());
        EXPECT_EQ("BAW789", (*it)->Callsign());
    }
} // namespace UKControllerPluginTest::Releases
