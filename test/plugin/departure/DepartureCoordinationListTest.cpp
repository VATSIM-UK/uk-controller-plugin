#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "departure/DepartureCoordinationList.h"
#include "dialog/DialogManager.h"
#include "euroscope/UserSetting.h"
#include "prenote/PrenoteMessageCollection.h"
#include "releases/DepartureReleaseEventHandler.h"
#include "releases/DepartureReleaseRequest.h"

using testing::Test;
using UKControllerPlugin::Departure::DepartureCoordinationList;
using UKControllerPlugin::Prenote::PrenoteMessageCollection;

namespace UKControllerPluginTest::Departure {

    class DepartureCoordinationListTest : public Test
    {
        public:
        DepartureCoordinationListTest()
            : userSettings(mockAsrProvider),
              list(std::make_shared<DepartureCoordinationList>(handler, prenotes, mockPlugin, controllers, 3)),
              handler(mockApi, taskRunner, mockPlugin, controllers, activeCallsigns, dialogManager, windows, 103, 104),
              dialogManager(dialogProvider)
        {
            this->pluginReturnedFlightplan =
                std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(*this->pluginReturnedFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));

            this->pluginReturnedRadarTarget =
                std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();

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

        PrenoteMessageCollection prenotes;
        testing::NiceMock<Euroscope::MockUserSettingProviderInterface> mockAsrProvider;
        UKControllerPlugin::Euroscope::UserSetting userSettings;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> pluginReturnedFlightplan;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>> pluginReturnedRadarTarget;
        std::shared_ptr<DepartureCoordinationList> list;
        UKControllerPlugin::Releases::DepartureReleaseEventHandler handler;
        UKControllerPlugin::Controller::ActiveCallsignCollection activeCallsigns;
        testing::NiceMock<Dialog::MockDialogProvider> dialogProvider;
        testing::NiceMock<Api::MockApiInterface> api;
        testing::NiceMock<Windows::MockWinApi> windows;
        UKControllerPlugin::Dialog::DialogManager dialogManager;
        UKControllerPlugin::Controller::ControllerPositionCollection controllers;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> mockPlugin;
        testing::NiceMock<Euroscope::MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
        testing::NiceMock<Api::MockApiInterface> mockApi;
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

    TEST_F(DepartureCoordinationListTest, LeftClickACallsignTogglesDecisionMenu)
    {
        ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
            .WillByDefault(Return(this->pluginReturnedFlightplan));

        ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("BAW123"))
            .WillByDefault(Return(this->pluginReturnedRadarTarget));

        EXPECT_CALL(this->mockPlugin, TriggerPopupList(testing::_, "Departure Release Decision", 1)).Times(1);

        this->list->LeftClick(mockRadarScreen, 1, "BAW123", {}, {});
    }

    TEST_F(DepartureCoordinationListTest, LeftClickACallsignDoesntToggleMenuIfNoFlightplan)
    {
        ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(nullptr));

        ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("BAW123"))
            .WillByDefault(Return(this->pluginReturnedRadarTarget));

        EXPECT_CALL(this->mockPlugin, TriggerPopupList(testing::_, "Departure Release Decision", 1)).Times(0);

        this->list->LeftClick(mockRadarScreen, 1, "BAW123", {}, {});
    }

    TEST_F(DepartureCoordinationListTest, LeftClickACallsignDoesntToggleMenuIfNoRadarTarget)
    {
        ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
            .WillByDefault(Return(this->pluginReturnedFlightplan));

        ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("BAW123")).WillByDefault(testing::Return(nullptr));

        EXPECT_CALL(this->mockPlugin, TriggerPopupList(testing::_, "Departure Release Decision", 1)).Times(0);

        this->list->LeftClick(mockRadarScreen, 1, "BAW123", {}, {});
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
