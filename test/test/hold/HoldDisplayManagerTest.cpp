#include "pch/pch.h"
#include "hold/HoldDisplayManager.h"
#include "hold/HoldManager.h"
#include "hold/HoldDisplayFactory.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockApiInterface.h"
#include "mock/MockUserSettingProviderInterface.h"
#include "euroscope/UserSetting.h"
#include "mock/MockTaskRunnerInterface.h"
#include "navaids/NavaidCollection.h"
#include "navaids/Navaid.h"
#include "hold/PublishedHoldCollection.h"
#include "mock/MockDialogProvider.h"
#include "dialog/DialogManager.h"
#include "hold/HoldingData.h"

using UKControllerPlugin::Hold::HoldDisplayManager;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::HoldDisplayFactory;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;
using UKControllerPlugin::Hold::PublishedHoldCollection;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Navaids::NavaidCollection;
using UKControllerPlugin::Navaids::Navaid;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using testing::Test;
using testing::NiceMock;
using testing::Return;
using testing::_;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldDisplayManagerTest : public Test
        {
            public:
                HoldDisplayManagerTest()
                    : displayFactory(mockPlugin, holdManager, navaids, holds, dialogManager),
                    displayManager(holdManager, displayFactory), dialogManager(dialogProvider),
                    userSetting(mockUserSettingProvider), holdManager(mockApi, taskRunner)
                {
                    this->navaids.AddNavaid({ 1, "TIMBA", EuroScopePlugIn::CPosition() });
                    this->navaids.AddNavaid({ 2, "WILLO", EuroScopePlugIn::CPosition() });
                    this->navaids.AddNavaid({ 3, "LAM", EuroScopePlugIn::CPosition() });
                    this->holds.Add(std::move(holdData));
                }

                NiceMock<MockTaskRunnerInterface> taskRunner;
                NiceMock<MockApiInterface> mockApi;
                NiceMock<MockDialogProvider> dialogProvider;
                DialogManager dialogManager;
                NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                UserSetting userSetting;
                HoldingData holdData = { 1, "TIMBA", "TIMBA TEST", 7000, 15000, 360, "left", {} };
                PublishedHoldCollection holds;
                NavaidCollection navaids;
                HoldManager holdManager;
                HoldDisplayFactory displayFactory;
                HoldDisplayManager displayManager;
        };

        TEST_F(HoldDisplayManagerTest, ItLoadsDisplaysFromTheAsr)
        {
            // Just prove that the hold is being added to the list of displays and that its ASR
            // is being loaded
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey("selectedHolds"))
                .Times(1)
                .WillRepeatedly(Return("TIMBA;WILLO"));

            EXPECT_CALL(mockUserSettingProvider, GetKey("holdTIMBAMinimised"))
                .Times(1)
                .WillRepeatedly(Return("1"));

            EXPECT_CALL(mockUserSettingProvider, GetKey("holdWILLOMinimised"))
                .Times(1)
                .WillRepeatedly(Return("0"));

            this->displayManager.AsrLoadedEvent(this->userSetting);
            EXPECT_EQ(2, this->displayManager.CountDisplays());
            EXPECT_EQ("TIMBA", this->displayManager.GetDisplay("TIMBA").navaid.identifier);
            EXPECT_EQ("WILLO", this->displayManager.GetDisplay("WILLO").navaid.identifier);
        }

        TEST_F(HoldDisplayManagerTest, ItSavesSelectedHoldsAndDisplaysToAsr)
        {
            // Just prove that the hold is being added to the list of displays and that its ASR
            // is being loaded
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey("selectedHolds"))
                .Times(1)
                .WillRepeatedly(Return("TIMBA;WILLO"));

            EXPECT_CALL(mockUserSettingProvider, GetKey("holdTIMBAMinimised"))
                .Times(1)
                .WillRepeatedly(Return("1"));

            EXPECT_CALL(mockUserSettingProvider, GetKey("holdWILLOMinimised"))
                .Times(1)
                .WillRepeatedly(Return("0"));

            this->displayManager.AsrLoadedEvent(this->userSetting);

            // Save the display and assert that the display is trying to save itself properly
            EXPECT_CALL(mockUserSettingProvider, SetKey("selectedHolds", "Selected Holds", "TIMBA;WILLO"))
                .Times(1);

            EXPECT_CALL(mockUserSettingProvider, SetKey("holdWILLOMinimised", "Hold WILLO Minimised", "0"))
                .Times(1);

            EXPECT_CALL(mockUserSettingProvider, SetKey("holdTIMBAMinimised", "Hold TIMBA Minimised", "1"))
                .Times(1);

            displayManager.AsrClosingEvent(this->userSetting);
        }

        TEST_F(HoldDisplayManagerTest, ItLoadsSelectedHolds)
        {
            // Just prove that the hold is being added to the list of displays and that its ASR
            // is being loaded
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey("selectedHolds"))
                .Times(1)
                .WillRepeatedly(Return("TIMBA"));

            EXPECT_CALL(mockUserSettingProvider, GetKey("holdTIMBAMinimised"))
                .Times(1)
                .WillRepeatedly(Return("1"));

            this->displayManager.AsrLoadedEvent(this->userSetting);

            // Load the new holds
            EXPECT_CALL(mockUserSettingProvider, GetKey("holdLAMMinimised"))
                .Times(1)
                .WillRepeatedly(Return("1"));
            this->displayManager.LoadSelectedHolds(std::vector<std::string>({ "LAM" }));
            EXPECT_EQ(1, this->displayManager.CountDisplays());
            EXPECT_EQ("LAM", this->displayManager.GetDisplay("LAM").navaid.identifier);
        }

        TEST_F(HoldDisplayManagerTest, ItSavesAHoldDisplaysWhenLoadingNewSelection)
        {
            // Just prove that the hold is being added to the list of displays and that its ASR
            // is being loaded
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey("selectedHolds"))
                .Times(1)
                .WillRepeatedly(Return("TIMBA"));

            EXPECT_CALL(mockUserSettingProvider, GetKey("holdTIMBAMinimised"))
                .Times(1)
                .WillRepeatedly(Return("1"));

            this->displayManager.AsrLoadedEvent(this->userSetting);

            // Load the new selection
            EXPECT_CALL(mockUserSettingProvider, SetKey("selectedHolds", "Selected Holds", "TIMBA"))
                .Times(1);

            EXPECT_CALL(mockUserSettingProvider, SetKey("holdTIMBAMinimised", "Hold TIMBA Minimised", "1"))
                .Times(1);

            EXPECT_CALL(mockUserSettingProvider, GetKey("holdLAMMinimised"))
                .Times(1)
                .WillRepeatedly(Return("1"));

            this->displayManager.LoadSelectedHolds(std::vector<std::string>({ "LAM" }));
            EXPECT_EQ(1, this->displayManager.CountDisplays());
            EXPECT_EQ("LAM", this->displayManager.GetDisplay("LAM").navaid.identifier);
        }

        TEST_F(HoldDisplayManagerTest, ItDoesntLoadNonExistantHoldDisplays)
        {
            // Just prove that the hold is being added to the list of displays and that its ASR
            // is being loaded
            this->displayManager.LoadSelectedHolds(std::vector<std::string>({ "UWUT" }));
            EXPECT_EQ(0, this->displayManager.CountDisplays());
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
