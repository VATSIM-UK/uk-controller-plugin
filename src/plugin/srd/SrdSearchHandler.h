#pragma once
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/ConfigurableDisplayInterface.h"

namespace UKControllerPlugin::Dialog {
    class DialogManager;
} // namespace UKControllerPlugin::Dialog

namespace UKControllerPlugin::Srd {
    struct SrdSearchParameters;

    /*
        A handler that triggers the SRD searching dialog
    */
    class SrdSearchHandler : public UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface,
                             public UKControllerPlugin::Plugin::FunctionCallEventHandler
    {
        public:
        SrdSearchHandler(int menuCallbackId, const Dialog::DialogManager& dialog);

        // Inherited via ConfigurableDisplayInterface
        void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
        [[nodiscard]] auto GetConfigurationMenuItem() const -> Plugin::PopupMenuItem override;
        void TagFunction(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
            const std::string& context,
            const POINT& mousePos);

        private:
        // Manages the dialogs
        const UKControllerPlugin::Dialog::DialogManager& dialog;

        // The callback id for the menu
        const int menuCallbackId;

        const std::string itemDescription = "Open SRD Search Dialog";

        // The last search parameters used
        std::shared_ptr<SrdSearchParameters> lastSearchParameters;
    };
} // namespace UKControllerPlugin::Srd
