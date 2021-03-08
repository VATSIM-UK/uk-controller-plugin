#include "pch/pch.h"
#include "flightinformationservice/FlightInformationServiceTagItem.h"
#include "tag/TagData.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "plugin/PopupMenuItem.h"

using testing::Test;
using UKControllerPlugin::FlightInformationService::FlightInformationServiceTagItem;
using UKControllerPlugin::Tag::TagData;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPlugin::Plugin::PopupMenuItem;

namespace UKControllerPluginTest {
    namespace FlightInformationService {

        class FlightInformationServiceTagItemTest : public Test
        {
            public:

                FlightInformationServiceTagItemTest()
                    : tagItem(plugin, 55)
                {
                    this->pluginReturnedFlightplan =
                        std::make_shared<testing::NiceMock<MockEuroScopeCFlightPlanInterface>>();

                    ON_CALL(*this->pluginReturnedFlightplan, GetCallsign())
                        .WillByDefault(testing::Return("BAW123"));

                    ON_CALL(this->flightplan, GetCallsign())
                        .WillByDefault(testing::Return("BAW123"));
                }

                TagData GetTagData(int tagItemId)
                {
                    return TagData(
                        flightplan,
                        radarTarget,
                        tagItemId,
                        EuroScopePlugIn::TAG_DATA_CORRELATED,
                        itemString,
                        &euroscopeColourCode,
                        &tagColour,
                        &fontSize
                    );
                }

                double fontSize = 24.1;
                COLORREF tagColour = RGB(255, 255, 255);
                int euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_ASSUMED;
                char itemString[16] = "Foooooo";
                testing::NiceMock<MockEuroscopePluginLoopbackInterface> plugin;
                std::shared_ptr<testing::NiceMock<MockEuroScopeCFlightPlanInterface>> pluginReturnedFlightplan;
                testing::NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
                testing::NiceMock<MockEuroScopeCRadarTargetInterface> radarTarget;
                FlightInformationServiceTagItem tagItem;
        };

        TEST_F(FlightInformationServiceTagItemTest, ItDescribesTheBaseTagItem)
        {
            EXPECT_EQ("Flight Information Service", tagItem.GetTagItemDescription(116));
        }

        TEST_F(FlightInformationServiceTagItemTest, ItDescribesTheBlankOptionTagItem)
        {
            EXPECT_EQ("Flight Information Service or Blank", tagItem.GetTagItemDescription(117));
        }

        TEST_F(FlightInformationServiceTagItemTest, ItSetsTagItemDataForBaseTagItem)
        {
            auto tagData = this->GetTagData(116);
            tagItem.SetServiceForAircraft("BAW123", "BASIC");
            tagItem.SetTagItemData(tagData);

            EXPECT_EQ("BASIC", tagData.GetItemString());
        }

        TEST_F(FlightInformationServiceTagItemTest, ItReturnsNoDataForBaseTagItem)
        {
            auto tagData = this->GetTagData(116);
            tagItem.SetTagItemData(tagData);

            EXPECT_EQ("UKFIS", tagData.GetItemString());
        }

        TEST_F(FlightInformationServiceTagItemTest, ItSetsTagItemDataForOrBlankTagItem)
        {
            auto tagData = this->GetTagData(117);
            tagItem.SetServiceForAircraft("BAW123", "TFC");
            tagItem.SetTagItemData(tagData);

            EXPECT_EQ("TFC", tagData.GetItemString());
        }

        TEST_F(FlightInformationServiceTagItemTest, ItReturnsNoDataForOrBlankTagItem)
        {
            auto tagData = this->GetTagData(117);
            tagItem.SetTagItemData(tagData);

            EXPECT_EQ("", tagData.GetItemString());
        }

        TEST_F(FlightInformationServiceTagItemTest, FlightplanDisconnectingClearsTagItem)
        {
            auto tagData = this->GetTagData(117);
            tagItem.SetServiceForAircraft("BAW123", "TFC");

            tagItem.FlightPlanDisconnectEvent(this->flightplan);
            tagItem.SetTagItemData(tagData);

            EXPECT_EQ("", tagData.GetItemString());
        }

