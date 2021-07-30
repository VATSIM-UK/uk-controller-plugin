#pragma once
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "pch/pch.h"
#include "controller/ControllerPositionCollection.h"
#include "dialog/DialogManager.h"
#include "euroscope/UserSetting.h"
#include "mock/MockApiInterface.h"
#include "mock/MockDialogProvider.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "releases/DepartureReleaseDecisionList.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockEuroscopeRadarScreenLoopbackInterface.h"
#include "mock/MockTaskRunnerInterface.h"
#include "mock/MockUserSettingProviderInterface.h"
#include "releases/DepartureReleaseEventHandler.h"
#include "releases/DepartureReleaseRequest.h"
#include "mock/MockWinApi.h"

using testing::Test;

namespace UKControllerPluginTest {
    namespace Releases {

        class DepartureReleaseDecisionListTest : public Test
        {
            public:
                DepartureReleaseDecisionListTest()
                    : handler(
                          mockApi,
                          taskRunner,
                          mockPlugin,
                          controllers,
                          activeCallsigns,
                          dialogManager,
                          windows,
                          101,
                          102,
                          103,
                          104
                      ),
                      list(
                          new UKControllerPlugin::Releases::DepartureReleaseDecisionList(
                              handler,
                              mockPlugin,
                              controllers,
                              3
                          )
                      ),
                      dialogManager(dialogProvider),
                      userSettings(mockAsrProvider)
                {
                    this->pluginReturnedFlightplan =
                        std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();

                    ON_CALL(*this->pluginReturnedFlightplan, GetCallsign)
                        .WillByDefault(testing::Return("BAW123"));

                    this->pluginReturnedRadarTarget =
                        std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();

                    // Add positions and releases
                    auto request = std::make_shared<UKControllerPlugin::Releases::DepartureReleaseRequest>(
                        1,
                        "BAW123",
                        3,
                        2,
                        std::chrono::system_clock::now() + std::chrono::minutes(5)
                    );
                    handler.AddReleaseRequest(request);
                    auto position = std::make_shared<UKControllerPlugin::Controller::ControllerPosition>(
                        2,
                        "EGFF_APP",
                        125.850,
                        std::vector<std::string>{"EGGD", "EGFF"},
                        true,
                        true
                    );
                    controllers.AddPosition(position);
                    auto controllerCallsign = std::make_shared<UKControllerPlugin::Controller::ActiveCallsign>(
                        "EGFF_APP",
                        "Test 1",
                        *position
                    );
                    this->activeCallsigns.AddUserCallsign(*controllerCallsign);
                }

                testing::NiceMock<Euroscope::MockUserSettingProviderInterface> mockAsrProvider;
                UKControllerPlugin::Euroscope::UserSetting userSettings;
                std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>
                pluginReturnedFlightplan;
                std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>
                pluginReturnedRadarTarget;
                std::shared_ptr<UKControllerPlugin::Releases::DepartureReleaseDecisionList> list;
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

        TEST_F(DepartureReleaseDecisionListTest, LeftClickCollapseButtonTogglesCollapsedContent)
        {
            this->list->LeftClick(
                mockRadarScreen,
                1,
                "collapseButton",
                {},
                {}
            );
            EXPECT_TRUE(this->list->ContentCollapsed());
            this->list->LeftClick(
                mockRadarScreen,
                1,
                "collapseButton",
                {},
                {}
            );
            EXPECT_FALSE(this->list->ContentCollapsed());
            this->list->LeftClick(
                mockRadarScreen,
                1,
                "collapseButton",
                {},
                {}
            );
            EXPECT_TRUE(this->list->ContentCollapsed());
        }

        TEST_F(DepartureReleaseDecisionListTest, LeftClickCloseButtonClosesWindow)
        {
            EXPECT_FALSE(this->list->IsVisible());
            this->list->ToggleVisible();
            EXPECT_TRUE(this->list->IsVisible());
            this->list->LeftClick(
                mockRadarScreen,
                1,
                "closeButton",
                {},
                {}
            );
            EXPECT_FALSE(this->list->IsVisible());
            this->list->LeftClick(
                mockRadarScreen,
                1,
                "closeButton",
                {},
                {}
            );
            EXPECT_FALSE(this->list->IsVisible());
        }

        TEST_F(DepartureReleaseDecisionListTest, LeftClickACallsignTogglesDecisionMenu)
        {
            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->pluginReturnedFlightplan));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("BAW123"))
                .WillByDefault(Return(this->pluginReturnedRadarTarget));

            EXPECT_CALL(this->mockPlugin, TriggerPopupList(testing::_, "Departure Release Decision", 1))
                .Times(1);

