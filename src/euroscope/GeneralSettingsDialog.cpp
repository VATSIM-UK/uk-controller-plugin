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

            // Prenotes
            this->prenoteEnabledCheckbox.SetCheck(
                this->userSettings.GetBooleanEntry(GeneralSettingsEntries::usePrenoteSettingsKey)
            );

            // Initial Altitudes
            this->initialAltitudeEnabledCheckbox.SetCheck(
                this->userSettings.GetBooleanEntry(GeneralSettingsEntries::initialAltitudeToggleSettingsKey)
            );

            // Squawks
            this->squawksEnabledCheckbox.SetCheck(
                this->userSettings.GetBooleanEntry(GeneralSettingsEntries::squawkToggleSettingsKey)
            );

            return TRUE;
        }

        void GeneralSettingsDialog::OnOK(void)
        {
            LogInfo("General Settings Dialog saved");

            // Prenotes Toggle
            this->userSettings.Save(
                GeneralSettingsEntries::usePrenoteSettingsKey,
                GeneralSettingsEntries::usePrenoteSettingsDescription,
                this->prenoteEnabledCheckbox.GetCheck()
            );

            // Initial Altitudes Toggle
            this->userSettings.Save(
                GeneralSettingsEntries::initialAltitudeToggleSettingsKey,
                GeneralSettingsEntries::initialAltitudeToggleSettingsDescription,
                this->initialAltitudeEnabledCheckbox.GetCheck()
            );

            // Squawk Toggle
            this->userSettings.Save(
                GeneralSettingsEntries::squawkToggleSettingsKey,
                GeneralSettingsEntries::squawkToggleSettingsDescription,
                this->squawksEnabledCheckbox.GetCheck()
            );

            this->EndDialog(0);
        }

        void GeneralSettingsDialog::DoDataExchange(CDataExchange * pDX)
        {
            CDialog::DoDataExchange(pDX);
            DDX_Control(pDX, GS_DIALOG_PRENOTE_CHECK, this->prenoteEnabledCheckbox);
            DDX_Control(pDX, GS_DIALOG_IA_CHECK, this->initialAltitudeEnabledCheckbox);
            DDX_Control(pDX, GS_DIALOG_SQUAWK_CHECK, this->squawksEnabledCheckbox);
        }

        BEGIN_MESSAGE_MAP(GeneralSettingsDialog, CDialog)
        END_MESSAGE_MAP()
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
