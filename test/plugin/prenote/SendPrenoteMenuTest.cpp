#include "api/ApiException.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "mock/MockApiInterface.h"
#include "mock/MockEuroScopeCControllerInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockTaskRunnerInterface.h"
#include "prenote/SendPrenoteMenu.h"
#include "prenote/PrenoteMessage.h"
#include "prenote/PrenoteMessageCollection.h"
#include "time/SystemClock.h"

using testing::NiceMock;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Prenote::PrenoteMessage;
using UKControllerPlugin::Prenote::PrenoteMessageCollection;
using UKControllerPlugin::Prenote::SendPrenoteMenu;
using UKControllerPlugin::Time::SetTestNow;
using UKControllerPlugin::Time::TimeNow;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCControllerInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;

namespace UKControllerPluginTest::Prenote {
    class SendPrenoteMenuTest : public ::testing::Test
    {
        public:
        SendPrenoteMenuTest()
            : collection(std::make_shared<PrenoteMessageCollection>()),
              menu(collection, controllers, callsigns, mockPlugin, mockTaskRunner, mockApi, 55)
        {
            SetTestNow(std::chrono::system_clock::now());

            ON_CALL(mockFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));

            mockPluginReturnedFlightplan = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(*mockPluginReturnedFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));
            ON_CALL(*mockPluginReturnedFlightplan, GetOrigin).WillByDefault(testing::Return("EGGD"));
            ON_CALL(*mockPluginReturnedFlightplan, GetSidName).WillByDefault(testing::Return("BADIM1X"));
            ON_CALL(*mockPluginReturnedFlightplan, GetDestination).WillByDefault(testing::Return("EGLL"));

            collection->Add(std::make_shared<PrenoteMessage>(1, "BAW123", "EGGD", "BADIM1X", "EGLL", 1, 5, TimeNow()));

            controllers.AddPosition(std::make_shared<ControllerPosition>(
                1, "EGKK_APP", 126.820, std::vector<std::string>{"EGKK"}, true, false, true, true));

            controllers.AddPosition(std::make_shared<ControllerPosition>(
                2, "LON_S_CTR", 129.420, std::vector<std::string>{"EGKK"}, true, true, true, true));

            controllers.AddPosition(std::make_shared<ControllerPosition>(
                3, "LON_SC_CTR", 132.600, std::vector<std::string>{"EGKK"}, true, false, false, true));

            controllers.AddPosition(std::make_shared<ControllerPosition>(
                4, "LON_W_CTR", 126.025, std::vector<std::string>{"EGGD"}, true, false, true, false));

            controllers.AddPosition(std::make_shared<ControllerPosition>(
                5, "LON_CTR", 123.9, std::vector<std::string>{"EGGD"}, true, false, true, true));

            // Default the user to active
            std::array<int, 5> positionsToAdd{1, 2, 3, 4, 5};
            for (const auto& positionId : positionsToAdd) {
                auto mockController = std::make_shared<NiceMock<MockEuroScopeCControllerInterface>>();
                ON_CALL(*mockController, GetCallsign)
                    .WillByDefault(testing::Return(this->controllers.FetchPositionById(positionId)->GetCallsign()));

                ON_CALL(*mockController, IsCurrentUser).WillByDefault(testing::Return(positionId == 1));

                if (positionId != 1) {
                    callsigns.AddCallsign(ActiveCallsign(
                        this->controllers.FetchPositionById(positionId)->GetCallsign(),
                        "Test",
                        *controllers.FetchPositionById(positionId)));
                    this->mockPlugin.AddAllControllersItem(mockController);
                } else {
                    callsigns.AddUserCallsign(ActiveCallsign(
                        this->controllers.FetchPositionById(positionId)->GetCallsign(),
                        "Test",
                        *controllers.FetchPositionById(positionId)));
                    this->mockPlugin.AddAllControllersItem(mockController);
                }
            }

            // Extra callsign for the purposes of "non-actives"
            auto mockController = std::make_shared<NiceMock<MockEuroScopeCControllerInterface>>();
            ON_CALL(*mockController, GetCallsign).WillByDefault(testing::Return("LON_D_CTR"));

