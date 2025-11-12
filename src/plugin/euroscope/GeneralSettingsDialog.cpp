#include "GeneralSettingsDialog.h"
#include "GeneralSettingsEntries.h"
#include "UserSettingAwareCollection.h"
#include "UserSetting.h"
#include "dialog/DialogCallArgument.h"
#include "setting/SettingRepository.h"
#include "graphics/ThemingModule.h"
#include "graphics/GdiplusBrushes.h"

using UKControllerPlugin::Dialog::DialogCallArgument;
using UKControllerPlugin::Euroscope::GeneralSettingsEntries;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Euroscope::UserSettingAwareCollection;
using UKControllerPlugin::Graphics::ThemingModule;
using UKControllerPlugin::Windows::GdiplusBrushes;


namespace UKControllerPlugin {
    namespace Euroscope {

        GeneralSettingsDialog::GeneralSettingsDialog(
            UserSetting& userSettings,
            const UserSettingAwareCollection& userSettingsHandlers,
            Setting::SettingRepository& settings,
            GdiplusBrushes& brushes)
            : userSettings(userSettings), brushes(brushes), userSettingsHandlers(userSettingsHandlers), settings(settings)
        {
        }

        GeneralSettingsDialog::GeneralSettingsDialog(const GeneralSettingsDialog& newObject)
            : userSettings(newObject.userSettings), brushes(newObject.brushes), userSettingsHandlers(newObject.userSettingsHandlers),
              settings(newObject.settings)
        {
        }

        LRESULT GeneralSettingsDialog::InitDialog(HWND hwnd)
        {
            CheckDlgButton(
                hwnd,
                GS_DIALOG_PRENOTE_CHECK,
                this->GetCheckboxStateFromSettings(GeneralSettingsEntries::usePrenoteSettingsKey));

            CheckDlgButton(
                hwnd,
                IDC_PRENOTE_CHAT_MESSAGE,
                this->GetCheckboxStateFromSettings(GeneralSettingsEntries::prenoteChatAreaMessagesSettingsKey));

            CheckDlgButton(
                hwnd,
                IDC_RELEASE_CHAT_MESSAGE,
                this->GetCheckboxStateFromSettings(GeneralSettingsEntries::releaseChatAreaMessagesSettingsKey));

            CheckDlgButton(
                hwnd,
                GS_DIALOG_IA_CHECK,
                this->GetCheckboxStateFromSettings(GeneralSettingsEntries::initialAltitudeToggleSettingsKey));

            CheckDlgButton(
                hwnd,
                GS_DIALOG_IH_CHECK,
                this->GetCheckboxStateFromSettings(GeneralSettingsEntries::initialHeadingToggleSettingsKey));

            CheckDlgButton(
                hwnd,
                GS_DIALOG_SQUAWK_CHECK,
                this->GetCheckboxStateFromSettings(GeneralSettingsEntries::squawkToggleSettingsKey));

            CheckDlgButton(
                hwnd,
                GS_DIALOG_QNH_CHECK,
                this->GetCheckboxStateFromSettings(GeneralSettingsEntries::pressureMonitorSendMessageKey));

            CheckDlgButton(
                hwnd,
                GS_TIME_FORMAT_CHECK,
                this->GetCheckboxStateFromSettings(GeneralSettingsEntries::unknownTimeFormatBlankKey));

            auto selectedChannel = this->settings.GetSetting("release_channel", DEFAULT_RELEASE_CHANNEL);
            if (this->releaseChannelMap.count(selectedChannel) == 0) {
                selectedChannel = DEFAULT_RELEASE_CHANNEL;
            }

            for (const auto& releaseChannel : this->releaseChannelMap) {
                const auto channel = releaseChannel.second.c_str();
                int insertIndex = SendDlgItemMessage(
                    hwnd, IDC_RELEASE_CHANNEL, CB_INSERTSTRING, NULL, reinterpret_cast<LPARAM>(channel));

                SendDlgItemMessage(
                    hwnd,
                    IDC_RELEASE_CHANNEL,
                    CB_SETITEMDATA,
                    insertIndex,
                    reinterpret_cast<LPARAM>(releaseChannel.first.c_str()));

                if (releaseChannel.first == selectedChannel) {
                    SendDlgItemMessage(hwnd, IDC_RELEASE_CHANNEL, CB_SETCURSEL, insertIndex, NULL);
                }
            }

            // Colour Palette
            auto selectedColourPalette = this->userSettings.GetStringEntry(
                GeneralSettingsEntries::colourPaletteSettingsKey, DEFAULT_COLOUR_PALETTE);

            if (this->colourPaletteMap.count(selectedColourPalette) == 0) {
                selectedColourPalette = DEFAULT_COLOUR_PALETTE;
            }

            for (const auto& palette : this->colourPaletteMap) {
                const auto paletteName = palette.second.c_str();
                int insertIndex = SendDlgItemMessage(
                    hwnd, IDC_COLOUR_PALETTE, CB_INSERTSTRING, NULL, reinterpret_cast<LPARAM>(paletteName));

                SendDlgItemMessage(
                    hwnd,
                    IDC_COLOUR_PALETTE,
                    CB_SETITEMDATA,
                    insertIndex,
                    reinterpret_cast<LPARAM>(palette.first.c_str()));

                if (palette.first == selectedColourPalette) {
                    SendDlgItemMessage(hwnd, IDC_COLOUR_PALETTE, CB_SETCURSEL, insertIndex, NULL);
                }
            }

            return TRUE;
        }