        TEST_F(FlightInformationServiceTagItemTest, ItPopulatesTheSelectionMenu)
        {
            PopupMenuItem menuItemBasic;
            menuItemBasic.firstValue = "BASIC";
            menuItemBasic.secondValue = "";
            menuItemBasic.callbackFunctionId = 55;
            menuItemBasic.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemBasic.disabled = false;
            menuItemBasic.fixedPosition = false;

            PopupMenuItem menuItemTraffic;
            menuItemTraffic.firstValue = "TFC";
            menuItemTraffic.secondValue = "";
            menuItemTraffic.callbackFunctionId = 55;
            menuItemTraffic.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemTraffic.disabled = false;
            menuItemTraffic.fixedPosition = false;

            PopupMenuItem menuItemDecon;
            menuItemDecon.firstValue = "DECON";
            menuItemDecon.secondValue = "";
            menuItemDecon.callbackFunctionId = 55;
            menuItemDecon.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemDecon.disabled = false;
            menuItemDecon.fixedPosition = false;

            PopupMenuItem menuItemProcedural;
            menuItemProcedural.firstValue = "PROC";
            menuItemProcedural.secondValue = "";
            menuItemProcedural.callbackFunctionId = 55;
            menuItemProcedural.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemProcedural.disabled = false;
            menuItemProcedural.fixedPosition = false;

            PopupMenuItem menuItemRemove;
            menuItemRemove.firstValue = "--";
            menuItemRemove.secondValue = "";
            menuItemRemove.callbackFunctionId = 55;
            menuItemRemove.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemRemove.disabled = false;
            menuItemRemove.fixedPosition = false;

            RECT expectedArea = {0, 0, 300, 500};
            EXPECT_CALL(this->plugin, TriggerPopupList(RectEq(expectedArea), "UKFIS", 1))
                .Times(1);

            EXPECT_CALL(this->plugin, AddItemToPopupList(menuItemBasic))
                .Times(1);

            EXPECT_CALL(this->plugin, AddItemToPopupList(menuItemTraffic))
                .Times(1);

            EXPECT_CALL(this->plugin, AddItemToPopupList(menuItemDecon))
                .Times(1);

            EXPECT_CALL(this->plugin, AddItemToPopupList(menuItemProcedural))
                .Times(1);

            EXPECT_CALL(this->plugin, AddItemToPopupList(menuItemRemove))
                .Times(1);

            this->tagItem.DisplayFlightInformationServicesMenu(this->flightplan, this->radarTarget, "", {0, 0});
        }

        TEST_F(FlightInformationServiceTagItemTest, SelectingAMenuItemHandlesInvalidFlightplan)
        {
            ON_CALL(this->plugin, GetSelectedFlightplan())
                .WillByDefault(testing::Return(nullptr));

            tagItem.MenuItemClicked(55, "BASIC");


            EXPECT_EQ("", tagItem.GetServiceForAircraft("BAW123"));
        }

        TEST_F(FlightInformationServiceTagItemTest, SelectingAMenuItemSetsTheServiceForTheAircraft)
        {
            ON_CALL(this->plugin, GetSelectedFlightplan())
                .WillByDefault(Return(this->pluginReturnedFlightplan));
            tagItem.MenuItemClicked(55, "BASIC");


            EXPECT_EQ("BASIC", tagItem.GetServiceForAircraft("BAW123"));
        }

        TEST_F(FlightInformationServiceTagItemTest, SelectingAMenuItemRemovesTheServiceForTheAircraft)
        {
            tagItem.SetServiceForAircraft("BAW123", "BASIC");
            ON_CALL(this->plugin, GetSelectedFlightplan())
                .WillByDefault(Return(this->pluginReturnedFlightplan));
            tagItem.MenuItemClicked(55, "--");


            EXPECT_EQ("", tagItem.GetServiceForAircraft("BAW123"));
        }
    }  // namespace FlightInformationService
}  // namespace UKControllerPluginTest
