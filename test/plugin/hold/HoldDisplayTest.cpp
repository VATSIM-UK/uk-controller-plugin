#include "euroscope/UserSetting.h"
#include "hold/AbstractHoldLevelRestriction.h"
#include "hold/DeemedSeparatedHold.h"
#include "hold/HoldDisplay.h"
#include "navaids/Navaid.h"
#include "hold/HoldManager.h"
#include "dialog/DialogManager.h"
#include "dialog/DialogData.h"
#include "hold/HoldingAircraft.h"
#include "hold/CompareHoldingAircraft.h"
#include "hold/HoldingData.h"
#include "hold/PublishedHoldCollection.h"

using testing::_;
using testing::NiceMock;
using testing::Return;
using testing::Test;
using testing::Throw;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Hold::CompareHoldingAircraft;
using UKControllerPlugin::Hold::DeemedSeparatedHold;
using UKControllerPlugin::Hold::HoldDisplay;
using UKControllerPlugin::Hold::HoldingAircraft;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::PublishedHoldCollection;
using UKControllerPlugin::Navaids::Navaid;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopeRadarScreenLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldDisplayTest : public Test
        {
            public:
            HoldDisplayTest()
                : addAircraftList(std::make_shared<NiceMock<List::MockPopupList>>()), dialogManager(mockDialogProvider),
                  userSetting(mockUserSettingProvider), navaid({2, "TIMBA", EuroScopePlugIn::CPosition()}),
                  holdManager(mockApi, mockTaskRunner),
                  display(mockPlugin, holdManager, navaid, publishedHolds, dialogManager, addAircraftList)
            {
                this->dialogManager.AddDialog(this->holdDialogData);
                this->navaid.coordinates.LoadFromStrings("E000.15.42.000", "N050.56.44.000");

                this->timbaPosition.m_Latitude = 51.06722;
                this->timbaPosition.m_Longitude = 0.43944;
                this->willoPosition.m_Latitude = 50.985;
                this->willoPosition.m_Longitude = -0.19167;
                this->mayPosition.m_Latitude = 51.017200;
                this->mayPosition.m_Longitude = 0.116111;
            }

            EuroScopePlugIn::CPosition timbaPosition;
            EuroScopePlugIn::CPosition willoPosition;
            EuroScopePlugIn::CPosition mayPosition;
            PublishedHoldCollection publishedHolds;
            DialogData holdDialogData = {IDD_HOLD_PARAMS, "Test"};
            std::shared_ptr<NiceMock<List::MockPopupList>> addAircraftList;
            NiceMock<MockTaskRunnerInterface> mockTaskRunner;
            NiceMock<MockApiInterface> mockApi;
            NiceMock<MockDialogProvider> mockDialogProvider;
            DialogManager dialogManager;
            NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
            NiceMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
            NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
            UserSetting userSetting;
            Navaid navaid;
            HoldManager holdManager;
            HoldDisplay display;
        };

        TEST_F(HoldDisplayTest, ItLoadsMinimisedFromAsr)
        {
            ON_CALL(this->mockUserSettingProvider, GetKey("holdTIMBAMinimised")).WillByDefault(Return("1"));

            this->display.LoadDataFromAsr(userSetting);
            EXPECT_TRUE(this->display.IsMinimised());
        }

        TEST_F(HoldDisplayTest, ItDefaultsToNotMinimised)
        {
            ON_CALL(this->mockUserSettingProvider, GetKey("holdTIMBAMinimised")).WillByDefault(Return("abc"));

            this->display.LoadDataFromAsr(userSetting);
            EXPECT_FALSE(this->display.IsMinimised());
        }

        TEST_F(HoldDisplayTest, ItLoadsMinimumLevelFromAsr)
        {
            ON_CALL(this->mockUserSettingProvider, GetKey("holdTIMBAMinLevel")).WillByDefault(Return("10000"));

            this->display.LoadDataFromAsr(userSetting);
            EXPECT_EQ(10000, this->display.GetMinimumLevel());
        }

        TEST_F(HoldDisplayTest, ItLoadsMinimumLevelFromPublishedHoldIfNotInAsr)
        {
            this->publishedHolds.Add({1, "TIMBA", "TIMBA", 2000, 3000});

            HoldDisplay display2(mockPlugin, holdManager, navaid, publishedHolds, dialogManager, addAircraftList);
            ON_CALL(this->mockUserSettingProvider, GetKey("holdTIMBAMinLevel")).WillByDefault(Return(""));

            display2.LoadDataFromAsr(userSetting);
            EXPECT_EQ(2000, display2.GetMinimumLevel());
        }

        TEST_F(HoldDisplayTest, ItLoadsMinimumLevelFromDefaultIfNonePublished)
        {
            ON_CALL(this->mockUserSettingProvider, GetKey("holdTIMBAMinLevel")).WillByDefault(Return(""));

            this->display.LoadDataFromAsr(userSetting);
            EXPECT_EQ(7000, this->display.GetMinimumLevel());
        }

        TEST_F(HoldDisplayTest, ItLoadsMaximumLevelFromAsr)
        {
            ON_CALL(this->mockUserSettingProvider, GetKey("holdTIMBAMaxLevel")).WillByDefault(Return("19000"));

            this->display.LoadDataFromAsr(userSetting);
            EXPECT_EQ(19000, this->display.GetMaximumLevel());
        }

        TEST_F(HoldDisplayTest, ItLoadsMaximumLevelFromPublishedHoldIfNotInAsr)
        {
            this->publishedHolds.Add({1, "TIMBA", "TIMBA", 2000, 3000});
            HoldDisplay display2(mockPlugin, holdManager, navaid, publishedHolds, dialogManager, addAircraftList);
            ON_CALL(this->mockUserSettingProvider, GetKey("holdTIMBAMaxLevel")).WillByDefault(Return(""));

            display2.LoadDataFromAsr(userSetting);
            EXPECT_EQ(3000, display2.GetMaximumLevel());
        }

        TEST_F(HoldDisplayTest, ItLoadsMaximumLevelFromDefaultIfNonePublished)
        {
            ON_CALL(this->mockUserSettingProvider, GetKey("holdTIMBAMaxLevel")).WillByDefault(Return(""));

            this->display.LoadDataFromAsr(userSetting);
            EXPECT_EQ(15000, this->display.GetMaximumLevel());
        }

        TEST_F(HoldDisplayTest, ItLoadsWindowPositionFromAsr)
        {
            ON_CALL(this->mockUserSettingProvider, GetKey("holdTIMBAPositionX")).WillByDefault(Return("151"));

            ON_CALL(this->mockUserSettingProvider, GetKey("holdTIMBAPositionY")).WillByDefault(Return("262"));

            this->display.LoadDataFromAsr(userSetting);

            EXPECT_EQ(151, this->display.GetDisplayPos().x);
            EXPECT_EQ(262, this->display.GetDisplayPos().y);
        }

        TEST_F(HoldDisplayTest, ItDefaultsWindowPosition)
        {
            ON_CALL(this->mockUserSettingProvider, GetKey("holdTIMBAPositionX")).WillByDefault(Return(""));

            ON_CALL(this->mockUserSettingProvider, GetKey("holdTIMBAPositionY")).WillByDefault(Return(""));

            this->display.LoadDataFromAsr(userSetting);
            EXPECT_EQ(100, this->display.GetDisplayPos().x);
            EXPECT_EQ(100, this->display.GetDisplayPos().y);
        }

        TEST_F(HoldDisplayTest, ItSavesDisplayLevelsToAsr)
        {
            EXPECT_CALL(this->mockUserSettingProvider, SetKey(_, _, _)).Times(3);

            EXPECT_CALL(this->mockUserSettingProvider, SetKey("holdTIMBAMaxLevel", "Hold TIMBA Maximum Level", "9000"))
                .Times(1);

            EXPECT_CALL(this->mockUserSettingProvider, SetKey("holdTIMBAMinLevel", "Hold TIMBA Minimum Level", "5000"))
                .Times(1);

            this->display.SetMinimumLevel(5000);
            this->display.SetMaximumLevel(9000);
            this->display.SaveDataToAsr(userSetting);
        }

        TEST_F(HoldDisplayTest, ItSavesMinimisedToTheAsr)
        {
            EXPECT_CALL(this->mockUserSettingProvider, SetKey(_, _, _)).Times(4);

            EXPECT_CALL(this->mockUserSettingProvider, SetKey("holdTIMBAMinimised", "Hold TIMBA Minimised", "1"))
                .Times(1);

            this->display.ButtonClicked("minimise");
            this->display.SaveDataToAsr(userSetting);
        }

        TEST_F(HoldDisplayTest, ItSavesPositionToTheAsr)
        {
            EXPECT_CALL(this->mockUserSettingProvider, SetKey(_, _, _)).Times(3);

            EXPECT_CALL(this->mockUserSettingProvider, SetKey("holdTIMBAPositionX", "Hold TIMBA X Position", "151"))
                .Times(1);

            EXPECT_CALL(this->mockUserSettingProvider, SetKey("holdTIMBAPositionY", "Hold TIMBA Y Position", "262"))
                .Times(1);

            this->display.Move({151, 262});
            this->display.SaveDataToAsr(userSetting);
        }

        TEST_F(HoldDisplayTest, ClickingMinusDescreasesMaximumDisplayLevelUntilMinimum)
        {
            EXPECT_EQ(15000, this->display.GetMaximumLevel());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(14000, this->display.GetMaximumLevel());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(13000, this->display.GetMaximumLevel());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(12000, this->display.GetMaximumLevel());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(11000, this->display.GetMaximumLevel());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(10000, this->display.GetMaximumLevel());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(9000, this->display.GetMaximumLevel());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(8000, this->display.GetMaximumLevel());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(7000, this->display.GetMaximumLevel());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(7000, this->display.GetMaximumLevel());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(7000, this->display.GetMaximumLevel());
            this->display.ButtonClicked("minus");
        }

        TEST_F(HoldDisplayTest, ClickingPlusIncreasesMaximumDisplayLevel)
        {
            EXPECT_EQ(15000, this->display.GetMaximumLevel());
            this->display.ButtonClicked("plus");
            EXPECT_EQ(16000, this->display.GetMaximumLevel());
            this->display.ButtonClicked("plus");
            EXPECT_EQ(17000, this->display.GetMaximumLevel());
            this->display.ButtonClicked("plus");
            EXPECT_EQ(18000, this->display.GetMaximumLevel());
        }

        TEST_F(HoldDisplayTest, ClickingAllResetsToDefaultsIfNoPublishedHold)
        {
            this->display.SetMinimumLevel(2000);
            this->display.SetMinimumLevel(3000);
            this->display.ButtonClicked("allLevels");
            EXPECT_EQ(15000, this->display.GetMaximumLevel());
            EXPECT_EQ(7000, this->display.GetMinimumLevel());
        }

        TEST_F(HoldDisplayTest, ClickingMinimiseMinimisesDisplay)
        {
            EXPECT_FALSE(this->display.IsMinimised());
            this->display.ButtonClicked("minimise");
            EXPECT_TRUE(this->display.IsMinimised());
            this->display.ButtonClicked("minimise");
            EXPECT_FALSE(this->display.IsMinimised());
        }

        TEST_F(HoldDisplayTest, ClickingInformationTogglesInformationMode)
        {
            EXPECT_FALSE(this->display.IsInInformationMode());
            this->display.ButtonClicked("information");
            EXPECT_TRUE(this->display.IsInInformationMode());
            this->display.ButtonClicked("information");
            EXPECT_FALSE(this->display.IsInInformationMode());
        }

        TEST_F(HoldDisplayTest, ClickingOpionsLoadsConfigDialog)
        {
            EXPECT_CALL(mockDialogProvider, OpenDialog(this->holdDialogData, _)).Times(1);
            this->display.ButtonClicked("options");
        }

        TEST_F(HoldDisplayTest, ClickingAddAddsAircraftToHoldManagerAndNotifiesApi)
        {
            std::shared_ptr<MockEuroScopeCRadarTargetInterface> mockRadarTarget(
                new NiceMock<MockEuroScopeCRadarTargetInterface>());

            std::shared_ptr<MockEuroScopeCFlightPlanInterface> mockFlightplan(
                new NiceMock<MockEuroScopeCFlightPlanInterface>());

            ON_CALL(this->mockPlugin, GetSelectedFlightplan()).WillByDefault(Return(mockFlightplan));

            ON_CALL(this->mockPlugin, GetSelectedRadarTarget()).WillByDefault(Return(mockRadarTarget));

            ON_CALL(*mockFlightplan, GetCallsign()).WillByDefault(Return("BAW123"));

            ON_CALL(*mockFlightplan, IsTrackedByUser()).WillByDefault(Return(true));

            EXPECT_CALL(this->mockApi, AssignAircraftToHold("BAW123", "TIMBA")).Times(1);

            this->display.ButtonClicked("add");
            EXPECT_EQ("TIMBA", this->holdManager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldDisplayTest, ClickingAddAddsDoesntAddToHoldManagerIfNotTrackedByUser)
        {
            std::shared_ptr<MockEuroScopeCRadarTargetInterface> mockRadarTarget(
                new NiceMock<MockEuroScopeCRadarTargetInterface>());

            std::shared_ptr<MockEuroScopeCFlightPlanInterface> mockFlightplan(
                new NiceMock<MockEuroScopeCFlightPlanInterface>());

            ON_CALL(this->mockPlugin, GetSelectedFlightplan()).WillByDefault(Return(mockFlightplan));

            ON_CALL(this->mockPlugin, GetSelectedRadarTarget()).WillByDefault(Return(mockRadarTarget));

            ON_CALL(*mockFlightplan, GetCallsign()).WillByDefault(Return("BAW123"));

            ON_CALL(*mockFlightplan, IsTrackedByUser()).WillByDefault(Return(false));

            EXPECT_CALL(this->mockApi, AssignAircraftToHold(_, _)).Times(0);

            EXPECT_EQ(0, this->holdManager.CountHoldingAircraft());

            this->display.ButtonClicked("add");
        }

        TEST_F(HoldDisplayTest, ClickingAddDoesNothingIfNoAircraftSelected)
        {
            ON_CALL(this->mockPlugin, GetSelectedFlightplan()).WillByDefault(Return(nullptr));

            ON_CALL(this->mockPlugin, GetSelectedRadarTarget()).WillByDefault(Return(nullptr));

            this->display.ButtonClicked("add");

            EXPECT_CALL(this->mockApi, AssignAircraftToHold(_, _)).Times(0);

            EXPECT_EQ(0, this->holdManager.CountHoldingAircraft());
        }

        TEST_F(HoldDisplayTest, MoveSetsWindowPos)
        {
            POINT newPos = {300, 300};
            this->display.Move(newPos);
            EXPECT_EQ(newPos.x, this->display.GetDisplayPos().x);
            EXPECT_EQ(newPos.y, this->display.GetDisplayPos().y);
        }

        TEST_F(HoldDisplayTest, MoveSetsDataStartHeight)
        {
            POINT newPos = {300, 300};
            this->display.Move(newPos);
            EXPECT_EQ(300 + this->display.dataStartOffset, this->display.GetDataStartHeight());
        }

        TEST_F(HoldDisplayTest, MoveSetsTitleBarRenderableArea)
        {
            POINT newPos = {300, 300};
            this->display.Move(newPos);

            Gdiplus::Rect expected = {300, 300, 225, 15};
            EXPECT_TRUE(expected.Equals(this->display.GetTitleArea()));
        }

        TEST_F(HoldDisplayTest, MoveSetsTitleBarClickableArea)
        {
            POINT newPos = {300, 300};
            this->display.Move(newPos);

            RECT expected = {300, 300, 525, 315};
            EXPECT_TRUE(expected.left == this->display.GetTitleClickArea().left);
            EXPECT_TRUE(expected.top == this->display.GetTitleClickArea().top);
            EXPECT_TRUE(expected.right == this->display.GetTitleClickArea().right);
            EXPECT_TRUE(expected.bottom == this->display.GetTitleClickArea().bottom);
        }

        TEST_F(HoldDisplayTest, MoveSetsMinimiseRenderableArea)
        {
            POINT newPos = {300, 300};
            this->display.Move(newPos);

            Gdiplus::Rect expected = {305, 302, 11, 11};
            EXPECT_TRUE(expected.Equals(this->display.GetMinimiseArea()));
        }

        TEST_F(HoldDisplayTest, MoveSetsMinmiseClickableArea)
        {
            POINT newPos = {300, 300};
            this->display.Move(newPos);

            RECT expected = {305, 302, 316, 313};
            EXPECT_TRUE(expected.left == this->display.GetMinimiseClickArea().left);
            EXPECT_TRUE(expected.top == this->display.GetMinimiseClickArea().top);
            EXPECT_TRUE(expected.right == this->display.GetMinimiseClickArea().right);
            EXPECT_TRUE(expected.bottom == this->display.GetMinimiseClickArea().bottom);
        }

        TEST_F(HoldDisplayTest, MoveSetsInformationRenderableArea)
        {
            POINT newPos = {300, 300};
            this->display.Move(newPos);

            Gdiplus::Rect expected = {321, 302, 11, 11};
            EXPECT_TRUE(expected.Equals(this->display.GetInformationArea()));
        }

        TEST_F(HoldDisplayTest, MoveSetsInformationClickableArea)
        {
            POINT newPos = {300, 300};
            this->display.Move(newPos);

            RECT expected = {321, 302, 332, 313};
            EXPECT_TRUE(expected.left == this->display.GetInformationClickArea().left);
            EXPECT_TRUE(expected.top == this->display.GetInformationClickArea().top);
            EXPECT_TRUE(expected.right == this->display.GetInformationClickArea().right);
            EXPECT_TRUE(expected.bottom == this->display.GetInformationClickArea().bottom);
        }

        TEST_F(HoldDisplayTest, MoveSetsOptionsRenderableArea)
        {
            POINT newPos = {300, 300};
            this->display.Move(newPos);

            Gdiplus::Rect expected = {337, 302, 11, 11};
            EXPECT_TRUE(expected.Equals(this->display.GetOptionsArea()));
        }

        TEST_F(HoldDisplayTest, MoveSetsOptionsClickableArea)
        {
            POINT newPos = {300, 300};
            this->display.Move(newPos);

            RECT expected = {337, 302, 348, 313};
            EXPECT_TRUE(expected.left == this->display.GetOptionsClickArea().left);
            EXPECT_TRUE(expected.top == this->display.GetOptionsClickArea().top);
            EXPECT_TRUE(expected.right == this->display.GetOptionsClickArea().right);
            EXPECT_TRUE(expected.bottom == this->display.GetOptionsClickArea().bottom);
        }

        TEST_F(HoldDisplayTest, MoveSetsMinusRenderableArea)
        {
            POINT newPos = {300, 300};
            this->display.Move(newPos);

            Gdiplus::Rect expected = {315, 318, this->display.bigButtonWidth, this->display.bigButtonHeight};
            EXPECT_TRUE(expected.Equals(this->display.GetMinusArea()));
        }

        TEST_F(HoldDisplayTest, MoveSetsMinusClickableArea)
        {
            POINT newPos = {300, 300};
            this->display.Move(newPos);

            RECT expected = {315, 318, 315 + this->display.bigButtonWidth, 318 + this->display.bigButtonHeight};
            EXPECT_TRUE(expected.left == this->display.GetMinusClickArea().left);
            EXPECT_TRUE(expected.top == this->display.GetMinusClickArea().top);
            EXPECT_TRUE(expected.right == this->display.GetMinusClickArea().right);
            EXPECT_TRUE(expected.bottom == this->display.GetMinusClickArea().bottom);
        }

        TEST_F(HoldDisplayTest, MoveSetsPlusRenderableArea)
        {
            POINT newPos = {300, 300};
            this->display.Move(newPos);

            Gdiplus::Rect expected = {365, 318, this->display.bigButtonWidth, this->display.bigButtonHeight};
            EXPECT_TRUE(expected.Equals(this->display.GetPlusArea()));
        }

        TEST_F(HoldDisplayTest, MoveSetsPlusClickableArea)
        {
            POINT newPos = {300, 300};
            this->display.Move(newPos);

            RECT expected = {365, 318, 365 + this->display.bigButtonWidth, 318 + this->display.bigButtonHeight};
            EXPECT_TRUE(expected.left == this->display.GetPlusClickArea().left);
            EXPECT_TRUE(expected.top == this->display.GetPlusClickArea().top);
            EXPECT_TRUE(expected.right == this->display.GetPlusClickArea().right);
            EXPECT_TRUE(expected.bottom == this->display.GetPlusClickArea().bottom);
        }

        TEST_F(HoldDisplayTest, MoveSetsAllLevelsRenderableArea)
        {
            POINT newPos = {300, 300};
            this->display.Move(newPos);

            Gdiplus::Rect expected = {415, 318, this->display.bigButtonWidth, this->display.bigButtonHeight};
            EXPECT_TRUE(expected.Equals(this->display.GetAllArea()));
        }

        TEST_F(HoldDisplayTest, MoveSetsAllLevelsClickableArea)
        {
            POINT newPos = {300, 300};
            this->display.Move(newPos);

            RECT expected = {415, 318, 415 + this->display.bigButtonWidth, 318 + this->display.bigButtonHeight};
            EXPECT_TRUE(expected.left == this->display.GetAllClickArea().left);
            EXPECT_TRUE(expected.top == this->display.GetAllClickArea().top);
            EXPECT_TRUE(expected.right == this->display.GetAllClickArea().right);
            EXPECT_TRUE(expected.bottom == this->display.GetAllClickArea().bottom);
        }

        TEST_F(HoldDisplayTest, MoveSetsAddRenderableArea)
        {
            POINT newPos = {300, 300};
            this->display.Move(newPos);

            Gdiplus::Rect expected = {465, 318, this->display.bigButtonWidth, this->display.bigButtonHeight};
            EXPECT_TRUE(expected.Equals(this->display.GetAddArea()));
        }

        TEST_F(HoldDisplayTest, MoveSetsAddClickableArea)
        {
            POINT newPos = {300, 300};
            this->display.Move(newPos);

            RECT expected = {465, 318, 465 + this->display.bigButtonWidth, 318 + this->display.bigButtonHeight};
            EXPECT_TRUE(expected.left == this->display.GetAddClickArea().left);
            EXPECT_TRUE(expected.top == this->display.GetAddClickArea().top);
            EXPECT_TRUE(expected.right == this->display.GetAddClickArea().right);
            EXPECT_TRUE(expected.bottom == this->display.GetAddClickArea().bottom);
        }

        TEST_F(HoldDisplayTest, MoveSetsUnderButtonLine)
        {
            POINT newPos = {300, 300};
            this->display.Move(newPos);

            Gdiplus::Point expectedLeft = {300, 350};
            Gdiplus::Point expectedRight = {525, 350};
            EXPECT_TRUE(expectedLeft.Equals(this->display.GetUnderButtonLineLeft()));
            EXPECT_TRUE(expectedRight.Equals(this->display.GetUnderButtonLineRight()));
        }

        TEST_F(HoldDisplayTest, CallsignClickedTriggersHoldMenu)
        {
            EXPECT_CALL(
                this->mockRadarScreen,
                TogglePluginTagFunction("BAW123", 9003, PointEq(POINT({1, 2})), RectEq(RECT({1, 2, 3, 4}))))
                .Times(1);

            this->display.CallsignClicked("BAW123", this->mockRadarScreen, {1, 2}, {1, 2, 3, 4});
        }

        TEST_F(HoldDisplayTest, ClearedLevelClickedTriggersHoldMenu)
        {
            EXPECT_CALL(
                this->mockRadarScreen,
                ToggleTemporaryAltitudePopupList("BAW123", PointEq(POINT({1, 2})), RectEq(RECT({1, 2, 3, 4}))))
                .Times(1);

            this->display.ClearedLevelClicked("BAW123", this->mockRadarScreen, {1, 2}, {1, 2, 3, 4});
        }

        TEST_F(HoldDisplayTest, ItCalculatesTheBackgroundRenderDetailsNoAircraftAtSameLevel)
        {
            Gdiplus::Rect expected{
                this->display.GetDisplayPos().x, this->display.GetDisplayPos().y, this->display.windowWidth, 305};
            std::map<int, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>> aircraft;
            aircraft[7000].insert(std::make_shared<HoldingAircraft>("BAW123", "TIMBA"));
            aircraft[8000].insert(std::make_shared<HoldingAircraft>("VIR25A", "TIMBA"));

            this->display.SetMinimumLevel(7000);
            this->display.SetMaximumLevel(15000);

            ASSERT_TRUE(expected.Equals(this->display.GetHoldViewBackgroundRender(aircraft)));
        }

        TEST_F(HoldDisplayTest, ItCalculatesTheBackgroundRenderDetailsAircraftAtSameLevel)
        {
            Gdiplus::Rect expected{
                this->display.GetDisplayPos().x, this->display.GetDisplayPos().y, this->display.windowWidth, 380};
            std::map<int, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>> aircraft;
            aircraft[7000].insert(std::make_shared<HoldingAircraft>("BAW123", "TIMBA"));
            aircraft[7000].insert(std::make_shared<HoldingAircraft>("EZY234", std::set<std::string>()));
            aircraft[8000].insert(std::make_shared<HoldingAircraft>("VIR25A", "TIMBA"));
            aircraft[8000].insert(std::make_shared<HoldingAircraft>("LOT123", "TIMBA"));
            aircraft[8000].insert(std::make_shared<HoldingAircraft>("RYR93", "TIMBA"));

            this->display.SetMinimumLevel(7000);
            this->display.SetMaximumLevel(15000);

            ASSERT_TRUE(expected.Equals(this->display.GetHoldViewBackgroundRender(aircraft)));
        }

        TEST_F(HoldDisplayTest, ItMapsAircraftToLevelsAndFiltersThoseOutsideTheDisplayRange)
        {
            std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft> aircraft;
            aircraft.insert(std::make_shared<HoldingAircraft>("BAW123", "TIMBA"));
            aircraft.insert(std::make_shared<HoldingAircraft>("EZY234", std::set<std::string>({"TIMBA"})));
            aircraft.insert(std::make_shared<HoldingAircraft>("VIR25A", std::set<std::string>({"TIMBA"})));
            aircraft.insert(std::make_shared<HoldingAircraft>("RYR191", "TIMBA"));
            aircraft.insert(std::make_shared<HoldingAircraft>("BMI234", "TIMBA"));
            aircraft.insert(std::make_shared<HoldingAircraft>("LOT555", "TIMBA"));

            this->display.SetMinimumLevel(7000);
            this->display.SetMaximumLevel(15000);

            // These two should both make it through at FL80 as BAW123 is assigned TIMBA
            std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> radarTargetBaw123 =
                std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();
            ON_CALL(*radarTargetBaw123, GetFlightLevel()).WillByDefault(Return(8100));

            ON_CALL(*radarTargetBaw123, GetVerticalSpeed()).WillByDefault(Return(0));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("BAW123")).WillByDefault(Return(radarTargetBaw123));

            std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> radarTargetEzy234 =
                std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();
            ON_CALL(*radarTargetEzy234, GetFlightLevel()).WillByDefault(Return(8550));

            ON_CALL(*radarTargetEzy234, GetVerticalSpeed()).WillByDefault(Return(-400));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("EZY234")).WillByDefault(Return(radarTargetEzy234));

            // This one shouldnt make it through as there's no assigned aircraft at this level
            std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> radarTargetVir25a =
                std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();
            ON_CALL(*radarTargetVir25a, GetFlightLevel()).WillByDefault(Return(9000));

            ON_CALL(*radarTargetVir25a, GetVerticalSpeed()).WillByDefault(Return(0));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("VIR25A")).WillByDefault(Return(radarTargetVir25a));

            // This one should make it through as assigned
            std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> radarTargetRyr191 =
                std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();
            ON_CALL(*radarTargetRyr191, GetFlightLevel()).WillByDefault(Return(10000));

            ON_CALL(*radarTargetRyr191, GetVerticalSpeed()).WillByDefault(Return(0));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("RYR191")).WillByDefault(Return(radarTargetRyr191));

            // This one shouldn't make it through as no radar target
            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("BMI234")).WillByDefault(Return(nullptr));

            // This one shouldnt make it through as its too high
            std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> radarTargetLot555 =
                std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();
            ON_CALL(*radarTargetLot555, GetFlightLevel()).WillByDefault(Return(16000));

            ON_CALL(*radarTargetLot555, GetVerticalSpeed()).WillByDefault(Return(0));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("LOT555")).WillByDefault(Return(radarTargetLot555));

            const std::map<int, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>> mapped =
                this->display.MapAircraftToLevels(aircraft);

            EXPECT_EQ(2, mapped.size());
            EXPECT_EQ(2, mapped.at(8000).size());
            EXPECT_EQ("BAW123", (*mapped.at(8000).cbegin())->GetCallsign());
            EXPECT_EQ("EZY234", (*++mapped.at(8000).cbegin())->GetCallsign());
            EXPECT_EQ(0, mapped.count(9000));
            EXPECT_EQ(1, mapped.at(10000).size());
            EXPECT_EQ("RYR191", (*mapped.at(10000).cbegin())->GetCallsign());
        }

        TEST_F(HoldDisplayTest, MappingAircraftToLevelsFiltersOutThoseInDeemedSeparatedHolds)
        {
            std::set<std::unique_ptr<DeemedSeparatedHold>> deemedSeparated;
            deemedSeparated.insert(std::make_unique<DeemedSeparatedHold>(2, 7));

            this->publishedHolds.Add({1, "TIMBA", "TIMBA", 7000, 15000, 12, "left", {}, std::move(deemedSeparated)});
            this->publishedHolds.Add({
                2,
                "WILLO",
                "WILLO",
                7000,
                15000,
            });
            HoldDisplay display2(mockPlugin, holdManager, navaid, publishedHolds, dialogManager, addAircraftList);
            display2.SetMinimumLevel(7000);
            display2.SetMaximumLevel(15000);

            std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft> aircraft;
            aircraft.insert(std::make_shared<HoldingAircraft>("BAW123", "TIMBA"));
            auto conflictingAircraft = std::make_shared<HoldingAircraft>("EZY234", std::set<std::string>({"TIMBA"}));
            conflictingAircraft->SetAssignedHold("WILLO");
            aircraft.insert(conflictingAircraft);

            this->display.SetMinimumLevel(7000);
            this->display.SetMaximumLevel(15000);

            // In this scenario, both are within the confines of their holds and are sufficiently distanced.
            std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> radarTargetBaw123 =
                std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();
            ON_CALL(*radarTargetBaw123, GetFlightLevel()).WillByDefault(Return(8000));

            ON_CALL(*radarTargetBaw123, GetVerticalSpeed()).WillByDefault(Return(0));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("BAW123")).WillByDefault(Return(radarTargetBaw123));

            ON_CALL(*radarTargetBaw123, GetPosition()).WillByDefault(Return(this->timbaPosition));

            std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> radarTargetEzy234 =
                std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();
            ON_CALL(*radarTargetEzy234, GetFlightLevel()).WillByDefault(Return(8000));

            ON_CALL(*radarTargetEzy234, GetVerticalSpeed()).WillByDefault(Return(0));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("EZY234")).WillByDefault(Return(radarTargetEzy234));

            ON_CALL(*radarTargetEzy234, GetPosition()).WillByDefault(Return(this->willoPosition));

            const std::map<int, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>> mapped =
                display2.MapAircraftToLevels(aircraft);

            EXPECT_EQ(1, mapped.size());
            EXPECT_EQ(1, mapped.at(8000).size());
            EXPECT_EQ("BAW123", (*mapped.at(8000).cbegin())->GetCallsign());
        }

        TEST_F(HoldDisplayTest, MappingAircraftToLevelsDoesntFilterOutAircraftBelowVslInsert)
        {
            std::set<std::unique_ptr<DeemedSeparatedHold>> deemedSeparated;
            deemedSeparated.insert(std::make_unique<DeemedSeparatedHold>(2, 15));

            this->publishedHolds.Add({1, "TIMBA", "TIMBA", 7000, 15000, 12, "left", {}, std::move(deemedSeparated)});
            this->publishedHolds.Add({
                2,
                "WILLO",
                "WILLO",
                7000,
                15000,
            });
            HoldDisplay display2(mockPlugin, holdManager, navaid, publishedHolds, dialogManager, addAircraftList);
            display2.SetMinimumLevel(7000);
            display2.SetMaximumLevel(15000);

            std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft> aircraft;
            aircraft.insert(std::make_shared<HoldingAircraft>("BAW123", "TIMBA"));
            auto conflictingAircraft = std::make_shared<HoldingAircraft>("EZY234", std::set<std::string>({"TIMBA"}));
            conflictingAircraft->SetAssignedHold("WILLO");
            aircraft.insert(conflictingAircraft);

            this->display.SetMinimumLevel(7000);
            this->display.SetMaximumLevel(15000);

            std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> radarTargetBaw123 =
                std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();
            ON_CALL(*radarTargetBaw123, GetFlightLevel()).WillByDefault(Return(8000));

            ON_CALL(*radarTargetBaw123, GetVerticalSpeed()).WillByDefault(Return(0));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("BAW123")).WillByDefault(Return(radarTargetBaw123));

            ON_CALL(*radarTargetBaw123, GetPosition()).WillByDefault(Return(this->timbaPosition));

            std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> radarTargetEzy234 =
                std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();
            ON_CALL(*radarTargetEzy234, GetFlightLevel()).WillByDefault(Return(8000));

            ON_CALL(*radarTargetEzy234, GetVerticalSpeed()).WillByDefault(Return(0));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("EZY234")).WillByDefault(Return(radarTargetEzy234));

            ON_CALL(*radarTargetEzy234, GetPosition()).WillByDefault(Return(this->mayPosition));

            const std::map<int, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>> mapped =
                display2.MapAircraftToLevels(aircraft);

            EXPECT_EQ(1, mapped.size());
            EXPECT_EQ(2, mapped.at(8000).size());
            EXPECT_EQ("BAW123", (*mapped.at(8000).cbegin())->GetCallsign());
            EXPECT_EQ("EZY234", (*++mapped.at(8000).cbegin())->GetCallsign());
        }

        TEST_F(HoldDisplayTest, MappingAircraftToLevelsDoesntFilterOutAircraftOutsideAssignedHold)
        {
            std::set<std::unique_ptr<DeemedSeparatedHold>> deemedSeparated;
            deemedSeparated.insert(std::make_unique<DeemedSeparatedHold>(2, 10));

            this->publishedHolds.Add({1, "TIMBA", "TIMBA", 7000, 15000, 12, "left", {}, std::move(deemedSeparated)});
            // WILLO starts at 9000, so the conflicting aircraft is outside WILLO's confines
            this->publishedHolds.Add({
                2,
                "WILLO",
                "WILLO",
                9000,
                15000,
            });
            HoldDisplay display2(mockPlugin, holdManager, navaid, publishedHolds, dialogManager, addAircraftList);
            display2.SetMinimumLevel(7000);
            display2.SetMaximumLevel(15000);

            std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft> aircraft;
            aircraft.insert(std::make_shared<HoldingAircraft>("BAW123", "TIMBA"));
            auto conflictingAircraft = std::make_shared<HoldingAircraft>("EZY234", std::set<std::string>({"TIMBA"}));
            conflictingAircraft->SetAssignedHold("WILLO");
            aircraft.insert(conflictingAircraft);

            this->display.SetMinimumLevel(7000);
            this->display.SetMaximumLevel(15000);

            std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> radarTargetBaw123 =
                std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();
            ON_CALL(*radarTargetBaw123, GetFlightLevel()).WillByDefault(Return(8000));

            ON_CALL(*radarTargetBaw123, GetVerticalSpeed()).WillByDefault(Return(0));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("BAW123")).WillByDefault(Return(radarTargetBaw123));

            ON_CALL(*radarTargetBaw123, GetPosition()).WillByDefault(Return(this->timbaPosition));

            std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> radarTargetEzy234 =
                std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();
            ON_CALL(*radarTargetEzy234, GetFlightLevel()).WillByDefault(Return(8000));

            ON_CALL(*radarTargetEzy234, GetVerticalSpeed()).WillByDefault(Return(0));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("EZY234")).WillByDefault(Return(radarTargetEzy234));

            ON_CALL(*radarTargetEzy234, GetPosition()).WillByDefault(Return(this->willoPosition));

            const std::map<int, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>> mapped =
                display2.MapAircraftToLevels(aircraft);

            EXPECT_EQ(1, mapped.size());
            EXPECT_EQ(2, mapped.at(8000).size());
            EXPECT_EQ("BAW123", (*mapped.at(8000).cbegin())->GetCallsign());
            EXPECT_EQ("EZY234", (*++mapped.at(8000).cbegin())->GetCallsign());
        }

        TEST_F(HoldDisplayTest, MappingAircraftToLevelsDoesntFilterOutAircraftAssignedToHoldOutsidePublishedHold)
        {
            std::set<std::unique_ptr<DeemedSeparatedHold>> deemedSeparated;
            deemedSeparated.insert(std::make_unique<DeemedSeparatedHold>(2, 10));

            // TIMBA starts at 9000, so the aircraft assigned to TIMBA is outside the published hold
            this->publishedHolds.Add({1, "TIMBA", "TIMBA", 9000, 15000, 12, "left", {}, std::move(deemedSeparated)});

            this->publishedHolds.Add({
                2,
                "WILLO",
                "WILLO",
                7000,
                15000,
            });
            HoldDisplay display2(mockPlugin, holdManager, navaid, publishedHolds, dialogManager, addAircraftList);
            display2.SetMinimumLevel(7000);
            display2.SetMaximumLevel(15000);

            std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft> aircraft;
            aircraft.insert(std::make_shared<HoldingAircraft>("BAW123", "TIMBA"));
            auto conflictingAircraft = std::make_shared<HoldingAircraft>("EZY234", std::set<std::string>({"TIMBA"}));
            conflictingAircraft->SetAssignedHold("WILLO");
            aircraft.insert(conflictingAircraft);

            this->display.SetMinimumLevel(7000);
            this->display.SetMaximumLevel(15000);

            std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> radarTargetBaw123 =
                std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();
            ON_CALL(*radarTargetBaw123, GetFlightLevel()).WillByDefault(Return(8000));

            ON_CALL(*radarTargetBaw123, GetVerticalSpeed()).WillByDefault(Return(0));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("BAW123")).WillByDefault(Return(radarTargetBaw123));

            ON_CALL(*radarTargetBaw123, GetPosition()).WillByDefault(Return(this->timbaPosition));

            std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> radarTargetEzy234 =
                std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();
            ON_CALL(*radarTargetEzy234, GetFlightLevel()).WillByDefault(Return(8000));

            ON_CALL(*radarTargetEzy234, GetVerticalSpeed()).WillByDefault(Return(0));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("EZY234")).WillByDefault(Return(radarTargetEzy234));

            ON_CALL(*radarTargetEzy234, GetPosition()).WillByDefault(Return(this->willoPosition));

            const std::map<int, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>> mapped =
                display2.MapAircraftToLevels(aircraft);

            EXPECT_EQ(1, mapped.size());
            EXPECT_EQ(2, mapped.at(8000).size());
            EXPECT_EQ("BAW123", (*mapped.at(8000).cbegin())->GetCallsign());
            EXPECT_EQ("EZY234", (*++mapped.at(8000).cbegin())->GetCallsign());
        }

        TEST_F(HoldDisplayTest, MappingAircraftToLevelsDoesntFilterOutAircraftNotInADeemedSeparatedHold)
        {
            std::set<std::unique_ptr<DeemedSeparatedHold>> deemedSeparated;
            deemedSeparated.insert(std::make_unique<DeemedSeparatedHold>(2, 10));

            this->publishedHolds.Add({1, "TIMBA", "TIMBA", 7000, 15000, 12, "left", {}, std::move(deemedSeparated)});
            this->publishedHolds.Add({
                2,
                "WILLO",
                "WILLO",
                7000,
                15000,
            });
            this->publishedHolds.Add({
                3,
                "MAY",
                "MAY",
                7000,
                15000,
            });
            HoldDisplay display2(mockPlugin, holdManager, navaid, publishedHolds, dialogManager, addAircraftList);
            display2.SetMinimumLevel(7000);
            display2.SetMaximumLevel(15000);

            std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft> aircraft;
            aircraft.insert(std::make_shared<HoldingAircraft>("BAW123", "TIMBA"));
            auto conflictingAircraft = std::make_shared<HoldingAircraft>("EZY234", std::set<std::string>({"TIMBA"}));
            conflictingAircraft->SetAssignedHold("MAY");
            aircraft.insert(conflictingAircraft);

            this->display.SetMinimumLevel(7000);
            this->display.SetMaximumLevel(15000);

            std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> radarTargetBaw123 =
                std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();
            ON_CALL(*radarTargetBaw123, GetFlightLevel()).WillByDefault(Return(8000));

            ON_CALL(*radarTargetBaw123, GetVerticalSpeed()).WillByDefault(Return(0));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("BAW123")).WillByDefault(Return(radarTargetBaw123));

            ON_CALL(*radarTargetBaw123, GetPosition()).WillByDefault(Return(this->timbaPosition));

            std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> radarTargetEzy234 =
                std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();
            ON_CALL(*radarTargetEzy234, GetFlightLevel()).WillByDefault(Return(8000));

            ON_CALL(*radarTargetEzy234, GetVerticalSpeed()).WillByDefault(Return(0));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("EZY234")).WillByDefault(Return(radarTargetEzy234));

            ON_CALL(*radarTargetEzy234, GetPosition()).WillByDefault(Return(this->willoPosition));

            const std::map<int, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>> mapped =
                display2.MapAircraftToLevels(aircraft);

            EXPECT_EQ(1, mapped.size());
            EXPECT_EQ(2, mapped.at(8000).size());
            EXPECT_EQ("BAW123", (*mapped.at(8000).cbegin())->GetCallsign());
            EXPECT_EQ("EZY234", (*++mapped.at(8000).cbegin())->GetCallsign());
        }

        TEST_F(HoldDisplayTest, MappingAircraftToLevelsFiltersOutAircraftThatArentAssignedToTheHold)
        {
            std::set<std::unique_ptr<DeemedSeparatedHold>> deemedSeparated;
            deemedSeparated.insert(std::make_unique<DeemedSeparatedHold>(2, 10));

            // TIMBA starts at 9000, so the aircraft assigned to TIMBA is outside the published hold
            this->publishedHolds.Add({1, "TIMBA", "TIMBA", 7000, 15000, 12, "left", {}, std::move(deemedSeparated)});

            this->publishedHolds.Add({
                2,
                "WILLO",
                "WILLO",
                7000,
                15000,
            });
            HoldDisplay display2(mockPlugin, holdManager, navaid, publishedHolds, dialogManager, addAircraftList);
            display2.SetMinimumLevel(7000);
            display2.SetMaximumLevel(15000);

            std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft> aircraft;
            aircraft.insert(std::make_shared<HoldingAircraft>("BAW123", "TIMBA"));
            auto conflictingAircraft = std::make_shared<HoldingAircraft>("EZY234", std::set<std::string>({"TIMBA"}));
            conflictingAircraft->SetAssignedHold("WILLO");
            aircraft.insert(conflictingAircraft);
            auto conflictingAircraft2 = std::make_shared<HoldingAircraft>("BAW012", std::set<std::string>({"TIMBA"}));
            conflictingAircraft2->SetAssignedHold("WILLO");
            aircraft.insert(conflictingAircraft2);

            this->display.SetMinimumLevel(7000);
            this->display.SetMaximumLevel(15000);

            std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> radarTargetBaw123 =
                std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();
            ON_CALL(*radarTargetBaw123, GetFlightLevel()).WillByDefault(Return(8000));

            ON_CALL(*radarTargetBaw123, GetVerticalSpeed()).WillByDefault(Return(0));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("BAW123")).WillByDefault(Return(radarTargetBaw123));

            ON_CALL(*radarTargetBaw123, GetPosition()).WillByDefault(Return(this->timbaPosition));

            std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> radarTargetEzy234 =
                std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();
            ON_CALL(*radarTargetEzy234, GetFlightLevel()).WillByDefault(Return(8000));

            ON_CALL(*radarTargetEzy234, GetVerticalSpeed()).WillByDefault(Return(0));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("EZY234")).WillByDefault(Return(radarTargetEzy234));

            ON_CALL(*radarTargetEzy234, GetPosition()).WillByDefault(Return(this->willoPosition));

            std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> radarTargetBaw012 =
                std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();
            ON_CALL(*radarTargetBaw012, GetFlightLevel()).WillByDefault(Return(8000));

            ON_CALL(*radarTargetBaw012, GetVerticalSpeed()).WillByDefault(Return(0));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign("BAW012")).WillByDefault(Return(radarTargetBaw012));

            ON_CALL(*radarTargetBaw012, GetPosition()).WillByDefault(Return(this->willoPosition));

            const std::map<int, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>> mapped =
                display2.MapAircraftToLevels(aircraft);

            EXPECT_EQ(1, mapped.size());
            EXPECT_EQ(1, mapped.at(8000).size());
            EXPECT_EQ("BAW123", (*mapped.at(8000).cbegin())->GetCallsign());
        }

        TEST_F(HoldDisplayTest, ButtonRightClickedDoesNothingIfNotAddButton)
        {
            EXPECT_CALL(*addAircraftList, Trigger).Times(0);

            display.ButtonRightClicked("notadd");
        }

        TEST_F(HoldDisplayTest, AddButtonRightClickTriggersTheList)
        {
            EXPECT_CALL(*addAircraftList, Trigger(PointEq(POINT{265, 118}))).Times(1);

            display.ButtonRightClicked("add");
        }
    } // namespace Hold
} // namespace UKControllerPluginTest