        void GeneralSettingsDialog::DestroyDialog(HWND hwnd)
        {
            LogInfo("General settings dialog saved");

            // Prenotes Toggle
            this->userSettings.Save(
                GeneralSettingsEntries::usePrenoteSettingsKey,
                GeneralSettingsEntries::usePrenoteSettingsDescription,
                this->GetSettingFromCheckboxState(hwnd, GS_DIALOG_PRENOTE_CHECK));

            // Prenotes in chat area
            this->userSettings.Save(
                GeneralSettingsEntries::prenoteChatAreaMessagesSettingsKey,
                GeneralSettingsEntries::prenoteChatAreaMessagesSettingsDescription,
                this->GetSettingFromCheckboxState(hwnd, IDC_PRENOTE_CHAT_MESSAGE));

            // Releases in chat area
            this->userSettings.Save(
                GeneralSettingsEntries::releaseChatAreaMessagesSettingsKey,
                GeneralSettingsEntries::releaseChatAreaMessagesSettingsDescription,
                this->GetSettingFromCheckboxState(hwnd, IDC_RELEASE_CHAT_MESSAGE));

            // Initial Altitudes Toggle
            this->userSettings.Save(
                GeneralSettingsEntries::initialAltitudeToggleSettingsKey,
                GeneralSettingsEntries::initialAltitudeToggleSettingsDescription,
                this->GetSettingFromCheckboxState(hwnd, GS_DIALOG_IA_CHECK));

            // Initial Headings Toggle
            this->userSettings.Save(
                GeneralSettingsEntries::initialHeadingToggleSettingsKey,
                GeneralSettingsEntries::initialHeadingToggleSettingsDescription,
                this->GetSettingFromCheckboxState(hwnd, GS_DIALOG_IH_CHECK));

            // Squawk Toggle
            this->userSettings.Save(
                GeneralSettingsEntries::squawkToggleSettingsKey,
                GeneralSettingsEntries::squawkToggleSettingsDescription,
                this->GetSettingFromCheckboxState(hwnd, GS_DIALOG_SQUAWK_CHECK));

            // Pressure monitor
            this->userSettings.Save(
                GeneralSettingsEntries::pressureMonitorSendMessageKey,
                GeneralSettingsEntries::pressureMonitorSendMessageDescription,
                this->GetSettingFromCheckboxState(hwnd, GS_DIALOG_QNH_CHECK));

            // Time format
            this->userSettings.Save(
                GeneralSettingsEntries::unknownTimeFormatBlankKey,
                GeneralSettingsEntries::unknownTimeFormatBlankDescription,
                this->GetSettingFromCheckboxState(hwnd, GS_TIME_FORMAT_CHECK));

            const auto selectedReleaseChannelIndex = SendDlgItemMessage(hwnd, IDC_RELEASE_CHANNEL, CB_GETCURSEL, 0, 0);

            const std::string selectedChannel = reinterpret_cast<const char*>(
                SendDlgItemMessage(hwnd, IDC_RELEASE_CHANNEL, CB_GETITEMDATA, selectedReleaseChannelIndex, 0));

            this->settings.UpdateSetting("release_channel", selectedChannel);

            // Colour Palette
            const auto selectedColourPaletteIndex = SendDlgItemMessage(hwnd, IDC_COLOUR_PALETTE, CB_GETCURSEL, 0, 0);

            const std::string selectedColourPalette = reinterpret_cast<const char*>(
                SendDlgItemMessage(hwnd, IDC_COLOUR_PALETTE, CB_GETITEMDATA, selectedColourPaletteIndex, 0));

            this->userSettings.Save(
                GeneralSettingsEntries::colourPaletteSettingsKey,
                GeneralSettingsEntries::colourPaletteSettingsDescription,
                selectedColourPalette);

            ThemingModule::ApplyTheme(selectedColourPalette, this->brushes);
            
            this->userSettingsHandlers.UserSettingsUpdateEvent(this->userSettings);
        }

        /*
            Public procedure that windows calls directly
        */
        LRESULT GeneralSettingsDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (msg == WM_INITDIALOG) {
                LogInfo("General settings dialog opened");
                SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<DialogCallArgument*>(lParam)->dialogArgument);
            } else if (msg == WM_DESTROY) {
                SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
                LogInfo("General settings dialog closed");
            }

            GeneralSettingsDialog* dialog =
                reinterpret_cast<GeneralSettingsDialog*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            return dialog ? dialog->_WndProc(hwnd, msg, wParam, lParam) : FALSE;
        }

        /*
            Private procedure that handles calls on the object
        */
        LRESULT GeneralSettingsDialog::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            switch (msg) {
            // Initialise
            case WM_INITDIALOG: {
                this->InitDialog(hwnd);
                return TRUE;
            };
            // Window Closed
            case WM_CLOSE: {
                EndDialog(hwnd, wParam);
                return TRUE;
            }
            // Buttons pressed
            case WM_COMMAND: {
                switch (LOWORD(wParam)) {
                case IDCANCEL: {
                    EndDialog(hwnd, wParam);
                    return TRUE;
                }
                case IDOK: {
                    // OK clicked, close the window
                    this->DestroyDialog(hwnd);
                    EndDialog(hwnd, wParam);
                    return TRUE;
                }
                }
            }
            }

            return FALSE;
        }

        /*
            Get the appropriate checkbox state from settings.
        */
        bool GeneralSettingsDialog::GetCheckboxStateFromSettings(std::string setting)
        {
            return this->userSettings.GetBooleanEntry(setting) ? BST_CHECKED : BST_UNCHECKED;
        }

        /*
            Get the setting to save based on the checkbox value
        */
        bool GeneralSettingsDialog::GetSettingFromCheckboxState(HWND hwnd, int checkboxId)
        {
            return IsDlgButtonChecked(hwnd, checkboxId) == BST_CHECKED;
        }
    } // namespace Euroscope
} // namespace UKControllerPlugin
