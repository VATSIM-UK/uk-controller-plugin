#include "pch/pch.h"
#include "euroscope/UserSetting.h"
#include "hold/HoldDisplay.h"
#include "navaids/Navaid.h"
#include "hold/HoldManager.h"
#include "mock/MockApiInterface.h"
#include "mock/MockTaskRunnerInterface.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockUserSettingProviderInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "mock/MockDialogProvider.h"
#include "dialog/DialogManager.h"
#include "dialog/DialogData.h"

using UKControllerPlugin::Hold::HoldDisplay;
using UKControllerPlugin::Navaids::Navaid;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPlugin::Dialog::DialogManager;
using testing::Test;
using testing::NiceMock;
using testing::Return;
using testing::_;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldDisplayTest : public Test
        {
            public:
                HoldDisplayTest()
                    : display(mockPlugin, holdManager, navaid, {}, dialogManager),
                    userSetting(mockUserSettingProvider), navaid({ 2, "TIMBA", EuroScopePlugIn::CPosition()}),
                    dialogManager(mockDialogProvider), holdManager(mockApi, mockTaskRunner)
                {
                    this->navaid.coordinates.LoadFromStrings("E000.15.42.000", "N050.56.44.000");
                }

                DialogData holdDialogData = { IDD_HOLD_PARAMS, "Test" };
                NiceMock<MockTaskRunnerInterface> mockTaskRunner;
                NiceMock<MockApiInterface> mockApi;
                NiceMock<MockDialogProvider> mockDialogProvider;
                DialogManager dialogManager;
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
                UserSetting userSetting;
                Navaid navaid;
                HoldManager holdManager;
                HoldDisplay display;
        };

        TEST_F(HoldDisplayTest, ItLoadsMinimisedFromAsr)
        {
            ON_CALL(this->mockUserSettingProvider, GetKey("holdTIMBAMinimised"))
                .WillByDefault(Return("1"));

            this->display.LoadDataFromAsr(userSetting);
            EXPECT_TRUE(this->display.IsMinimised());
        }

        TEST_F(HoldDisplayTest, ItDefaultsToNotMinimised)
        {
            ON_CALL(this->mockUserSettingProvider, GetKey("holdTIMBAMinimised"))
                .WillByDefault(Return("abc"));

            this->display.LoadDataFromAsr(userSetting);
            EXPECT_FALSE(this->display.IsMinimised());
        }

        TEST_F(HoldDisplayTest, ItLoadsWindowPositionFromAsr)
        {
            ON_CALL(this->mockUserSettingProvider, GetKey("holdTIMBAPositionX"))
                .WillByDefault(Return("151"));

            ON_CALL(this->mockUserSettingProvider, GetKey("holdTIMBAPositionY"))
                .WillByDefault(Return("262"));

            this->display.LoadDataFromAsr(userSetting);

            EXPECT_EQ(151, this->display.GetDisplayPos().x);
            EXPECT_EQ(262, this->display.GetDisplayPos().y);
        }

        TEST_F(HoldDisplayTest, ItDefaultsWindowPosition)
        {
            ON_CALL(this->mockUserSettingProvider, GetKey("holdTIMBAPositionX"))
                .WillByDefault(Return(""));

            ON_CALL(this->mockUserSettingProvider, GetKey("holdTIMBAPositionY"))
                .WillByDefault(Return(""));

            this->display.LoadDataFromAsr(userSetting);
            EXPECT_EQ(100, this->display.GetDisplayPos().x);
            EXPECT_EQ(100, this->display.GetDisplayPos().y);
        }

        TEST_F(HoldDisplayTest, ItSavesDisplayLevelsToAsr)
        {
            EXPECT_CALL(this->mockUserSettingProvider, SetKey(_, _, _));

            EXPECT_CALL(
                this->mockUserSettingProvider,
                SetKey("holdTIMBAMaxLevel", "Hold TIMBA Maximum Level", "9000")
            )
                .Times(1);

            EXPECT_CALL(
                this->mockUserSettingProvider,
                SetKey("holdTIMBAMinLevel", "Hold TIMBA Minimum Level", "5000")
            )
                .Times(1);

            this->display.SetMinimumLevel(5000);
            this->display.SetMinimumLevel(9000);
            this->display.SaveDataToAsr(userSetting);
        }

        TEST_F(HoldDisplayTest, ItSavesMinimisedToTheAsr)
        {
            EXPECT_CALL(this->mockUserSettingProvider, SetKey(_, _, _))
                .Times(3);

            EXPECT_CALL(
                this->mockUserSettingProvider,
                SetKey("holdTIMBAMinimised", "Hold TIMBA Minimised", "1")
            )
                .Times(1);

            this->display.ButtonClicked("minimise");
            this->display.SaveDataToAsr(userSetting);
        }

        TEST_F(HoldDisplayTest, ItSavesPositionToTheAsr)
        {
            EXPECT_CALL(this->mockUserSettingProvider, SetKey(_, _, _))
                .Times(2);

            EXPECT_CALL(
                this->mockUserSettingProvider,
                SetKey("holdTIMBAPositionX", "Hold TIMBA X Position", "151")
            )
                .Times(1);

            EXPECT_CALL(
                this->mockUserSettingProvider,
                SetKey("holdTIMBAPositionY", "Hold TIMBA Y Position", "262")
            )
                .Times(1);

            this->display.Move({ 151, 262 });
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
            EXPECT_CALL(mockDialogProvider, OpenDialog(this->holdDialogData, _))
                .Times(1);
            this->display.ButtonClicked("options");
        }

        TEST_F(HoldDisplayTest, ClickingAddAddsAircraftToHoldManagerAndNotifiesApi)
        {
            std::shared_ptr<MockEuroScopeCRadarTargetInterface> mockRadarTarget(
                new NiceMock<MockEuroScopeCRadarTargetInterface>()
            );

            std::shared_ptr<MockEuroScopeCFlightPlanInterface> mockFlightplan(
                new NiceMock<MockEuroScopeCFlightPlanInterface>()
            );

            ON_CALL(this->mockPlugin, GetSelectedFlightplan())
                .WillByDefault(Return(mockFlightplan));

            ON_CALL(this->mockPlugin, GetSelectedRadarTarget())
                .WillByDefault(Return(mockRadarTarget));

            ON_CALL(*mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            EXPECT_CALL(this->mockApi, AssignAircraftToHold("BAW123", "TIMBA"))
                .Times(1);

            this->display.ButtonClicked("add");
            EXPECT_EQ("TIMBA", this->holdManager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldDisplayTest, ClickingAddDoesNothingIfNoAircraftSelected)
        {
            ON_CALL(this->mockPlugin, GetSelectedFlightplan())
                .WillByDefault(Return(nullptr));

            ON_CALL(this->mockPlugin, GetSelectedRadarTarget())
                .WillByDefault(Return(nullptr));

            this->display.ButtonClicked("add");

            EXPECT_CALL(this->mockApi, AssignAircraftToHold(_, _))
                .Times(0);

            EXPECT_EQ(0, this->holdManager.CountHoldingAircraft());
        }

        TEST_F(HoldDisplayTest, MoveSetsWindowPos)
        {
            POINT newPos = { 300, 300 };
            this->display.Move(newPos);
            EXPECT_EQ(newPos.x, this->display.GetDisplayPos().x);
            EXPECT_EQ(newPos.y, this->display.GetDisplayPos().y);
        }

        TEST_F(HoldDisplayTest, MoveSetsDataStartHeight)
        {
            POINT newPos = { 300, 300 };
            this->display.Move(newPos);
            EXPECT_EQ(300 + this->display.dataStartOffset, this->display.GetDataStartHeight());
        }

        TEST_F(HoldDisplayTest, MoveSetsTitleBarRenderableArea)
        {
            POINT newPos = { 300, 300 };
            this->display.Move(newPos);

            Gdiplus::Rect expected = { 300, 300, 200, 15 };
            EXPECT_TRUE(expected.Equals(this->display.GetTitleArea()));
        }

        TEST_F(HoldDisplayTest, MoveSetsTitleBarClickableArea)
        {
            POINT newPos = { 300, 300 };
            this->display.Move(newPos);

            RECT expected = { 300, 300, 500, 315 };
            EXPECT_TRUE(expected.left == this->display.GetTitleClickArea().left);
            EXPECT_TRUE(expected.top == this->display.GetTitleClickArea().top);
            EXPECT_TRUE(expected.right == this->display.GetTitleClickArea().right);
            EXPECT_TRUE(expected.bottom == this->display.GetTitleClickArea().bottom);
        }

        TEST_F(HoldDisplayTest, MoveSetsMinimiseRenderableArea)
        {
            POINT newPos = { 300, 300 };
            this->display.Move(newPos);

            Gdiplus::Rect expected = { 305, 302, 11, 11 };
            EXPECT_TRUE(expected.Equals(this->display.GetMinimiseArea()));
        }

        TEST_F(HoldDisplayTest, MoveSetsMinmiseClickableArea)
        {
            POINT newPos = { 300, 300 };
            this->display.Move(newPos);

            RECT expected = { 305, 302, 316, 313 };
            EXPECT_TRUE(expected.left == this->display.GetMinimiseClickArea().left);
            EXPECT_TRUE(expected.top == this->display.GetMinimiseClickArea().top);
            EXPECT_TRUE(expected.right == this->display.GetMinimiseClickArea().right);
            EXPECT_TRUE(expected.bottom == this->display.GetMinimiseClickArea().bottom);
        }

        TEST_F(HoldDisplayTest, MoveSetsInformationRenderableArea)
        {
            POINT newPos = { 300, 300 };
            this->display.Move(newPos);

            Gdiplus::Rect expected = { 321, 302, 11, 11 };
            EXPECT_TRUE(expected.Equals(this->display.GetInformationArea()));
        }

        TEST_F(HoldDisplayTest, MoveSetsInformationClickableArea)
        {
            POINT newPos = { 300, 300 };
            this->display.Move(newPos);

            RECT expected = { 321, 302, 332, 313 };
            EXPECT_TRUE(expected.left == this->display.GetInformationClickArea().left);
            EXPECT_TRUE(expected.top == this->display.GetInformationClickArea().top);
            EXPECT_TRUE(expected.right == this->display.GetInformationClickArea().right);
            EXPECT_TRUE(expected.bottom == this->display.GetInformationClickArea().bottom);
        }

        TEST_F(HoldDisplayTest, MoveSetsMinusRenderableArea)
        {
            POINT newPos = { 300, 300 };
            this->display.Move(newPos);

            Gdiplus::Rect expected = { 305, 318, 40, 40 };
            EXPECT_TRUE(expected.Equals(this->display.GetMinusArea()));
        }

        TEST_F(HoldDisplayTest, MoveSetsMinusClickableArea)
        {
            POINT newPos = { 300, 300 };
            this->display.Move(newPos);

            RECT expected = { 305, 318, 345, 358 };
            EXPECT_TRUE(expected.left == this->display.GetMinusClickArea().left);
            EXPECT_TRUE(expected.top == this->display.GetMinusClickArea().top);
            EXPECT_TRUE(expected.right == this->display.GetMinusClickArea().right);
            EXPECT_TRUE(expected.bottom == this->display.GetMinusClickArea().bottom);
        }

        TEST_F(HoldDisplayTest, MoveSetsPlusRenderableArea)
        {
            POINT newPos = { 300, 300 };
            this->display.Move(newPos);

            Gdiplus::Rect expected = { 355, 318, 40, 40 };
            EXPECT_TRUE(expected.Equals(this->display.GetPlusArea()));
        }

        TEST_F(HoldDisplayTest, MoveSetsPlusClickableArea)
        {
            POINT newPos = { 300, 300 };
            this->display.Move(newPos);

            RECT expected = { 355, 318, 395, 358 };
            EXPECT_TRUE(expected.left == this->display.GetPlusClickArea().left);
            EXPECT_TRUE(expected.top == this->display.GetPlusClickArea().top);
            EXPECT_TRUE(expected.right == this->display.GetPlusClickArea().right);
            EXPECT_TRUE(expected.bottom == this->display.GetPlusClickArea().bottom);
        }

        TEST_F(HoldDisplayTest, MoveSetsAllLevelsRenderableArea)
        {
            POINT newPos = { 300, 300 };
            this->display.Move(newPos);

            Gdiplus::Rect expected = { 405, 318, 40, 40 };
            EXPECT_TRUE(expected.Equals(this->display.GetAllArea()));
        }

        TEST_F(HoldDisplayTest, MoveSetsAllLevelsClickableArea)
        {
            POINT newPos = { 300, 300 };
            this->display.Move(newPos);

            RECT expected = { 405, 318, 445, 358 };
            EXPECT_TRUE(expected.left == this->display.GetAllClickArea().left);
            EXPECT_TRUE(expected.top == this->display.GetAllClickArea().top);
            EXPECT_TRUE(expected.right == this->display.GetAllClickArea().right);
            EXPECT_TRUE(expected.bottom == this->display.GetAllClickArea().bottom);
        }

        TEST_F(HoldDisplayTest, MoveSetsAddRenderableArea)
        {
            POINT newPos = { 300, 300 };
            this->display.Move(newPos);

            Gdiplus::Rect expected = { 455, 318, 40, 40 };
            EXPECT_TRUE(expected.Equals(this->display.GetAddArea()));
        }

        TEST_F(HoldDisplayTest, MoveSetsAddClickableArea)
        {
            POINT newPos = { 300, 300 };
            this->display.Move(newPos);

            RECT expected = { 455, 318, 495, 358 };
            EXPECT_TRUE(expected.left == this->display.GetAddClickArea().left);
            EXPECT_TRUE(expected.top == this->display.GetAddClickArea().top);
            EXPECT_TRUE(expected.right == this->display.GetAddClickArea().right);
            EXPECT_TRUE(expected.bottom == this->display.GetAddClickArea().bottom);
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
