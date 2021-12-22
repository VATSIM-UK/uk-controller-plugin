#include "dialog/DialogCallArgument.h"
#include "dialog/DialogManager.h"
#include "srd/SrdSearchHandler.h"
#include "srd/SrdSearchParameters.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Test;
using UKControllerPlugin::Dialog::DialogCallArgument;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Srd::SrdSearchHandler;
using UKControllerPlugin::Srd::SrdSearchParameters;
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

    TEST_F(SrdSearchHandlerTest, TagFunctionCallsDialogOpenWithNoParameters)
    {
        EXPECT_CALL(this->dialogProvider, OpenDialog(this->srdSearchDialogData, _)).Times(1);

        NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
        NiceMock<MockEuroScopeCRadarTargetInterface> radarTarget;

        this->handler.TagFunction(flightplan, radarTarget, "", {});
    }

    TEST_F(SrdSearchHandlerTest, TagFunctionCallsDialogOpenWithParameters)
    {
        SrdSearchParameters expected{"EGKK", "EGLL", 360, "BAW123"};
        DialogCallArgument actualArg;

        EXPECT_CALL(this->dialogProvider, OpenDialog(this->srdSearchDialogData, testing::_))
            .Times(1)
            .WillOnce(testing::SaveArgPointee<1>(&actualArg));

        NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
        NiceMock<MockEuroScopeCRadarTargetInterface> radarTarget;

        ON_CALL(flightplan, GetOrigin).WillByDefault(testing::Return("EGKK"));

        ON_CALL(flightplan, GetDestination).WillByDefault(testing::Return("EGLL"));

        ON_CALL(flightplan, GetCruiseLevel).WillByDefault(testing::Return(360));

        ON_CALL(flightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));

        this->handler.TagFunction(flightplan, radarTarget, "", {});

        EXPECT_EQ(expected, *reinterpret_cast<SrdSearchParameters*>(actualArg.contextArgument));
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
