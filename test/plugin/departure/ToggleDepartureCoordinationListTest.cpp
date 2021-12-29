#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPositionCollection.h"
#include "departure/DepartureCoordinationList.h"
#include "departure/ToggleDepartureCoordinationList.h"
#include "dialog/DialogManager.h"
#include "prenote/PrenoteMessageCollection.h"
#include "releases/DepartureReleaseEventHandler.h"

using testing::Test;
using UKControllerPlugin::Departure::DepartureCoordinationList;
using UKControllerPlugin::Departure::ToggleDepartureCoordinationList;
using UKControllerPlugin::Prenote::PrenoteMessageCollection;

namespace UKControllerPluginTest::Departure {

    class ToggleDepartureCoordinationListTest : public Test
    {
        public:
        ToggleDepartureCoordinationListTest()
            : handler(mockApi, taskRunner, mockPlugin, controllers, activeCallsigns, dialogManager, windows, 103, 104),
              list(std::make_shared<DepartureCoordinationList>(
                  handler, prenotes, mockPlugin, controllers, activeCallsigns, 3)),
              dialogManager(dialogProvider)
        {
        }

        ToggleDepartureCoordinationList GetList(bool alreadyToggled)
        {
            if (alreadyToggled) {
                list->ToggleVisible();
            }

            return ToggleDepartureCoordinationList(list, 2);
        }

        PrenoteMessageCollection prenotes;
        UKControllerPlugin::Releases::DepartureReleaseEventHandler handler;
        std::shared_ptr<DepartureCoordinationList> list;
        UKControllerPlugin::Controller::ActiveCallsignCollection activeCallsigns;
        testing::NiceMock<Dialog::MockDialogProvider> dialogProvider;
        testing::NiceMock<Api::MockApiInterface> api;
        UKControllerPlugin::Dialog::DialogManager dialogManager;
        UKControllerPlugin::Controller::ControllerPositionCollection controllers;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> mockPlugin;
        testing::NiceMock<Api::MockApiInterface> mockApi;
        testing::NiceMock<Windows::MockWinApi> windows;
        TaskManager::MockTaskRunnerInterface taskRunner;
    };

    TEST_F(ToggleDepartureCoordinationListTest, ItReturnsAConfigurationItemNotToggled)
    {
        const ToggleDepartureCoordinationList list = GetList(true);

        UKControllerPlugin::Plugin::PopupMenuItem expected{
            "Toggle Departure Coordination List", "", 2, EuroScopePlugIn::POPUP_ELEMENT_CHECKED, false, false};

        EXPECT_EQ(expected, list.GetConfigurationMenuItem());
    }

    TEST_F(ToggleDepartureCoordinationListTest, ItReturnsAConfigurationItemToggled)
    {
        const ToggleDepartureCoordinationList list = GetList(false);

        UKControllerPlugin::Plugin::PopupMenuItem expected{
            "Toggle Departure Coordination List", "", 2, EuroScopePlugIn::POPUP_ELEMENT_UNCHECKED, false, false};

        EXPECT_EQ(expected, list.GetConfigurationMenuItem());
    }

    TEST_F(ToggleDepartureCoordinationListTest, ClickingTheItemTogglesTheList)
    {
        ToggleDepartureCoordinationList toggleList = GetList(true);
        EXPECT_TRUE(list->IsVisible());
        toggleList.Configure(1, "", {});
        EXPECT_FALSE(list->IsVisible());
        toggleList.Configure(1, "", {});
        EXPECT_TRUE(list->IsVisible());
    }
} // namespace UKControllerPluginTest::Departure
