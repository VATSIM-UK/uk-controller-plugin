#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "departure/DepartureCoordinationList.h"
#include "dialog/DialogManager.h"
#include "euroscope/UserSetting.h"
#include "message/UserMessager.h"
#include "prenote/PrenoteMessageCollection.h"
#include "releases/DepartureReleaseEventHandler.h"
#include "releases/DepartureReleaseRequest.h"

using testing::Test;
using UKControllerPlugin::Departure::DepartureCoordinationList;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Prenote::PrenoteMessageCollection;

namespace UKControllerPluginTest::Departure {

    class DepartureCoordinationListTest : public Test
    {
        public:
        DepartureCoordinationListTest()
            : userSettings(mockAsrProvider), messager(mockPlugin),
              list(std::make_shared<DepartureCoordinationList>(
                  handler, prenotes, mockPlugin, controllers, activeCallsigns, 3)),
              handler(
                  mockApi,
                  taskRunner,
                  mockPlugin,
                  controllers,
                  activeCallsigns,
                  dialogManager,
                  windows,
                  messager,
                  103,
                  104),
              dialogManager(dialogProvider)
        {
            // Add positions and releases
            auto request = std::make_shared<UKControllerPlugin::Releases::DepartureReleaseRequest>(
                1, "BAW123", 3, 2, std::chrono::system_clock::now() + std::chrono::minutes(5));
            handler.AddReleaseRequest(request);
            auto position = std::make_shared<UKControllerPlugin::Controller::ControllerPosition>(
                2, "EGFF_APP", 125.850, std::vector<std::string>{"EGGD", "EGFF"}, true, true);
            controllers.AddPosition(position);
            auto controllerCallsign =
                std::make_shared<UKControllerPlugin::Controller::ActiveCallsign>("EGFF_APP", "Test 1", *position, true);
            this->activeCallsigns.AddUserCallsign(*controllerCallsign);
        }

        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> mockPlugin;
        testing::NiceMock<Euroscope::MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
        testing::NiceMock<Api::MockApiInterface> mockApi;
        PrenoteMessageCollection prenotes;
        testing::NiceMock<Euroscope::MockUserSettingProviderInterface> mockAsrProvider;
        UKControllerPlugin::Euroscope::UserSetting userSettings;
        std::shared_ptr<DepartureCoordinationList> list;
        UserMessager messager;
        UKControllerPlugin::Releases::DepartureReleaseEventHandler handler;
        UKControllerPlugin::Controller::ActiveCallsignCollection activeCallsigns;
        testing::NiceMock<Dialog::MockDialogProvider> dialogProvider;
        testing::NiceMock<Api::MockApiInterface> api;
        testing::NiceMock<Windows::MockWinApi> windows;
        UKControllerPlugin::Dialog::DialogManager dialogManager;
        UKControllerPlugin::Controller::ControllerPositionCollection controllers;
        TaskManager::MockTaskRunnerInterface taskRunner;
    };

    TEST_F(DepartureCoordinationListTest, LeftClickCollapseButtonTogglesCollapsedContent)
    {
        this->list->LeftClick(mockRadarScreen, 1, "collapseButton", {}, {});
        EXPECT_TRUE(this->list->ContentCollapsed());
        this->list->LeftClick(mockRadarScreen, 1, "collapseButton", {}, {});
        EXPECT_FALSE(this->list->ContentCollapsed());
        this->list->LeftClick(mockRadarScreen, 1, "collapseButton", {}, {});
        EXPECT_TRUE(this->list->ContentCollapsed());
    }

    TEST_F(DepartureCoordinationListTest, LeftClickCloseButtonClosesWindow)
    {
        EXPECT_FALSE(this->list->IsVisible());
        this->list->ToggleVisible();
        EXPECT_TRUE(this->list->IsVisible());
        this->list->LeftClick(mockRadarScreen, 1, "closeButton", {}, {});
        EXPECT_FALSE(this->list->IsVisible());
        this->list->LeftClick(mockRadarScreen, 1, "closeButton", {}, {});
        EXPECT_FALSE(this->list->IsVisible());
    }

    TEST_F(DepartureCoordinationListTest, LeftClickAReleaseTogglesDecisionMenuTagFunction)
    {
        EXPECT_CALL(
            mockRadarScreen, TogglePluginTagFunction("BAW123", 9013, PointEq(POINT{1, 2}), RectEq(RECT{3, 4, 5, 6})))
            .Times(1);

        this->list->LeftClick(mockRadarScreen, 1, "Rls.BAW123", {1, 2}, {3, 4, 5, 6});
    }

