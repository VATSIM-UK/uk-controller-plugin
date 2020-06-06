#include "pch/pch.h"
#include "hold/HoldDisplayManager.h"
#include "hold/HoldManager.h"
#include "hold/HoldDisplayFactory.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockApiInterface.h"
#include "mock/MockUserSettingProviderInterface.h"
#include "euroscope/UserSetting.h"

using UKControllerPlugin::Hold::HoldDisplayManager;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::HoldDisplayFactory;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPlugin::Euroscope::UserSetting;
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
                    : displayFactory(mockPlugin, holdManager), holdProfileManager(mockApi),
                    displayManager(holdProfileManager, holdManager, displayFactory),
                    userSetting(mockUserSettingProvider)
                {
                    this->holdProfileManager.AddProfile({ 1, "Test Profile", {1} });
                    this->holdProfileManager.AddProfile({ 2, "Test Profile 2", {1} });
                    this->holdProfileManager.AddProfile({ 3, "Test Profile 3", {55} });
                    this->holdManager.AddHold(ManagedHold({ 1, "TIMBA", "TIMBA", 7000, 15000, 300, "right", {} }));
                }

                NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
                NiceMock<MockApiInterface> mockApi;
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                UserSetting userSetting;
                HoldManager holdManager;
                HoldDisplayFactory displayFactory;
                HoldDisplayManager displayManager;
        };

        TEST_F(HoldDisplayManagerTest, ItDefaultsToNoProfileSet)
        {
            EXPECT_EQ(0, this->displayManager.GetCurrentProfile());
        }

        TEST_F(HoldDisplayManagerTest, ItDefaultsToNoProfileIfNoAsrProfile)
        {
            ON_CALL(mockUserSettingProvider, GetKey(this->displayManager.selectedProfileAsrKey))
                .WillByDefault(Return(""));

            ON_CALL(mockUserSettingProvider, KeyExists(this->displayManager.selectedProfileAsrKey))
                .WillByDefault(Return(false));

            this->displayManager.AsrLoadedEvent(this->userSetting);
            EXPECT_EQ(0, this->displayManager.GetCurrentProfile());
        }

        TEST_F(HoldDisplayManagerTest, ItDoesntLoadDisplaysIfNoProfile)
        {
            ON_CALL(mockUserSettingProvider, GetKey(this->displayManager.selectedProfileAsrKey))
                .WillByDefault(Return("0"));

            ON_CALL(mockUserSettingProvider, KeyExists(this->displayManager.selectedProfileAsrKey))
                .WillByDefault(Return(true));

            this->displayManager.AsrLoadedEvent(this->userSetting);
            EXPECT_EQ(0, this->displayManager.GetCurrentProfile());
        }

        TEST_F(HoldDisplayManagerTest, ItSetsCurrentProfile)
        {
            ON_CALL(mockUserSettingProvider, GetKey(this->displayManager.selectedProfileAsrKey))
                .WillByDefault(Return("1"));

            ON_CALL(mockUserSettingProvider, KeyExists(this->displayManager.selectedProfileAsrKey))
                .WillByDefault(Return(true));

            this->displayManager.AsrLoadedEvent(this->userSetting);
            EXPECT_EQ(1, this->displayManager.GetCurrentProfile());
        }

        TEST_F(HoldDisplayManagerTest, ItLoadsDisplaysForGivenProfile)
        {
            // Just prove that the hold is being added to the list of displays and that its ASR
            // is being loaded
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey("holdProfile1Hold1LevelsSkipped"))
                .Times(1)
                .WillRepeatedly(Return("1"));

            EXPECT_CALL(mockUserSettingProvider, GetKey(this->displayManager.selectedProfileAsrKey))
                .WillRepeatedly(Return("1"));

            EXPECT_CALL(mockUserSettingProvider, KeyExists(this->displayManager.selectedProfileAsrKey))
                .WillRepeatedly(Return(true));

            this->displayManager.AsrLoadedEvent(this->userSetting);
            EXPECT_EQ(1, this->displayManager.CountDisplays());
        }

        TEST_F(HoldDisplayManagerTest, ItSavesSelectedProfileAndDisplaysToAsr)
        {
            // Just prove that the hold is being added to the list of displays and that its ASR
            // is being loaded
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey("holdProfile1Hold1LevelsSkipped"))
                .Times(1)
                .WillRepeatedly(Return("1"));

            EXPECT_CALL(mockUserSettingProvider, GetKey(this->displayManager.selectedProfileAsrKey))
                .WillRepeatedly(Return("1"));

            EXPECT_CALL(mockUserSettingProvider, KeyExists(this->displayManager.selectedProfileAsrKey))
                .WillRepeatedly(Return(true));

            this->displayManager.AsrLoadedEvent(this->userSetting);

            // Save the display and assert that the display is trying to save itself properly
            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey("selectedHoldProfile", "Selected Hold Profile", "1")
            )
                .Times(1);

            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey("holdProfile1Hold1LevelsSkipped", "Hold Profile (Test Profile - TIMBA) Levels Skipped", "1")
            )
                .Times(1);

            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey("holdProfile1Hold1Minimised", "Hold Profile (Test Profile - TIMBA) Minimised", "0")
            )
                .Times(1);

            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey("holdProfile1Hold1PositionX", "Hold Profile (Test Profile - TIMBA) X Pos", "100")
            )
                .Times(1);

            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey("holdProfile1Hold1PositionY", "Hold Profile (Test Profile - TIMBA) Y Pos", "100")
            )
                .Times(1);

            displayManager.AsrClosingEvent(this->userSetting);
        }

        TEST_F(HoldDisplayManagerTest, ItLoadsAProfile)
        {
            // Just prove that the hold is being added to the list of displays and that its ASR
            // is being loaded
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey("holdProfile1Hold1LevelsSkipped"))
                .Times(1)
                .WillRepeatedly(Return("1"));

            EXPECT_CALL(mockUserSettingProvider, GetKey(this->displayManager.selectedProfileAsrKey))
                .WillRepeatedly(Return("1"));

            EXPECT_CALL(mockUserSettingProvider, KeyExists(this->displayManager.selectedProfileAsrKey))
                .WillRepeatedly(Return(true));

            this->displayManager.AsrLoadedEvent(this->userSetting);

            // Load the new profile
            EXPECT_CALL(mockUserSettingProvider, GetKey("holdProfile2Hold1LevelsSkipped"))
                .Times(1)
                .WillRepeatedly(Return("1"));
            this->displayManager.LoadProfile(2);
            EXPECT_EQ(2, this->displayManager.GetCurrentProfile());
            EXPECT_EQ(1, this->displayManager.CountDisplays());
        }

        TEST_F(HoldDisplayManagerTest, ItSavesAProfileWhenLoadingNewOne)
        {
            // Just prove that the hold is being added to the list of displays and that its ASR
            // is being loaded
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey("holdProfile1Hold1LevelsSkipped"))
                .Times(1)
                .WillRepeatedly(Return("1"));

            EXPECT_CALL(mockUserSettingProvider, GetKey(this->displayManager.selectedProfileAsrKey))
                .WillRepeatedly(Return("1"));

            EXPECT_CALL(mockUserSettingProvider, KeyExists(this->displayManager.selectedProfileAsrKey))
                .WillRepeatedly(Return(true));

            this->displayManager.AsrLoadedEvent(this->userSetting);

            // Load the new profile
            EXPECT_CALL(mockUserSettingProvider, GetKey("holdProfile2Hold1LevelsSkipped"))
                .Times(1)
                .WillRepeatedly(Return("1"));

            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey("holdProfile1Hold1LevelsSkipped", "Hold Profile (Test Profile - TIMBA) Levels Skipped", "1")
            )
                .Times(1);

            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey("holdProfile1Hold1Minimised", "Hold Profile (Test Profile - TIMBA) Minimised", "0")
            )
                .Times(1);

            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey("holdProfile1Hold1PositionX", "Hold Profile (Test Profile - TIMBA) X Pos", "100")
            )
                .Times(1);

            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey("holdProfile1Hold1PositionY", "Hold Profile (Test Profile - TIMBA) Y Pos", "100")
            )
                .Times(1);

            this->displayManager.LoadProfile(2);
            EXPECT_EQ(2, this->displayManager.GetCurrentProfile());
            EXPECT_EQ(1, this->displayManager.CountDisplays());
        }

        TEST_F(HoldDisplayManagerTest, ItDoesntLoadNonExistantHoldDisplays)
        {
            // Just prove that the hold is being added to the list of displays and that its ASR
            // is being loaded
            this->displayManager.LoadProfile(3);
            EXPECT_EQ(0, this->displayManager.CountDisplays());
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
