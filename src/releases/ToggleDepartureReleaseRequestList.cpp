#include "pch/stdafx.h"
#include "releases/ToggleDepartureReleaseRequestList.h"
#include "euroscope/EuroscopeFlightplanListInterface.h"

namespace UKControllerPlugin {
    namespace Releases {

        ToggleDepartureReleaseRequestList::ToggleDepartureReleaseRequestList(
            Euroscope::EuroscopeFlightplanListInterface& list,
            const int callbackId
        ): list(list), callbackId(callbackId)
        {}

        void ToggleDepartureReleaseRequestList::Configure(int functionId, std::string subject, RECT screenObjectArea)
        {
            this->list.Show();
        }

        Plugin::PopupMenuItem ToggleDepartureReleaseRequestList::GetConfigurationMenuItem() const
        {
            return {
                "Show Departure Release Request List",
                "",
                this->callbackId,
                EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX,
                false,
                false
            };
        }
    } // namespace Releases
} // namespace UKControllerPlugin