            ON_CALL(*mockController, IsCurrentUser).WillByDefault(testing::Return(false));
        }

        ActiveCallsignCollection callsigns;
        ControllerPositionCollection controllers;
        NiceMock<MockTaskRunnerInterface> mockTaskRunner;
        NiceMock<MockApiInterface> mockApi;
        NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
        NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
        std::shared_ptr<MockEuroScopeCFlightPlanInterface> mockPluginReturnedFlightplan;
        std::shared_ptr<PrenoteMessageCollection> collection;
        SendPrenoteMenu menu;
    };

    TEST_F(SendPrenoteMenuTest, ItDisplaysTheCancellationMenuWithControllers)
    {
        PopupMenuItem menuItemControllerOne;
        menuItemControllerOne.firstValue = "LON_S_CTR";
        menuItemControllerOne.secondValue = "";
        menuItemControllerOne.callbackFunctionId = 55;
        menuItemControllerOne.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        menuItemControllerOne.disabled = false;
        menuItemControllerOne.fixedPosition = false;

        PopupMenuItem menuItemControllerTwo;
        menuItemControllerTwo.firstValue = "LON_SC_CTR";
        menuItemControllerTwo.secondValue = "";
        menuItemControllerTwo.callbackFunctionId = 55;
        menuItemControllerTwo.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        menuItemControllerTwo.disabled = false;
        menuItemControllerTwo.fixedPosition = false;

        RECT expectedArea = {0, 0, 400, 600};
        EXPECT_CALL(mockPlugin, TriggerPopupList(RectEq(expectedArea), "Send Prenote", 1)).Times(1);

        EXPECT_CALL(mockPlugin, AddItemToPopupList(menuItemControllerOne)).Times(1);

        EXPECT_CALL(mockPlugin, AddItemToPopupList(menuItemControllerTwo)).Times(1);

        menu.DisplayControllerSelectionMenu(mockFlightplan, {0, 0});
    }

    TEST_F(SendPrenoteMenuTest, ItDoesntDisplayControllerMenuIfUserIsNotActive)
    {
        callsigns.RemoveCallsign(ActiveCallsign("EGKK_APP", "Test", *controllers.FetchPositionById(1)));
        EXPECT_CALL(mockPlugin, TriggerPopupList(testing::_, testing::_, testing::_)).Times(0);

        menu.DisplayControllerSelectionMenu(mockFlightplan, {0, 0});
    }

    TEST_F(SendPrenoteMenuTest, ItDoesntDisplayControllerMenuIfUserCantSendPrenotes)
    {
        callsigns.RemoveCallsign(ActiveCallsign("EGKK_APP", "Test", *controllers.FetchPositionById(1)));
        callsigns.RemoveCallsign(ActiveCallsign("LON_SC_CTR", "Test", *controllers.FetchPositionById(3)));
        callsigns.AddUserCallsign(ActiveCallsign("LON_SC_CTR", "Test", *controllers.FetchPositionById(3)));
        EXPECT_CALL(mockPlugin, TriggerPopupList(testing::_, testing::_, testing::_)).Times(0);

        menu.DisplayControllerSelectionMenu(mockFlightplan, {0, 0});
    }

    TEST_F(SendPrenoteMenuTest, ItSendsAPrenoteMessage)
    {
        ON_CALL(this->mockPlugin, GetSelectedFlightplan)
            .WillByDefault(testing::Return(this->mockPluginReturnedFlightplan));

        EXPECT_CALL(this->mockApi, CreatePrenoteMessage("BAW123", "EGGD", "BADIM1X", "EGLL", 1, 2, 300))
            .Times(1)
            .WillOnce(testing::Return(nlohmann::json({{"id", 55}})));

        menu.ControllerForPrenoteSelected("LON_S_CTR");

        auto prenote = this->collection->GetById(55);
        EXPECT_NE(nullptr, prenote);
        EXPECT_EQ(55, prenote->GetId());
        EXPECT_EQ("BAW123", prenote->GetCallsign());
        EXPECT_EQ("EGGD", prenote->GetDepartureAirfield());
        EXPECT_EQ("BADIM1X", prenote->GetSid());
        EXPECT_EQ("EGLL", prenote->GetDestinationAirfield());
        EXPECT_EQ(1, prenote->GetSendingControllerId());
        EXPECT_EQ(2, prenote->GetTargetControllerId());
        EXPECT_EQ(TimeNow() + std::chrono::seconds(300), prenote->GetExpiresAt());
        EXPECT_FALSE(prenote->IsAcknowledged());
    }

    TEST_F(SendPrenoteMenuTest, ItHandlesApiExceptionFromRequest)
    {
        ON_CALL(this->mockPlugin, GetSelectedFlightplan)
            .WillByDefault(testing::Return(this->mockPluginReturnedFlightplan));

        EXPECT_CALL(this->mockApi, CreatePrenoteMessage("BAW123", "EGGD", "BADIM1X", "EGLL", 1, 2, 300))
            .Times(1)
            .WillOnce(testing::Throw(ApiException("Foo")));

        menu.ControllerForPrenoteSelected("LON_S_CTR");

        EXPECT_EQ(nullptr, this->collection->GetById(55));
    }

    TEST_F(SendPrenoteMenuTest, ItHandlesIdNotIntegerFromResponse)
    {
        ON_CALL(this->mockPlugin, GetSelectedFlightplan)
            .WillByDefault(testing::Return(this->mockPluginReturnedFlightplan));

        EXPECT_CALL(this->mockApi, CreatePrenoteMessage("BAW123", "EGGD", "BADIM1X", "EGLL", 1, 2, 300))
            .Times(1)
            .WillOnce(testing::Return(nlohmann::json({{"id", "abc"}})));

        menu.ControllerForPrenoteSelected("LON_S_CTR");

        EXPECT_EQ(nullptr, this->collection->GetById(55));
    }

    TEST_F(SendPrenoteMenuTest, ItHandlesIdMissingFromResponse)
    {
        ON_CALL(this->mockPlugin, GetSelectedFlightplan)
            .WillByDefault(testing::Return(this->mockPluginReturnedFlightplan));

        EXPECT_CALL(this->mockApi, CreatePrenoteMessage("BAW123", "EGGD", "BADIM1X", "EGLL", 1, 2, 300))
            .Times(1)
            .WillOnce(testing::Return(nlohmann::json({{"notid", 12}})));

        menu.ControllerForPrenoteSelected("LON_S_CTR");

        EXPECT_EQ(nullptr, this->collection->GetById(55));
    }

    TEST_F(SendPrenoteMenuTest, ItHandlesResponseNotObject)
    {
        ON_CALL(this->mockPlugin, GetSelectedFlightplan)
            .WillByDefault(testing::Return(this->mockPluginReturnedFlightplan));

        EXPECT_CALL(this->mockApi, CreatePrenoteMessage("BAW123", "EGGD", "BADIM1X", "EGLL", 1, 2, 300))
            .Times(1)
            .WillOnce(testing::Return(nlohmann::json::array({1, 2})));

        menu.ControllerForPrenoteSelected("LON_S_CTR");

        EXPECT_EQ(nullptr, this->collection->GetById(55));
    }

    TEST_F(SendPrenoteMenuTest, ItHandlesInvalidControllerChosen)
    {
        ON_CALL(this->mockPlugin, GetSelectedFlightplan)
            .WillByDefault(testing::Return(this->mockPluginReturnedFlightplan));

        EXPECT_CALL(
            this->mockApi,
            CreatePrenoteMessage(testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_))
            .Times(0);

        menu.ControllerForPrenoteSelected("LTC_CTR");

        EXPECT_EQ(nullptr, this->collection->GetById(55));
    }

    TEST_F(SendPrenoteMenuTest, ItHandlesUserNotActive)
    {
        callsigns.Flush();

        ON_CALL(this->mockPlugin, GetSelectedFlightplan)
            .WillByDefault(testing::Return(this->mockPluginReturnedFlightplan));

        EXPECT_CALL(
            this->mockApi,
            CreatePrenoteMessage(testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_))
            .Times(0);

        menu.ControllerForPrenoteSelected("LON_S_CTR");

        EXPECT_EQ(nullptr, this->collection->GetById(55));
    }

    TEST_F(SendPrenoteMenuTest, ItHandlesNoSelectedFlightplan)
    {
        ON_CALL(this->mockPlugin, GetSelectedFlightplan).WillByDefault(testing::Return(nullptr));

        EXPECT_CALL(
            this->mockApi,
            CreatePrenoteMessage(testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_))
            .Times(0);

        menu.ControllerForPrenoteSelected("LON_S_CTR");

        EXPECT_EQ(nullptr, this->collection->GetById(55));
    }
} // namespace UKControllerPluginTest::Prenote
