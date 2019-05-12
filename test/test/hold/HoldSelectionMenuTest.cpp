#include "pch/pch.h"
#include "hold/HoldManager.h"
#include "hold/HoldProfileManager.h"
#include "hold/HoldDisplayManager.h"
#include "hold/HoldDisplayFactory.h"
#include "hold/HoldSelectionMenu.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockApiInterface.h"
#include "hold/ManagedHold.h"
#include "hold/HoldingData.h"
#include "hold/HoldProfile.h"
#include "helper/Matchers.h"
#include "plugin/PopupMenuItem.h"
#include "euroscope/UserSetting.h"
#include "mock/MockUserSettingProviderInterface.h"

using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::HoldProfileManager;
using UKControllerPlugin::Hold::HoldDisplayFactory;
using UKControllerPlugin::Hold::HoldDisplayManager;
using UKControllerPlugin::Hold::HoldSelectionMenu;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPlugin::Hold::ManagedHold;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::HoldProfile;
using UKControllerPlugin::Plugin::PopupMenuItem;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::_;
using ::testing::InSequence;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldSelectionMenuTest : public Test
        {
            public:
                HoldSelectionMenuTest()
                    : profileManager(mockApi), displayFactory(this->mockPlugin, this->holdManager),
                    holdSelectionMenu(this->holdManager, this->profileManager, this->mockPlugin, 1),
                    userSetting(mockUserSettingProvider)
                {
                    this->mockFlightplan.reset(new NiceMock<MockEuroScopeCFlightPlanInterface>);
                    this->mockRadarTarget.reset(new NiceMock<MockEuroScopeCRadarTargetInterface>);
                    this->holdManager.AddHold(ManagedHold(std::move(this->holdData)));
                    this->holdManager.AddHold(ManagedHold(std::move(this->holdData2)));
                    this->holdManager.AddHold(ManagedHold(std::move(this->holdData3)));
                    this->profileManager.AddProfile(profile1);
                    this->profileManager.AddProfile(profile2);

                    ON_CALL(*this->mockFlightplan, GetCallsign())
                        .WillByDefault(Return("BAW123"));

                    ON_CALL(this->mockUserSettingProvider, GetKey(_))
                        .WillByDefault(Return(""));
                }

                std::shared_ptr <NiceMock<MockEuroScopeCFlightPlanInterface>> mockFlightplan;
                std::shared_ptr <NiceMock<MockEuroScopeCRadarTargetInterface>> mockRadarTarget;
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
                NiceMock<MockApiInterface> mockApi;
                UserSetting userSetting;
                HoldDisplayFactory displayFactory;
                HoldProfileManager profileManager;
                HoldManager holdManager;
                HoldSelectionMenu holdSelectionMenu;
                HoldingData holdData = { 1, "TIMBA", "TIMBA", 7000, 15000, 309, "left", {} };
                HoldingData holdData2 = { 2, "WILLO", "WILLO", 7000, 15000, 309, "left", {} };
                HoldingData holdData3 = { 3, "MAY", "MAY", 7000, 15000, 309, "left", {} };
                HoldProfile profile1 = { 1, "TEST", {1, 2} };
                HoldProfile profile2 = { 2, "TEST", {2, 3} };
        };

        TEST_F(HoldSelectionMenuTest, ItStartsWithNoDisplayManagers)
        {
            EXPECT_EQ(0, this->holdSelectionMenu.CountDisplayManagers());
        }

        TEST_F(HoldSelectionMenuTest, ItAddsHoldDisplayManagers)
        {
            std::shared_ptr<HoldDisplayManager> displayManager = std::make_shared<HoldDisplayManager>(
                profileManager,
                holdManager,
                displayFactory
            );
            this->holdSelectionMenu.AddDisplayManager(displayManager);
            EXPECT_EQ(1, this->holdSelectionMenu.CountDisplayManagers());
        }

        TEST_F(HoldSelectionMenuTest, ItPopulatesTheMenuFromDisplayManagers)
        {
            std::shared_ptr<HoldDisplayManager> displayManager1 = std::make_shared<HoldDisplayManager>(
                profileManager,
                holdManager,
                displayFactory
            );

            std::shared_ptr<HoldDisplayManager> displayManager2 = std::make_shared<HoldDisplayManager>(
                profileManager,
                holdManager,
                displayFactory
            );

            displayManager1->AsrLoadedEvent(this->userSetting);
            displayManager2->AsrLoadedEvent(this->userSetting);
            displayManager1->LoadProfile(1);
            displayManager2->LoadProfile(2);
            this->holdSelectionMenu.AddDisplayManager(displayManager1);
            this->holdSelectionMenu.AddDisplayManager(displayManager2);


            PopupMenuItem menuItemHold1;
            menuItemHold1.firstValue = "TIMBA";
            menuItemHold1.secondValue = "";
            menuItemHold1.callbackFunctionId = 2;
            menuItemHold1.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemHold1.disabled = false;
            menuItemHold1.fixedPosition = false;

            PopupMenuItem menuItemHold2;
            menuItemHold2.firstValue = "WILLO";
            menuItemHold2.secondValue = "";
            menuItemHold2.callbackFunctionId = 3;
            menuItemHold2.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemHold2.disabled = false;
            menuItemHold2.fixedPosition = false;

            PopupMenuItem menuItemHold3;
            menuItemHold3.firstValue = "MAY";
            menuItemHold3.secondValue = "";
            menuItemHold3.callbackFunctionId = 4;
            menuItemHold3.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemHold3.disabled = false;
            menuItemHold3.fixedPosition = false;

            PopupMenuItem menuItemCancel;
            menuItemCancel.firstValue = "--";
            menuItemCancel.secondValue = "";
            menuItemCancel.callbackFunctionId = 1;
            menuItemCancel.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemCancel.disabled = false;
            menuItemCancel.fixedPosition = true;

            RECT expectedArea = { 0, 0, 300, 500 };
            EXPECT_CALL(this->mockPlugin, TriggerPopupList(RectEq(expectedArea), "Hold", 1))
                .Times(1);

            EXPECT_CALL(this->mockPlugin, AddItemToPopupList(menuItemHold1))
                .Times(1);

            // Hold 2 is in common with both profiles, but it should only be added once!
            EXPECT_CALL(this->mockPlugin, AddItemToPopupList(menuItemHold2))
                .Times(1);

            EXPECT_CALL(this->mockPlugin, AddItemToPopupList(menuItemHold3))
                .Times(1);

            EXPECT_CALL(this->mockPlugin, AddItemToPopupList(menuItemCancel))
                .Times(1);

            this->holdSelectionMenu.DisplayMenu(*this->mockFlightplan, *this->mockRadarTarget, "", { 0, 0 });
        }

        TEST_F(HoldSelectionMenuTest, ItDisplaysEmptyMenuIfNoHolds)
        {
            RECT expectedArea = { 0, 0, 300, 500 };
            EXPECT_CALL(this->mockPlugin, TriggerPopupList(RectEq(expectedArea), "Hold", 1))
                .Times(1);

            this->holdSelectionMenu.DisplayMenu(
                *this->mockFlightplan,
                *this->mockRadarTarget,
                "test",
                { 0, 0 }
            );
        }

        TEST_F(HoldSelectionMenuTest, SelectingAMenuItemAddsAircraftToCorrespondingHold)
        {
            ON_CALL(this->mockPlugin, GetSelectedFlightplan())
                .WillByDefault(Return(this->mockFlightplan));

            ON_CALL(this->mockPlugin, GetSelectedRadarTarget())
                .WillByDefault(Return(this->mockRadarTarget));

            // Number 2 - that's hold 1 (base + id);
            this->holdSelectionMenu.MenuItemClicked(2, "test");
            EXPECT_TRUE(this->holdManager.GetManagedHold(1)->HasAircraft("BAW123"));
            EXPECT_FALSE(this->holdManager.GetManagedHold(2)->HasAircraft("BAW123"));
            EXPECT_FALSE(this->holdManager.GetManagedHold(3)->HasAircraft("BAW123"));
        }

        TEST_F(HoldSelectionMenuTest, SelectingAMenuItemDoesNothingIfFlightplanInvalid)
        {
            ON_CALL(this->mockPlugin, GetSelectedFlightplan())
                .WillByDefault(Return(std::shared_ptr<MockEuroScopeCFlightPlanInterface>(NULL)));

            ON_CALL(this->mockPlugin, GetSelectedRadarTarget())
                .WillByDefault(Return(this->mockRadarTarget));

            // Number 2 - that's hold 1 (base + id);
            this->holdSelectionMenu.MenuItemClicked(2, "test");
            EXPECT_FALSE(this->holdManager.GetManagedHold(1)->HasAircraft("BAW123"));
            EXPECT_FALSE(this->holdManager.GetManagedHold(2)->HasAircraft("BAW123"));
            EXPECT_FALSE(this->holdManager.GetManagedHold(3)->HasAircraft("BAW123"));
        }

        TEST_F(HoldSelectionMenuTest, SelectingAMenuItemDoesNothingIfRadarTargetInvalid)
        {
            ON_CALL(this->mockPlugin, GetSelectedFlightplan())
                .WillByDefault(Return(this->mockFlightplan));

            ON_CALL(this->mockPlugin, GetSelectedRadarTarget())
                .WillByDefault(Return(std::shared_ptr<MockEuroScopeCRadarTargetInterface>(NULL)));

            // Number 2 - that's hold 1 (base + id);
            this->holdSelectionMenu.MenuItemClicked(2, "test");
            EXPECT_FALSE(this->holdManager.GetManagedHold(1)->HasAircraft("BAW123"));
            EXPECT_FALSE(this->holdManager.GetManagedHold(2)->HasAircraft("BAW123"));
            EXPECT_FALSE(this->holdManager.GetManagedHold(3)->HasAircraft("BAW123"));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
