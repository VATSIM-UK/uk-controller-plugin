#pragma once
#include "pch/pch.h"
#include "releases/ToggleDepartureReleaseDecisionList.h"
#include "mock/MockEuroscopeFlightplanList.h"
#include "plugin/PopupMenuItem.h"

using testing::Test;
using UKControllerPlugin::Releases::ToggleDepartureReleaseRequestList;
using UKControllerPluginTest::Euroscope::MockEuroscopeFlightplanList;

namespace UKControllerPluginTest {
    namespace Releases {

        class ToggleDepartureReleaseRequestListTest : public Test
        {
            public:
                ToggleDepartureReleaseRequestListTest()
                    : list(mockEuroscopeList, 2)
                {}

                testing::NiceMock<MockEuroscopeFlightplanList> mockEuroscopeList;
                ToggleDepartureReleaseRequestList list;
        };

        TEST_F(ToggleDepartureReleaseRequestListTest, ItReturnsAConfigurationItem)
        {
            UKControllerPlugin::Plugin::PopupMenuItem expected{
                "Show Departure Release Request List",
                "",
                2,
                EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX,
                false,
                false
            };

            EXPECT_EQ(expected, this->list.GetConfigurationMenuItem());
        }

        TEST_F(ToggleDepartureReleaseRequestListTest, ClickingTheItemShowsTheList)
        {
            EXPECT_CALL(this->mockEuroscopeList, Show)
                .Times(1);

            this->list.Configure(2, "", {});
        }
    } // namespace Releases
} // namespace UKControllerPluginTest
