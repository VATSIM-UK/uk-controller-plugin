#include "pch/pch.h"
#include "srd/SrdSearchHandler.h"
#include "plugin/PopupMenuItem.h"
#include "dialog/DialogManager.h"
#include "dialog/DialogData.h"
#include "mock/MockDialogProvider.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"

using UKControllerPlugin::Srd::SrdSearchHandler;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Plugin::PopupMenuItem;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace Srd {

        class SrdSearchHandlerTest : public Test
        {
            public:

                SrdSearchHandlerTest()
                    : dialog(dialogProvider), handler(55, dialog)
                {
                    dialog.AddDialog(this->srdSearchDialogData);
                }

                DialogData srdSearchDialogData = { IDD_SRD_SEARCH, "Test" };
                NiceMock<MockDialogProvider> dialogProvider;
                DialogManager dialog;
                SrdSearchHandler handler;
        };

        TEST_F(SrdSearchHandlerTest, ConfigureCallsDialogOpen)
        {
            EXPECT_CALL(this->dialogProvider, OpenDialog(this->srdSearchDialogData, _))
                .Times(1);

            this->handler.Configure(55, "", {});
        }

        TEST_F(SrdSearchHandlerTest, TagFunctionCallsDialogOpen)
        {
            EXPECT_CALL(this->dialogProvider, OpenDialog(this->srdSearchDialogData, _))
                .Times(1);

            NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
            NiceMock<MockEuroScopeCRadarTargetInterface> radarTarget;

            this->handler.TagFunction(
                flightplan,
                radarTarget,
                "",
                {}
            );
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
    }  // namespace Srd
}  // namespace UKControllerPluginTest
