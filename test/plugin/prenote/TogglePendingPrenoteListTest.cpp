#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPositionCollection.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "prenote/PendingPrenoteList.h"
#include "prenote/PrenoteMessageCollection.h"
#include "prenote/TogglePendingPrenoteList.h"

using testing::Test;
using UKControllerPlugin::Prenote::PendingPrenoteList;
using UKControllerPlugin::Prenote::PrenoteMessageCollection;
using UKControllerPlugin::Prenote::TogglePendingPrenoteList;

namespace UKControllerPluginTest::Prenote {

    class TogglePendingPrenoteListTest : public Test
    {
        public:
        TogglePendingPrenoteListTest()
            : messages(std::make_shared<PrenoteMessageCollection>()),
              list(std::make_shared<PendingPrenoteList>(nullptr, nullptr, mockPlugin, controllers, activeCallsigns, 1))
        {
        }

        TogglePendingPrenoteList GetList(bool alreadyToggled) const
        {
            if (alreadyToggled) {
                list->ToggleVisible();
            }

            return {list, 55};
        }

        std::shared_ptr<PrenoteMessageCollection> messages;
        UKControllerPlugin::Controller::ActiveCallsignCollection activeCallsigns;
        UKControllerPlugin::Controller::ControllerPositionCollection controllers;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> mockPlugin;
        std::shared_ptr<PendingPrenoteList> list;
    };

    TEST_F(TogglePendingPrenoteListTest, ItReturnsAConfigurationItemNotToggled)
    {
        auto toggle = GetList(true);

        UKControllerPlugin::Plugin::PopupMenuItem expected{
            "Toggle Pending Prenote List", "", 55, EuroScopePlugIn::POPUP_ELEMENT_CHECKED, false, false};

        EXPECT_EQ(expected, toggle.GetConfigurationMenuItem());
    }

    TEST_F(TogglePendingPrenoteListTest, ItReturnsAConfigurationItemToggled)
    {
        auto toggle = GetList(false);

        UKControllerPlugin::Plugin::PopupMenuItem expected{
            "Toggle Pending Prenote List", "", 55, EuroScopePlugIn::POPUP_ELEMENT_UNCHECKED, false, false};

        EXPECT_EQ(expected, toggle.GetConfigurationMenuItem());
    }

    TEST_F(TogglePendingPrenoteListTest, ClickingTheItemTogglesTheList)
    {
        auto toggle = GetList(true);
        EXPECT_TRUE(list->IsVisible());
        toggle.Configure(1, "", {});
        EXPECT_FALSE(list->IsVisible());
        toggle.Configure(1, "", {});
        EXPECT_TRUE(list->IsVisible());
    }
} // namespace UKControllerPluginTest::Prenote
