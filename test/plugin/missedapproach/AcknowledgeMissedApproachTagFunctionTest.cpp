#include "missedapproach/AcknowledgeMissedApproachTagFunction.h"
#include "missedapproach/MissedApproach.h"
#include "missedapproach/MissedApproachCollection.h"
#include "api/ApiException.h"
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"
#include "dialog/DialogData.h"
#include "dialog/DialogManager.h"
#include "ownership/AirfieldServiceProviderCollection.h"
#include "ownership/ServiceProvision.h"
#include "time/SystemClock.h"

using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::MissedApproach::AcknowledgeMissedApproachTagFunction;
using UKControllerPlugin::MissedApproach::MissedApproach;
using UKControllerPlugin::MissedApproach::MissedApproachCollection;
using UKControllerPlugin::Ownership::AirfieldServiceProviderCollection;
using UKControllerPlugin::Ownership::ServiceProvision;
using UKControllerPlugin::Ownership::ServiceType;
using UKControllerPlugin::Time::SetTestNow;

namespace UKControllerPluginTest::MissedApproach {
    class AcknowledgeMissedApproachTagFunctionTest : public testing::Test
    {
        public:
        AcknowledgeMissedApproachTagFunctionTest()
            : position(1, "EGKK_APP", 123.456, {}, true, true),
              userCallsign(std::make_shared<ActiveCallsign>("EGKK_APP", "Test", position, true)),
              notUserCallsign(std::make_shared<ActiveCallsign>("EGKK_APP", "Test", position, false)),
              dialogManager(mockDialogProvider), tagFunction(missedApproaches, dialogManager, mockApi, providers)
        {
            SetTestNow(std::chrono::system_clock::now());
            ON_CALL(mockFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));
            ON_CALL(mockFlightplan, GetDestination).WillByDefault(testing::Return("EGKK"));
            dialogManager.AddDialog(missedApproachDialogData);

            this->providers.SetProvidersForAirfield(
                "EGKK", {std::make_shared<ServiceProvision>(ServiceType::FinalApproach, userCallsign)});
        }

