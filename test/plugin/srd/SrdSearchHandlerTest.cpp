#include "dialog/DialogManager.h"
#include "mock/MockDialogProvider.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "srd/SrdSearchHandler.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Test;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Srd::SrdSearchHandler;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;

namespace UKControllerPluginTest::Srd {

    class SrdSearchHandlerTest : public Test
    {
        public:
        SrdSearchHandlerTest() : dialog(dialogProvider), handler(55, dialog)
        {
            dialog.AddDialog(this->srdSearchDialogData);
        }

        DialogData srdSearchDialogData = {IDD_SRD_SEARCH, "Test"};
        NiceMock<MockDialogProvider> dialogProvider;
        DialogManager dialog;
        SrdSearchHandler handler;
    };

    TEST_F(SrdSearchHandlerTest, ConfigureCallsDialogOpen)
    {
        EXPECT_CALL(this->dialogProvider, OpenDialog(this->srdSearchDialogData, _)).Times(1);

        this->handler.Configure(55, "", {});
    }

    TEST_F(SrdSearchHandlerTest, TagFunctionCallsDialogOpen)
    {
        EXPECT_CALL(this->dialogProvider, OpenDialog(this->srdSearchDialogData, _)).Times(1);

        NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
        NiceMock<MockEuroScopeCRadarTargetInterface> radarTarget;

        this->handler.TagFunction(flightplan, radarTarget, "", {});
    }

    TEST_F(SrdSearchHandlerTest, ItReturnsAConfigurationMenuItem)
    {
        PopupMenuItem expected;
        expected.firstValue = "Open SRD Search Dialog";
        expected.secondValue = "";
        expected.callbackFunctionId = 55;
        expected.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        expected.disabled = false;
        expected.fixedPosition = false;

        EXPECT_EQ(expected, this->handler.GetConfigurationMenuItem());
    }
} // namespace UKControllerPluginTest::Srd