    TEST_F(DepartureCoordinationListTest, LeftClickAPrenoteTogglesAcknowledgeTagFunction)
    {
        EXPECT_CALL(
            mockRadarScreen, TogglePluginTagFunction("BAW123", 9019, PointEq(POINT{1, 2}), RectEq(RECT{3, 4, 5, 6})))
            .Times(1);

        this->list->LeftClick(mockRadarScreen, 1, "Pre.BAW123", {1, 2}, {3, 4, 5, 6});
    }

    TEST_F(DepartureCoordinationListTest, ItLoadsDefaultVisiblityFromAsr)
    {
        EXPECT_CALL(mockAsrProvider, GetKey(testing::_)).WillRepeatedly(testing::Return(""));

        EXPECT_CALL(mockAsrProvider, GetKey("departureCoordinationListVisible")).Times(1).WillOnce(testing::Return(""));

        this->list->AsrLoadedEvent(userSettings);

        EXPECT_FALSE(this->list->IsVisible());
    }

    TEST_F(DepartureCoordinationListTest, ItLoadsVisibilityFromAsr)
    {
        EXPECT_CALL(mockAsrProvider, GetKey(testing::_)).WillRepeatedly(testing::Return(""));

        EXPECT_CALL(mockAsrProvider, GetKey("departureCoordinationListVisible"))
            .Times(1)
            .WillOnce(testing::Return("1"));

        this->list->AsrLoadedEvent(userSettings);

        EXPECT_TRUE(this->list->IsVisible());
    }

    TEST_F(DepartureCoordinationListTest, ItLoadsDefaultContentCollapseFromAsr)
    {
        EXPECT_CALL(mockAsrProvider, GetKey(testing::_)).WillRepeatedly(testing::Return(""));

        EXPECT_CALL(mockAsrProvider, GetKey("departureCoordinationListContentCollapsed"))
            .Times(1)
            .WillOnce(testing::Return(""));

        this->list->AsrLoadedEvent(userSettings);

        EXPECT_FALSE(this->list->ContentCollapsed());
    }

    TEST_F(DepartureCoordinationListTest, ItLoadsContentCollapsedFromAsr)
    {
        EXPECT_CALL(mockAsrProvider, GetKey(testing::_)).WillRepeatedly(testing::Return(""));

        EXPECT_CALL(mockAsrProvider, GetKey("departureCoordinationListContentCollapsed"))
            .Times(1)
            .WillOnce(testing::Return("1"));

        this->list->AsrLoadedEvent(userSettings);

        EXPECT_TRUE(this->list->ContentCollapsed());
    }

    TEST_F(DepartureCoordinationListTest, ItLoadsDefaultPositionFromAsr)
    {
        EXPECT_CALL(mockAsrProvider, GetKey(testing::_)).WillRepeatedly(testing::Return(""));

        EXPECT_CALL(mockAsrProvider, GetKey("departureCoordinationListXPosition"))
            .Times(1)
            .WillOnce(testing::Return(""));

        EXPECT_CALL(mockAsrProvider, GetKey("departureCoordinationListYPosition"))
            .Times(1)
            .WillOnce(testing::Return(""));

        this->list->AsrLoadedEvent(userSettings);

        EXPECT_TRUE(this->list->Position().Equals(Gdiplus::PointF{100.0f, 100.0f}));
    }

    TEST_F(DepartureCoordinationListTest, ItLoadsPositionFromAsr)
    {
        EXPECT_CALL(mockAsrProvider, GetKey(testing::_)).WillRepeatedly(testing::Return(""));

        EXPECT_CALL(mockAsrProvider, GetKey("departureCoordinationListXPosition"))
            .Times(1)
            .WillOnce(testing::Return("250"));

        EXPECT_CALL(mockAsrProvider, GetKey("departureCoordinationListYPosition"))
            .Times(1)
            .WillOnce(testing::Return("150"));

        this->list->AsrLoadedEvent(userSettings);

        EXPECT_TRUE(this->list->Position().Equals(Gdiplus::PointF{250.0f, 150.0f}));
    }

    TEST_F(DepartureCoordinationListTest, ItSavesEverythingToAsrOnClose)
    {
        this->list->Move({100, 200, 300, 400}, "");
        EXPECT_CALL(
            mockAsrProvider, SetKey("departureCoordinationListVisible", "Departure Coordination List Visible", "0"))
            .Times(1);

        EXPECT_CALL(
            mockAsrProvider,
            SetKey("departureCoordinationListContentCollapsed", "Departure Coordination List Content Collapsed", "0"))
            .Times(1);

        EXPECT_CALL(
            mockAsrProvider,
            SetKey("departureCoordinationListXPosition", "Departure Coordination List X Position", "100"))
            .Times(1);

        EXPECT_CALL(
            mockAsrProvider,
            SetKey("departureCoordinationListYPosition", "Departure Coordination List Y Position", "200"))
            .Times(1);

        this->list->AsrClosingEvent(userSettings);
    }
} // namespace UKControllerPluginTest::Departure