        ControllerPosition position;
        std::shared_ptr<ActiveCallsign> userCallsign;
        std::shared_ptr<ActiveCallsign> notUserCallsign;
        DialogData missedApproachDialogData = {IDD_MISSED_APPROACH_ACKNOWLEDGE, "Test"};
        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> mockFlightplan;
        MissedApproachCollection missedApproaches;
        testing::NiceMock<Dialog::MockDialogProvider> mockDialogProvider;
        testing::NiceMock<Api::MockApiInterface> mockApi;
        DialogManager dialogManager;
        AirfieldServiceProviderCollection providers;
        AcknowledgeMissedApproachTagFunction tagFunction;
    };

    TEST_F(AcknowledgeMissedApproachTagFunctionTest, ItTriggersTheDialog)
    {
        missedApproaches.Add(std::make_shared<class MissedApproach>(
            1, "BAW123", std::chrono::system_clock::now() + std::chrono::minutes(5), false));

        EXPECT_CALL(this->mockDialogProvider, OpenDialog(this->missedApproachDialogData, testing::_)).Times(1);

        tagFunction.TriggerDialog(mockFlightplan);
    }

    TEST_F(AcknowledgeMissedApproachTagFunctionTest, ItDoesntTriggerDialogUserNotProvidingFinalApproach)
    {
        this->providers.SetProvidersForAirfield(
            "EGKK", {std::make_shared<ServiceProvision>(ServiceType::Tower, userCallsign)});

        missedApproaches.Add(std::make_shared<class MissedApproach>(
            1, "BAW123", std::chrono::system_clock::now() + std::chrono::minutes(5), false));

        EXPECT_CALL(this->mockDialogProvider, OpenDialog(testing::_, testing::_)).Times(0);

        tagFunction.TriggerDialog(mockFlightplan);
    }

    TEST_F(AcknowledgeMissedApproachTagFunctionTest, ItDoesntTriggerDialogWrongCallsign)
    {
        missedApproaches.Add(std::make_shared<class MissedApproach>(
            1, "BAW456", std::chrono::system_clock::now() + std::chrono::minutes(5), false));

        EXPECT_CALL(this->mockDialogProvider, OpenDialog(testing::_, testing::_)).Times(0);

        tagFunction.TriggerDialog(mockFlightplan);
    }

    TEST_F(AcknowledgeMissedApproachTagFunctionTest, ItDoesntTriggerDialogTriggeredByUser)
    {
        missedApproaches.Add(std::make_shared<class MissedApproach>(
            1, "BAW123", std::chrono::system_clock::now() + std::chrono::minutes(5), true));

        EXPECT_CALL(this->mockDialogProvider, OpenDialog(testing::_, testing::_)).Times(0);

        tagFunction.TriggerDialog(mockFlightplan);
    }

    TEST_F(AcknowledgeMissedApproachTagFunctionTest, ItDoesntTriggerDialogExpiredMissedApproach)
    {
        missedApproaches.Add(std::make_shared<class MissedApproach>(
            1, "BAW123", std::chrono::system_clock::now() - std::chrono::minutes(5), false));

        EXPECT_CALL(this->mockDialogProvider, OpenDialog(testing::_, testing::_)).Times(0);

        tagFunction.TriggerDialog(mockFlightplan);
    }

    TEST_F(AcknowledgeMissedApproachTagFunctionTest, ItDoesntTriggerDialogAlreadyAcknowledgedMissedApproach)
    {
        missedApproaches.Add(std::make_shared<class MissedApproach>(
            1, "BAW123", std::chrono::system_clock::now() + std::chrono::minutes(5), false));
        missedApproaches.Get(1)->Acknowledge("test", "test");

        EXPECT_CALL(this->mockDialogProvider, OpenDialog(testing::_, testing::_)).Times(0);

        tagFunction.TriggerDialog(mockFlightplan);
    }

    TEST_F(AcknowledgeMissedApproachTagFunctionTest, ItTriggersAcknowledgement)
    {
        missedApproaches.Add(std::make_shared<class MissedApproach>(
            1, "BAW123", std::chrono::system_clock::now() + std::chrono::minutes(5), false));

        EXPECT_CALL(mockApi, AcknowledgeMissedApproach(1, "Some remarks")).Times(1);

        tagFunction.TriggerDialog(mockFlightplan);
        tagFunction.Acknowledge("Some remarks");
        EXPECT_TRUE(missedApproaches.Get(1)->IsAcknowledged());
    }

    TEST_F(AcknowledgeMissedApproachTagFunctionTest, ItHandlesApiExceptionOnAcknowledgement)
    {
        missedApproaches.Add(std::make_shared<class MissedApproach>(
            1, "BAW123", std::chrono::system_clock::now() + std::chrono::minutes(5), false));

        EXPECT_CALL(mockApi, AcknowledgeMissedApproach(1, "Some remarks"))
            .Times(1)
            .WillOnce(testing::Throw(ApiException("Foo")));

        tagFunction.TriggerDialog(mockFlightplan);
        EXPECT_NO_THROW(tagFunction.Acknowledge("Some remarks"));
        EXPECT_FALSE(missedApproaches.Get(1)->IsAcknowledged());
    }

    TEST_F(AcknowledgeMissedApproachTagFunctionTest, ItDoesntAcknowledgeIfNoApproach)
    {
        missedApproaches.Add(std::make_shared<class MissedApproach>(
            1, "BAW123", std::chrono::system_clock::now() + std::chrono::minutes(5), false));

        EXPECT_CALL(mockApi, AcknowledgeMissedApproach(testing::_, testing::_)).Times(0);

        tagFunction.TriggerDialog(mockFlightplan);
        missedApproaches.Remove(missedApproaches.Get(1));
        tagFunction.Acknowledge("Some remarks");
    }

    TEST_F(AcknowledgeMissedApproachTagFunctionTest, ItDoesntAcknowledgeIfUserNotProvidingFinalApproach)
    {
        missedApproaches.Add(std::make_shared<class MissedApproach>(
            1, "BAW123", std::chrono::system_clock::now() + std::chrono::minutes(5), false));

        EXPECT_CALL(mockApi, AcknowledgeMissedApproach(testing::_, testing::_)).Times(0);

        tagFunction.TriggerDialog(mockFlightplan);

        this->providers.SetProvidersForAirfield(
            "EGKK", {std::make_shared<ServiceProvision>(ServiceType::Tower, userCallsign)});

        tagFunction.Acknowledge("Some remarks");
        EXPECT_FALSE(missedApproaches.Get(1)->IsAcknowledged());
    }
} // namespace UKControllerPluginTest::MissedApproach
