#pragma once
#include "MissedApproachRenderOptions.h"
#include "ownership/ServiceType.h"

namespace UKControllerPlugin::Airfield {
    class AirfieldCollection;
} // namespace UKControllerPlugin::Airfield

namespace UKControllerPlugin::MissedApproach {
    class MissedApproachUserSettingHandler;
    class MissedApproachRenderOptions;

    /**
     * Configures missed approaches.
     */
    class MissedApproachConfigurationDialog
    {
        public:
        MissedApproachConfigurationDialog(
            std::shared_ptr<MissedApproachUserSettingHandler> missedApproachUserSettings,
            const Airfield::AirfieldCollection& airfields);
        static auto CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;

        private:
        auto PrivateWindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
        auto InitDialog(HWND hwnd, LPARAM lParam) -> LRESULT;
        auto SaveDialog(HWND hwnd) -> LRESULT;

        // Global settings
        const std::shared_ptr<MissedApproachUserSettingHandler> missedApproachUserSettings;

        // Airfields
        const Airfield::AirfieldCollection& airfields;

        // ASR specific seettings
        std::shared_ptr<MissedApproachRenderOptions> renderOptions;

        // Map the service types to front-end
        const std::map<Ownership::ServiceType, std::string> serviceTypeMap{
            {Ownership::ServiceType::Tower, "Tower"},
            {Ownership::ServiceType::FinalApproach, "Final Approach"},
            {Ownership::ServiceType::Approach, "Approach"},
        };
    };
} // namespace UKControllerPlugin::MissedApproach
