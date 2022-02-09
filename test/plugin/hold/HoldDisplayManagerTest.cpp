#include "aircraft/CallsignSelectionListFactory.h"
#include "dialog/DialogManager.h"
#include "euroscope/UserSetting.h"
#include "hold/AbstractHoldLevelRestriction.h"
#include "hold/DeemedSeparatedHold.h"
#include "hold/HoldDisplay.h"
#include "hold/HoldDisplayFactory.h"
#include "hold/HoldDisplayManager.h"
#include "hold/HoldManager.h"
#include "hold/PublishedHoldCollection.h"
#include "list/PopupListFactory.h"
#include "navaids/NavaidCollection.h"
#include "plugin/FunctionCallEventHandler.h"

using testing::_;
using testing::NiceMock;
using testing::Return;
using testing::Test;
using UKControllerPlugin::Aircraft::CallsignSelectionListFactory;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Hold::HoldDisplayFactory;
using UKControllerPlugin::Hold::HoldDisplayManager;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::PublishedHoldCollection;
using UKControllerPlugin::List::PopupListFactory;
using UKControllerPlugin::Navaids::Navaid;
using UKControllerPlugin::Navaids::NavaidCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;

namespace UKControllerPluginTest::Hold {

    class HoldDisplayManagerTest : public Test
    {
        public:
        HoldDisplayManagerTest()
            : popupFactory(functionHandlers, mockPlugin), listFactory(popupFactory), dialogManager(dialogProvider),
              userSetting(mockUserSettingProvider), holdManager(mockApi, taskRunner),
              displayFactory(mockPlugin, holdManager, navaids, holds, dialogManager, listFactory),
              displayManager(displayFactory)
        {
            this->navaids.AddNavaid({1, "TIMBA", EuroScopePlugIn::CPosition()});
            this->navaids.AddNavaid({2, "WILLO", EuroScopePlugIn::CPosition()});
            this->navaids.AddNavaid({3, "LAM", EuroScopePlugIn::CPosition()});
            this->holds.Add(std::move(holdData));
        }

        NiceMock<MockTaskRunnerInterface> taskRunner;
        NiceMock<MockApiInterface> mockApi;
        NiceMock<MockDialogProvider> dialogProvider;
        NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
        NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
        FunctionCallEventHandler functionHandlers;
        PopupListFactory popupFactory;
        CallsignSelectionListFactory listFactory;
        DialogManager dialogManager;
        UserSetting userSetting;
        HoldingData holdData = {1, "TIMBA", "TIMBA TEST", 7000, 15000, 360, "left", {}};
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
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("selectedHolds")).Times(1).WillRepeatedly(Return("TIMBA;WILLO"));

        EXPECT_CALL(mockUserSettingProvider, GetKey("holdTIMBAMinimised")).Times(1).WillRepeatedly(Return("1"));

        EXPECT_CALL(mockUserSettingProvider, GetKey("holdWILLOMinimised")).Times(1).WillRepeatedly(Return("0"));

        this->displayManager.AsrLoadedEvent(this->userSetting);
        EXPECT_EQ(2, this->displayManager.CountDisplays());
        EXPECT_EQ("TIMBA", this->displayManager.GetDisplay("TIMBA").navaid.identifier);
        EXPECT_EQ("WILLO", this->displayManager.GetDisplay("WILLO").navaid.identifier);
    }

    TEST_F(HoldDisplayManagerTest, ItSavesSelectedHoldsAndDisplaysToAsr)
    {
        // Just prove that the hold is being added to the list of displays and that its ASR
        // is being loaded
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("selectedHolds")).Times(1).WillRepeatedly(Return("TIMBA;WILLO"));

        EXPECT_CALL(mockUserSettingProvider, GetKey("holdTIMBAMinimised")).Times(1).WillRepeatedly(Return("1"));

        EXPECT_CALL(mockUserSettingProvider, GetKey("holdWILLOMinimised")).Times(1).WillRepeatedly(Return("0"));

        this->displayManager.AsrLoadedEvent(this->userSetting);

        // Save the display and assert that the display is trying to save itself properly
        EXPECT_CALL(mockUserSettingProvider, SetKey(_, _, _)).Times(8);

        EXPECT_CALL(mockUserSettingProvider, SetKey("selectedHolds", "Selected Holds", "TIMBA;WILLO")).Times(1);

        EXPECT_CALL(mockUserSettingProvider, SetKey("holdWILLOMinimised", "Hold WILLO Minimised", "0")).Times(1);

        EXPECT_CALL(mockUserSettingProvider, SetKey("holdTIMBAMinimised", "Hold TIMBA Minimised", "1")).Times(1);

        displayManager.AsrClosingEvent(this->userSetting);
    }

    TEST_F(HoldDisplayManagerTest, ItLoadsSelectedHolds)
    {
        // Just prove that the hold is being added to the list of displays and that its ASR
        // is being loaded
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("selectedHolds")).Times(1).WillRepeatedly(Return("TIMBA"));

        EXPECT_CALL(mockUserSettingProvider, GetKey("holdTIMBAMinimised")).Times(1).WillRepeatedly(Return("1"));

        this->displayManager.AsrLoadedEvent(this->userSetting);

        // Load the new holds
        EXPECT_CALL(mockUserSettingProvider, GetKey("holdLAMMinimised")).Times(1).WillRepeatedly(Return("1"));
        this->displayManager.LoadSelectedHolds(std::vector<std::string>({"LAM"}));
        EXPECT_EQ(1, this->displayManager.CountDisplays());
        EXPECT_EQ("LAM", this->displayManager.GetDisplay("LAM").navaid.identifier);
    }

    TEST_F(HoldDisplayManagerTest, ItSavesAHoldDisplaysWhenLoadingNewSelection)
    {
        // Just prove that the hold is being added to the list of displays and that its ASR
        // is being loaded
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("selectedHolds")).Times(1).WillRepeatedly(Return("TIMBA"));

        EXPECT_CALL(mockUserSettingProvider, GetKey("holdTIMBAMinimised")).Times(1).WillRepeatedly(Return("1"));

        this->displayManager.AsrLoadedEvent(this->userSetting);

        // Load the new selection
        EXPECT_CALL(mockUserSettingProvider, SetKey(_, _, _)).Times(4);

        EXPECT_CALL(mockUserSettingProvider, SetKey("holdTIMBAMinimised", "Hold TIMBA Minimised", "1")).Times(1);

        EXPECT_CALL(mockUserSettingProvider, GetKey("holdLAMMinimised")).Times(1).WillRepeatedly(Return("1"));

        this->displayManager.LoadSelectedHolds(std::vector<std::string>({"LAM"}));
        EXPECT_EQ(1, this->displayManager.CountDisplays());
        EXPECT_EQ("LAM", this->displayManager.GetDisplay("LAM").navaid.identifier);
    }

    TEST_F(HoldDisplayManagerTest, ItDoesntLoadNonExistantHoldDisplays)
    {
        // Just prove that the hold is being added to the list of displays and that its ASR
        // is being loaded
        this->displayManager.LoadSelectedHolds(std::vector<std::string>({"UWUT"}));
        EXPECT_EQ(0, this->displayManager.CountDisplays());
    }
} // namespace UKControllerPluginTest::Hold
