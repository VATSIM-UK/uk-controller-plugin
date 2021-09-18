#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "euroscope/UserSetting.h"
#include "mock/MockApiInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockEuroscopeRadarScreenLoopbackInterface.h"
#include "mock/MockTaskRunnerInterface.h"
#include "mock/MockUserSettingProviderInterface.h"
#include "prenote/AcknowledgePrenoteMessage.h"
#include "prenote/PendingPrenoteList.h"
#include "prenote/PrenoteMessage.h"
#include "prenote/PrenoteMessageCollection.h"
#include "time/SystemClock.h"

using testing::Test;
using UKControllerPlugin::Prenote::AcknowledgePrenoteMessage;
using UKControllerPlugin::Prenote::PendingPrenoteList;
using UKControllerPlugin::Prenote::PrenoteMessage;
using UKControllerPlugin::Prenote::PrenoteMessageCollection;
using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPluginTest::Prenote {

    class PendingPrenoteListTest : public Test
    {
        public:
        PendingPrenoteListTest()
            : messages(std::make_shared<PrenoteMessageCollection>()),
              acknowledge(std::make_shared<AcknowledgePrenoteMessage>(messages, activeCallsigns, mockTaskRunner, api)),
              userSettings(mockAsrProvider), list(messages, acknowledge, mockPlugin, controllers, activeCallsigns, 1)
        {
            this->pluginReturnedFlightplan =
                std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(*this->pluginReturnedFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));

            // Add positions and releases
            auto message = std::make_shared<PrenoteMessage>(1, "BAW123", "EGGD", "BADIM1X", "EGLL", 3, 2, TimeNow());
            messages->Add(message);
            auto position = std::make_shared<UKControllerPlugin::Controller::ControllerPosition>(
                2, "EGFF_APP", 125.850, std::vector<std::string>{"EGGD", "EGFF"}, true, true);
            controllers.AddPosition(position);
            auto controllerCallsign =
                std::make_shared<UKControllerPlugin::Controller::ActiveCallsign>("EGFF_APP", "Test 1", *position);
            this->activeCallsigns.AddUserCallsign(*controllerCallsign);
        }

        testing::NiceMock<Api::MockApiInterface> api;
        testing::NiceMock<TaskManager::MockTaskRunnerInterface> mockTaskRunner;
        std::shared_ptr<PrenoteMessageCollection> messages;
        UKControllerPlugin::Controller::ActiveCallsignCollection activeCallsigns;
        std::shared_ptr<PrenoteMessage> message;
        std::shared_ptr<AcknowledgePrenoteMessage> acknowledge;
        testing::NiceMock<Euroscope::MockUserSettingProviderInterface> mockAsrProvider;
        UKControllerPlugin::Euroscope::UserSetting userSettings;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> pluginReturnedFlightplan;
        UKControllerPlugin::Controller::ControllerPositionCollection controllers;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> mockPlugin;
        testing::NiceMock<Euroscope::MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
        PendingPrenoteList list;
    };

    TEST_F(PendingPrenoteListTest, LeftClickCollapseButtonTogglesCollapsedContent)
    {
        this->list.LeftClick(mockRadarScreen, 1, "collapseButton", {}, {});
        EXPECT_TRUE(this->list.ContentCollapsed());
        this->list.LeftClick(mockRadarScreen, 1, "collapseButton", {}, {});
        EXPECT_FALSE(this->list.ContentCollapsed());
        this->list.LeftClick(mockRadarScreen, 1, "collapseButton", {}, {});
        EXPECT_TRUE(this->list.ContentCollapsed());
    }

    TEST_F(PendingPrenoteListTest, LeftClickCloseButtonClosesWindow)
    {
        EXPECT_FALSE(this->list.IsVisible());
        this->list.ToggleVisible();
        EXPECT_TRUE(this->list.IsVisible());
        this->list.LeftClick(mockRadarScreen, 1, "closeButton", {}, {});
        EXPECT_FALSE(this->list.IsVisible());
        this->list.LeftClick(mockRadarScreen, 1, "closeButton", {}, {});
        EXPECT_FALSE(this->list.IsVisible());
    }

    TEST_F(PendingPrenoteListTest, LeftClickACallsignAcknowledgesTheMessage)
    {
        ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
            .WillByDefault(Return(this->pluginReturnedFlightplan));

        EXPECT_CALL(api, AcknowledgePrenoteMessage(1, 2)).Times(1);

        this->list.LeftClick(mockRadarScreen, 1, "BAW123", {}, {});
    }

    TEST_F(PendingPrenoteListTest, LeftClickACallsignDoesntToggleMenuIfNoFlightplan)
    {
        ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(nullptr));

        EXPECT_CALL(api, AcknowledgePrenoteMessage(testing::_, testing::_)).Times(0);

        this->list.LeftClick(mockRadarScreen, 1, "BAW123", {}, {});
    }

    TEST_F(PendingPrenoteListTest, ItLoadsDefaultVisiblityFromAsr)
    {
        EXPECT_CALL(mockAsrProvider, GetKey(testing::_)).WillRepeatedly(testing::Return(""));

        EXPECT_CALL(mockAsrProvider, GetKey("pendingPrenoteMessageListVisible")).Times(1).WillOnce(testing::Return(""));

        this->list.AsrLoadedEvent(userSettings);

        EXPECT_FALSE(this->list.IsVisible());
    }

    TEST_F(PendingPrenoteListTest, ItLoadsVisibilityFromAsr)
    {
        EXPECT_CALL(mockAsrProvider, GetKey(testing::_)).WillRepeatedly(testing::Return(""));

        EXPECT_CALL(mockAsrProvider, GetKey("pendingPrenoteMessageListVisible"))
            .Times(1)
            .WillOnce(testing::Return("1"));

        this->list.AsrLoadedEvent(userSettings);

        EXPECT_TRUE(this->list.IsVisible());
    }

    TEST_F(PendingPrenoteListTest, ItLoadsDefaultContentCollapseFromAsr)
    {
        EXPECT_CALL(mockAsrProvider, GetKey(testing::_)).WillRepeatedly(testing::Return(""));

        EXPECT_CALL(mockAsrProvider, GetKey("pendingPrenoteMessageListContentCollapsed"))
            .Times(1)
            .WillOnce(testing::Return(""));

        this->list.AsrLoadedEvent(userSettings);

        EXPECT_FALSE(this->list.ContentCollapsed());
    }

    TEST_F(PendingPrenoteListTest, ItLoadsContentCollapsedFromAsr)
    {
        EXPECT_CALL(mockAsrProvider, GetKey(testing::_)).WillRepeatedly(testing::Return(""));

        EXPECT_CALL(mockAsrProvider, GetKey("pendingPrenoteMessageListContentCollapsed"))
            .Times(1)
            .WillOnce(testing::Return("1"));

        this->list.AsrLoadedEvent(userSettings);

        EXPECT_TRUE(this->list.ContentCollapsed());
    }

    TEST_F(PendingPrenoteListTest, ItLoadsDefaultPositionFromAsr)
    {
        EXPECT_CALL(mockAsrProvider, GetKey(testing::_)).WillRepeatedly(testing::Return(""));

        EXPECT_CALL(mockAsrProvider, GetKey("pendingPrenoteMessageListXPosition"))
            .Times(1)
            .WillOnce(testing::Return(""));

        EXPECT_CALL(mockAsrProvider, GetKey("pendingPrenoteMessageListYPosition"))
            .Times(1)
            .WillOnce(testing::Return(""));

        this->list.AsrLoadedEvent(userSettings);

        EXPECT_TRUE(this->list.Position().Equals(Gdiplus::PointF{100.0F, 100.0F}));
    }

    TEST_F(PendingPrenoteListTest, ItLoadsPositionFromAsr)
    {
        EXPECT_CALL(mockAsrProvider, GetKey(testing::_)).WillRepeatedly(testing::Return(""));

        EXPECT_CALL(mockAsrProvider, GetKey("pendingPrenoteMessageListXPosition"))
            .Times(1)
            .WillOnce(testing::Return("250"));

        EXPECT_CALL(mockAsrProvider, GetKey("pendingPrenoteMessageListYPosition"))
            .Times(1)
            .WillOnce(testing::Return("150"));

        this->list.AsrLoadedEvent(userSettings);

        EXPECT_TRUE(this->list.Position().Equals(Gdiplus::PointF{250.0F, 150.0F}));
    }

    TEST_F(PendingPrenoteListTest, ItSavesEverythingToAsrOnClose)
    {
        this->list.Move({100, 200, 300, 400}, "");
        EXPECT_CALL(mockAsrProvider, SetKey("pendingPrenoteMessageListVisible", "Pending Prenote List Visible", "0"))
            .Times(1);

        EXPECT_CALL(
            mockAsrProvider,
            SetKey("pendingPrenoteMessageListContentCollapsed", "Pending Prenote List Content Collapsed", "0"))
            .Times(1);

        EXPECT_CALL(
            mockAsrProvider, SetKey("pendingPrenoteMessageListXPosition", "Pending Prenote List X Position", "100"))
            .Times(1);

        EXPECT_CALL(
            mockAsrProvider, SetKey("pendingPrenoteMessageListYPosition", "Pending Prenote List Y Position", "200"))
            .Times(1);

        this->list.AsrClosingEvent(userSettings);
    }
} // namespace UKControllerPluginTest::Prenote
