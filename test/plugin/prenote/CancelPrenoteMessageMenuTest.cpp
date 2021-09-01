#include "api/ApiException.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "mock/MockApiInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockTaskRunnerInterface.h"
#include "prenote/CancelPrenoteMessageMenu.h"
#include "prenote/PrenoteMessage.h"
#include "prenote/PrenoteMessageCollection.h"
#include "time/SystemClock.h"

using testing::NiceMock;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Prenote::CancelPrenoteMessageMenu;
using UKControllerPlugin::Prenote::PrenoteMessage;
using UKControllerPlugin::Prenote::PrenoteMessageCollection;
using UKControllerPlugin::Time::TimeNow;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;

namespace UKControllerPluginTest::Prenote {
    class CancelPrenoteMessageMenuTest : public ::testing::Test
    {
        public:
        CancelPrenoteMessageMenuTest()
            : collection(std::make_shared<PrenoteMessageCollection>()),
              menu(collection, controllers, mockPlugin, mockTaskRunner, mockApi, 55)
        {
            ON_CALL(mockFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));

            mockPluginReturnedFlightplan = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(*mockPluginReturnedFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));

            collection->Add(std::make_shared<PrenoteMessage>(1, "BAW123", "EGGD", "BADIM1X", "EGLL", 1, 2, TimeNow()));
            collection->Add(std::make_shared<PrenoteMessage>(2, "BAW123", "EGGD", "BADIM1X", "EGLL", 2, 3, TimeNow()));
            collection->Add(std::make_shared<PrenoteMessage>(5, "BAW456", "EGGD", "BADIM1X", "EGLL", 1, 2, TimeNow()));

            controllers.AddPosition(std::make_shared<ControllerPosition>(
                1, "EGKK_APP", 126.820, std::vector<std::string>{"EGKK"}, true, false));

            controllers.AddPosition(std::make_shared<ControllerPosition>(
                2, "LON_S_CTR", 129.420, std::vector<std::string>{"EGKK"}, true, false));

            controllers.AddPosition(std::make_shared<ControllerPosition>(
                3, "LON_SC_CTR", 132.600, std::vector<std::string>{"EGKK"}, true, false));
        }

        ControllerPositionCollection controllers;
        NiceMock<MockTaskRunnerInterface> mockTaskRunner;
        NiceMock<MockApiInterface> mockApi;
        NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
        NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
        std::shared_ptr<MockEuroScopeCFlightPlanInterface> mockPluginReturnedFlightplan;
        std::shared_ptr<PrenoteMessageCollection> collection;
        CancelPrenoteMessageMenu menu;
    };

    TEST_F(CancelPrenoteMessageMenuTest, ItDeletesThePrenoteMessage)
    {
        ON_CALL(mockPlugin, GetSelectedFlightplan()).WillByDefault(testing::Return(mockPluginReturnedFlightplan));

        EXPECT_CALL(mockApi, DeletePrenoteMessage(1)).Times(1);

        menu.ControllerForPrenoteDeletionSelected("LON_S_CTR");

        EXPECT_EQ(nullptr, collection->GetById(1));
    }

    TEST_F(CancelPrenoteMessageMenuTest, ItHandlesApiExceptionsWhenDeletingPrenotes)
    {
        ON_CALL(mockPlugin, GetSelectedFlightplan()).WillByDefault(testing::Return(mockPluginReturnedFlightplan));

        EXPECT_CALL(mockApi, DeletePrenoteMessage(1)).Times(1).WillOnce(testing::Throw(ApiException("Foo")));

        menu.ControllerForPrenoteDeletionSelected("LON_S_CTR");

        EXPECT_NE(nullptr, collection->GetById(1));
    }

    TEST_F(CancelPrenoteMessageMenuTest, ItDoesntDoAnythingIfControllerNotFound)
    {
        ON_CALL(mockPlugin, GetSelectedFlightplan()).WillByDefault(testing::Return(mockPluginReturnedFlightplan));

        EXPECT_CALL(mockApi, DeletePrenoteMessage(testing::_)).Times(0);

        menu.ControllerForPrenoteDeletionSelected("LON_CTR");

        EXPECT_NE(nullptr, collection->GetById(1));
    }

    TEST_F(CancelPrenoteMessageMenuTest, ItDoesntDoAnythingIfPrenoteNotFound)
    {
        this->collection->Remove(1);

        ON_CALL(mockPlugin, GetSelectedFlightplan()).WillByDefault(testing::Return(mockPluginReturnedFlightplan));

        EXPECT_CALL(mockApi, DeletePrenoteMessage(testing::_)).Times(0);

        menu.ControllerForPrenoteDeletionSelected("LON_S_CTR");
    }

    TEST_F(CancelPrenoteMessageMenuTest, ItDoesntDoAnythingIfNoFlightplan)
    {
        ON_CALL(mockPlugin, GetSelectedFlightplan()).WillByDefault(testing::Return(nullptr));

        EXPECT_CALL(mockApi, DeletePrenoteMessage(testing::_)).Times(0);

        menu.ControllerForPrenoteDeletionSelected("LON_S_CTR");
        EXPECT_NE(nullptr, collection->GetById(1));
    }

    TEST_F(CancelPrenoteMessageMenuTest, ItDisplaysTheCancellationMenuWithControllers)
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
        EXPECT_CALL(mockPlugin, TriggerPopupList(RectEq(expectedArea), "Cancel Prenote", 1)).Times(1);

        EXPECT_CALL(mockPlugin, AddItemToPopupList(menuItemControllerOne)).Times(1);

        EXPECT_CALL(mockPlugin, AddItemToPopupList(menuItemControllerTwo)).Times(1);

        menu.DisplayPrenoteToDeleteMenu(mockFlightplan, {0, 0});
    }

    TEST_F(CancelPrenoteMessageMenuTest, ItDoesntDisplayTheCancelMenuIfNoReleases)
    {
        this->collection->Remove(1);
        this->collection->Remove(2);
        EXPECT_CALL(mockPlugin, TriggerPopupList(testing::_, testing::_, testing::_)).Times(0);

        menu.DisplayPrenoteToDeleteMenu(mockFlightplan, {0, 0});
    }
} // namespace UKControllerPluginTest::Prenote
