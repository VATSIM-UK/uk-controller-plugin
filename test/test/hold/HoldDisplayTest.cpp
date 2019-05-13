#include "pch/pch.h"
#include "euroscope/UserSetting.h"
#include "hold/HoldDisplay.h"
#include "hold/ManagedHold.h"
#include "hold/HoldManager.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockUserSettingProviderInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"

using UKControllerPlugin::Hold::HoldDisplay;
using UKControllerPlugin::Hold::ManagedHold;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
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
                    : display(mockPlugin, hold, holdManager), userSetting(mockUserSettingProvider),
                    hold({ 2, "TIMBA", "TIMBA", 7000, 15000, 301, "left", {} })
                {
                    this->holdManager.AddHold(std::move(hold));
                }

                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
                UserSetting userSetting;
                ManagedHold hold;
                HoldManager holdManager;
                HoldDisplay display;
        };

        TEST_F(HoldDisplayTest, ItSetsLevelsSkippable)
        {
            EXPECT_EQ(8, this->display.maxLevelsSkippable);
        }

        TEST_F(HoldDisplayTest, ItLoadsLevelsSkippedFromAsr)
        {
            ON_CALL(this->mockUserSettingProvider, GetKey("holdProfile1Hold2LevelsSkipped"))
                .WillByDefault(Return("4"));

            this->display.LoadDataFromAsr(userSetting, 1);
            EXPECT_EQ(4, this->display.GetLevelsSkipped());
        }

        TEST_F(HoldDisplayTest, ItDefaultsToNoLevelsSkipped)
        {
            ON_CALL(this->mockUserSettingProvider, GetKey("holdProfile1Hold2LevelsSkipped"))
                .WillByDefault(Return("a"));

            this->display.LoadDataFromAsr(userSetting, 1);
            EXPECT_EQ(0, this->display.GetLevelsSkipped());
        }

        TEST_F(HoldDisplayTest, ItSetsWindowHeightBasedOnLevelsSkipped)
        {
            ON_CALL(this->mockUserSettingProvider, GetKey("holdProfile1Hold2LevelsSkipped"))
                .WillByDefault(Return("4"));

            this->display.LoadDataFromAsr(userSetting, 1);
            int expected = this->display.dataStartOffset + (5 * this->display.lineHeight);
            EXPECT_EQ(expected, this->display.GetWindowHeight());
        }

        TEST_F(HoldDisplayTest, ItLoadsMinimisedFromAsr)
        {
            ON_CALL(this->mockUserSettingProvider, GetKey("holdProfile1Hold2Minimised"))
                .WillByDefault(Return("1"));

            this->display.LoadDataFromAsr(userSetting, 1);
            EXPECT_TRUE(this->display.IsMinimised());
        }

        TEST_F(HoldDisplayTest, ItDefaultsToNotMinimised)
        {
            ON_CALL(this->mockUserSettingProvider, GetKey("holdProfile1Hold2LevelsSkipped"))
                .WillByDefault(Return("a"));

            this->display.LoadDataFromAsr(userSetting, 1);
            EXPECT_FALSE(this->display.IsMinimised());
        }

        TEST_F(HoldDisplayTest, ItLoadsWindowPositionFromAsr)
        {
            ON_CALL(this->mockUserSettingProvider, GetKey("holdProfile1Hold2PositionX"))
                .WillByDefault(Return("151"));

            ON_CALL(this->mockUserSettingProvider, GetKey("holdProfile1Hold2PositionY"))
                .WillByDefault(Return("262"));

            this->display.LoadDataFromAsr(userSetting, 1);

            EXPECT_EQ(151, this->display.GetDisplayPos().x);
            EXPECT_EQ(262, this->display.GetDisplayPos().y);
        }

        TEST_F(HoldDisplayTest, ItDefaultsWindowPosition)
        {
            ON_CALL(this->mockUserSettingProvider, GetKey("holdProfile1Hold2PositionX"))
                .WillByDefault(Return(""));

            ON_CALL(this->mockUserSettingProvider, GetKey("holdProfile1Hold2PositionY"))
                .WillByDefault(Return(""));

            this->display.LoadDataFromAsr(userSetting, 1);
            EXPECT_EQ(100, this->display.GetDisplayPos().x);
            EXPECT_EQ(100, this->display.GetDisplayPos().y);
        }

        TEST_F(HoldDisplayTest, ItSavesLevelsSkippedToTheAsr)
        {
            EXPECT_CALL(this->mockUserSettingProvider, SetKey(_, _, _))
                .Times(3);

            EXPECT_CALL(
                this->mockUserSettingProvider,
                SetKey("holdProfile1Hold2LevelsSkipped", "Hold Profile (Test Profile - TIMBA) Levels Skipped", "3")
            )
                .Times(1);

            this->display.ButtonClicked("minus");
            this->display.ButtonClicked("minus");
            this->display.ButtonClicked("minus");
            this->display.SaveDataToAsr(userSetting, 1, "Test Profile");
        }

        TEST_F(HoldDisplayTest, ItSavesMinimisedToTheAsr)
        {
            EXPECT_CALL(this->mockUserSettingProvider, SetKey(_, _, _))
                .Times(3);

            EXPECT_CALL(
                this->mockUserSettingProvider,
                SetKey("holdProfile1Hold2Minimised", "Hold Profile (Test Profile - TIMBA) Minimised", "1")
            )
                .Times(1);

            this->display.ButtonClicked("minimise");
            this->display.SaveDataToAsr(userSetting, 1, "Test Profile");
        }

        TEST_F(HoldDisplayTest, ItSavesPositionToTheAsr)
        {
            EXPECT_CALL(this->mockUserSettingProvider, SetKey(_, _, _))
                .Times(2);

            EXPECT_CALL(
                this->mockUserSettingProvider,
                SetKey("holdProfile1Hold2PositionX", "Hold Profile (Test Profile - TIMBA) X Pos", "151")
            )
                .Times(1);

            EXPECT_CALL(
                this->mockUserSettingProvider,
                SetKey("holdProfile1Hold2PositionY", "Hold Profile (Test Profile - TIMBA) Y Pos", "262")
            )
                .Times(1);

            this->display.Move({ 151, 262 });
            this->display.SaveDataToAsr(userSetting, 1, "Test Profile");
        }

        TEST_F(HoldDisplayTest, ClickingMinusIncreasesSkippedLevelsUpToALimit)
        {
            EXPECT_EQ(0, this->display.GetLevelsSkipped());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(1, this->display.GetLevelsSkipped());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(2, this->display.GetLevelsSkipped());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(3, this->display.GetLevelsSkipped());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(4, this->display.GetLevelsSkipped());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(5, this->display.GetLevelsSkipped());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(6, this->display.GetLevelsSkipped());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(7, this->display.GetLevelsSkipped());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(8, this->display.GetLevelsSkipped());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(8, this->display.GetLevelsSkipped());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(8, this->display.GetLevelsSkipped());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(8, this->display.GetLevelsSkipped());
        }

        TEST_F(HoldDisplayTest, ClickingMinusIncreasesWindowHeightToALimit)
        {
            int windowHeight = this->display.GetWindowHeight();

            this->display.ButtonClicked("minus");
            EXPECT_EQ(windowHeight - this->display.lineHeight, this->display.GetWindowHeight());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(windowHeight - (2 * this->display.lineHeight), this->display.GetWindowHeight());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(windowHeight - (3 * this->display.lineHeight), this->display.GetWindowHeight());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(windowHeight - (4 * this->display.lineHeight), this->display.GetWindowHeight());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(windowHeight - (5 * this->display.lineHeight), this->display.GetWindowHeight());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(windowHeight - (6 * this->display.lineHeight), this->display.GetWindowHeight());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(windowHeight - (7 * this->display.lineHeight), this->display.GetWindowHeight());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(windowHeight - (8 * this->display.lineHeight), this->display.GetWindowHeight());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(windowHeight - (8 * this->display.lineHeight), this->display.GetWindowHeight());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(windowHeight - (8 * this->display.lineHeight), this->display.GetWindowHeight());
        }

        TEST_F(HoldDisplayTest, ClickingPlusReducesLevelsSkipped)
        {
            EXPECT_EQ(0, this->display.GetLevelsSkipped());
            this->display.ButtonClicked("minus");
            EXPECT_EQ(1, this->display.GetLevelsSkipped());
            this->display.ButtonClicked("plus");
            EXPECT_EQ(0, this->display.GetLevelsSkipped());
            this->display.ButtonClicked("plus");
            EXPECT_EQ(0, this->display.GetLevelsSkipped());
        }

        TEST_F(HoldDisplayTest, ClickingPlusReducesWindowHeight)
        {
            int originalWindowHeight = this->display.GetWindowHeight();

            this->display.ButtonClicked("minus");
            EXPECT_EQ(originalWindowHeight - this->display.lineHeight, this->display.GetWindowHeight());
            this->display.ButtonClicked("plus");
            EXPECT_EQ(originalWindowHeight, this->display.GetWindowHeight());
            this->display.ButtonClicked("plus");
            EXPECT_EQ(originalWindowHeight, this->display.GetWindowHeight());
        }

        TEST_F(HoldDisplayTest, ClickingAllResetsLevelsSkipped)
        {
            this->display.ButtonClicked("minus");
            this->display.ButtonClicked("minus");
            this->display.ButtonClicked("minus");
            EXPECT_EQ(3, this->display.GetLevelsSkipped());
            this->display.ButtonClicked("allLevels");
            EXPECT_EQ(0, this->display.GetLevelsSkipped());
        }

        TEST_F(HoldDisplayTest, ClickingAllResetsWindowHeight)
        {
            int originalWindowHeight = this->display.GetWindowHeight();

            this->display.ButtonClicked("minus");
            this->display.ButtonClicked("minus");
            this->display.ButtonClicked("minus");
            EXPECT_EQ(originalWindowHeight - (3 * this->display.lineHeight), this->display.GetWindowHeight());
            this->display.ButtonClicked("allLevels");
            EXPECT_EQ(originalWindowHeight, this->display.GetWindowHeight());
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

        TEST_F(HoldDisplayTest, ClickingAddAddsAircraftToHoldManager)
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

            this->display.ButtonClicked("add");
            EXPECT_TRUE(this->holdManager.GetAircraftHold("BAW123")->GetHoldParameters().identifier == 2);
        }

        TEST_F(HoldDisplayTest, ClickingAddDoesNothingIfNoAircraftSelected)
        {
            ON_CALL(this->mockPlugin, GetSelectedFlightplan())
                .WillByDefault(Return(nullptr));

            ON_CALL(this->mockPlugin, GetSelectedRadarTarget())
                .WillByDefault(Return(nullptr));

            this->display.ButtonClicked("add");

            EXPECT_EQ(0, this->holdManager.GetManagedHold(2)->CountHoldingAircraft());
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