            this->list->LeftClick(
                mockRadarScreen,
                1,
                "BAW123",
                {},
                {}
            );
        }

        TEST_F(DepartureReleaseDecisionListTest, LeftClickACallsignDoesntToggleMenuIfNoFlightplan)
        {
            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(testing::Return(nullptr));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("BAW123"))
                .WillByDefault(Return(this->pluginReturnedRadarTarget));

            EXPECT_CALL(this->mockPlugin, TriggerPopupList(testing::_, "Departure Release Decision", 1))
            .Times(0);

            this->list->LeftClick(
                mockRadarScreen,
                1,
                "BAW123",
                {},
                {}
            );
        }

        TEST_F(DepartureReleaseDecisionListTest, LeftClickACallsignDoesntToggleMenuIfNoRadarTarget)
        {
            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->pluginReturnedFlightplan));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("BAW123"))
                .WillByDefault(testing::Return(nullptr));

            EXPECT_CALL(this->mockPlugin, TriggerPopupList(testing::_, "Departure Release Decision", 1))
            .Times(0);

            this->list->LeftClick(
                mockRadarScreen,
                1,
                "BAW123",
                {},
                {}
            );
        }

        TEST_F(DepartureReleaseDecisionListTest, ItLoadsDefaultVisiblityFromAsr)
        {
            EXPECT_CALL(mockAsrProvider, GetKey(testing::_))
                .WillRepeatedly(testing::Return(""));

            EXPECT_CALL(mockAsrProvider, GetKey("departureReleaseRequestListVisible"))
                .Times(1)
                .WillOnce(testing::Return(""));

            this->list->AsrLoadedEvent(userSettings);

            EXPECT_FALSE(this->list->IsVisible());
        }

        TEST_F(DepartureReleaseDecisionListTest, ItLoadsVisibilityFromAsr)
        {
            EXPECT_CALL(mockAsrProvider, GetKey(testing::_))
                .WillRepeatedly(testing::Return(""));

            EXPECT_CALL(mockAsrProvider, GetKey("departureReleaseRequestListVisible"))
                .Times(1)
                .WillOnce(testing::Return("1"));

            this->list->AsrLoadedEvent(userSettings);

            EXPECT_TRUE(this->list->IsVisible());
        }

        TEST_F(DepartureReleaseDecisionListTest, ItLoadsDefaultContentCollapseFromAsr)
        {
            EXPECT_CALL(mockAsrProvider, GetKey(testing::_))
                .WillRepeatedly(testing::Return(""));

            EXPECT_CALL(mockAsrProvider, GetKey("departureReleaseRequestListContentCollapsed"))
                .Times(1)
                .WillOnce(testing::Return(""));

            this->list->AsrLoadedEvent(userSettings);

            EXPECT_FALSE(this->list->ContentCollapsed());
        }

        TEST_F(DepartureReleaseDecisionListTest, ItLoadsContentCollapsedFromAsr)
        {
            EXPECT_CALL(mockAsrProvider, GetKey(testing::_))
                .WillRepeatedly(testing::Return(""));

            EXPECT_CALL(mockAsrProvider, GetKey("departureReleaseRequestListContentCollapsed"))
                .Times(1)
                .WillOnce(testing::Return("1"));

            this->list->AsrLoadedEvent(userSettings);

            EXPECT_TRUE(this->list->ContentCollapsed());
        }

        TEST_F(DepartureReleaseDecisionListTest, ItLoadsDefaultPositionFromAsr)
        {
            EXPECT_CALL(mockAsrProvider, GetKey(testing::_))
                .WillRepeatedly(testing::Return(""));

            EXPECT_CALL(mockAsrProvider, GetKey("departureReleaseRequestListXPosition"))
                .Times(1)
                .WillOnce(testing::Return(""));

            EXPECT_CALL(mockAsrProvider, GetKey("departureReleaseRequestListYPosition"))
                .Times(1)
                .WillOnce(testing::Return(""));

            this->list->AsrLoadedEvent(userSettings);

            EXPECT_TRUE(this->list->Position().Equals(Gdiplus::PointF{100.0f, 100.0f}));
        }

        TEST_F(DepartureReleaseDecisionListTest, ItLoadsPositionFromAsr)
        {
            EXPECT_CALL(mockAsrProvider, GetKey(testing::_))
                .WillRepeatedly(testing::Return(""));

            EXPECT_CALL(mockAsrProvider, GetKey("departureReleaseRequestListXPosition"))
                .Times(1)
            .WillOnce(testing::Return("250"));

            EXPECT_CALL(mockAsrProvider, GetKey("departureReleaseRequestListYPosition"))
                .Times(1)
                .WillOnce(testing::Return("150"));

            this->list->AsrLoadedEvent(userSettings);

            EXPECT_TRUE(this->list->Position().Equals(Gdiplus::PointF{250.0f, 150.0f}));
        }

        TEST_F(DepartureReleaseDecisionListTest, ItSavesEverythingToAsrOnClose)
        {
            this->list->Move({100, 200, 300, 400}, "");
            EXPECT_CALL(
                mockAsrProvider,
                SetKey("departureReleaseRequestListVisible", "Departure Release Request List Visible", "0")
            )
                .Times(1);

            EXPECT_CALL(
                mockAsrProvider,
                SetKey(
                    "departureReleaseRequestListContentCollapsed",
                    "Departure Release Request List Content Collapsed",
                    "0"
                )
            )
                .Times(1);

            EXPECT_CALL(
                mockAsrProvider,
                SetKey(
                    "departureReleaseRequestListXPosition",
                    "Departure Release Request List X Position",
                    "100"
                )
            )
                .Times(1);

            EXPECT_CALL(
                mockAsrProvider,
                SetKey(
                    "departureReleaseRequestListYPosition",
                    "Departure Release Request List Y Position",
                    "200"
                )
            )
                .Times(1);

            this->list->AsrClosingEvent(userSettings);
        }
    } // namespace Releases
} // namespace UKControllerPluginTest
