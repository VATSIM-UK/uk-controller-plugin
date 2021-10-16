#include "MissedApproachConfigurationDialog.h"
#include "airfield/AirfieldCollection.h"

namespace UKControllerPlugin::MissedApproach {

    MissedApproachConfigurationDialog::MissedApproachConfigurationDialog(
        std::shared_ptr<MissedApproachUserSettingHandler> missedApproachUserSettings,
        const Airfield::AirfieldCollection& airfields)
        : missedApproachUserSettings(std::move(missedApproachUserSettings)), airfields(airfields)
    {
        auto test = this->airfields.GetSize();
        test = test / 2;
    }

    auto CALLBACK MissedApproachConfigurationDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        -> LRESULT
    {
        return TRUE;
    }
} // namespace UKControllerPlugin::MissedApproach
