#pragma once
#include "radarscreen/ConfigurableDisplayInterface.h"

namespace UKControllerPlugin::Prenote {
    class PendingPrenoteList;

    /*
     * A class for the configuration menu that toggles
     * the pending prenotes list.
     */
    class TogglePendingPrenoteList : public RadarScreen::ConfigurableDisplayInterface
    {
        public:
        TogglePendingPrenoteList(std::shared_ptr<PendingPrenoteList> list, int callbackId);
        void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
        [[nodiscard]] auto GetConfigurationMenuItem() const -> Plugin::PopupMenuItem override;

        private:
        // The list
        std::shared_ptr<PendingPrenoteList> list;

        // Callback for the list
        const int callbackId;
    };
} // namespace UKControllerPlugin::Prenote
