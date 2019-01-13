#include "pch/stdafx.h"
#include "euroscope/GeneralSettingsDialog.h"
#include "euroscope/UserSetting.h"
#include "euroscope/GeneralSettingsEntries.h"

using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Euroscope::GeneralSettingsEntries;

namespace UKControllerPlugin {
    namespace Euroscope {

        IMPLEMENT_DYNAMIC(GeneralSettingsDialog, CDialog)

        GeneralSettingsDialog::GeneralSettingsDialog(CWnd * parentWindow, UserSetting & userSettings)
            : CDialog(IDD_GENERAL_SETTINGS, parentWindow), userSettings(userSettings)
        {

        }

        BOOL GeneralSettingsDialog::OnInitDialog(void)
        {
            CDialog::OnInitDialog();

            this->prenoteEnabledCheckbox.SetCheck(
                this->userSettings.GetBooleanEntry(GeneralSettingsEntries::usePrenoteSettingsKey)
            );

            return TRUE;
        }

        void GeneralSettingsDialog::OnOK(void)
        {
            LogInfo("General Settings Dialog saved");
            this->userSettings.Save(
                GeneralSettingsEntries::usePrenoteSettingsKey,
                GeneralSettingsEntries::usePrenoteSettingsDescription,
                this->prenoteEnabledCheckbox.GetCheck()
            );

            this->EndDialog(0);
        }

        void GeneralSettingsDialog::DoDataExchange(CDataExchange * pDX)
        {
            CDialog::DoDataExchange(pDX);
            DDX_Control(pDX, GS_DIALOG_PRENOTE_CHECK, this->prenoteEnabledCheckbox);
        }

        BEGIN_MESSAGE_MAP(GeneralSettingsDialog, CDialog)
        END_MESSAGE_MAP()
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
