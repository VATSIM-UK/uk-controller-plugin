#include "pch/stdafx.h"
#include "releases/ToggleDepartureReleaseDecisionList.h"
#include "DepartureReleaseDecisionList.h"

namespace UKControllerPlugin {
    namespace Releases {

        ToggleDepartureReleaseDecisionList::ToggleDepartureReleaseDecisionList(
            std::shared_ptr<DepartureReleaseDecisionList> list,
            int callbackId
        ): list(list), callbackId(callbackId)
        {}

        void ToggleDepartureReleaseDecisionList::Configure(int functionId, std::string subject, RECT screenObjectArea)
        {
            this->list->ToggleVisible();
        }

        Plugin::PopupMenuItem ToggleDepartureReleaseDecisionList::GetConfigurationMenuItem() const
        {
            return {
                "Show Departure Release Decision List",
                "",
                this->callbackId,
                this->list->IsVisible()
                    ? EuroScopePlugIn::POPUP_ELEMENT_CHECKED
                    : EuroScopePlugIn::POPUP_ELEMENT_UNCHECKED,
                false,
                false
            };
        }
    } // namespace Releases
} // namespace UKControllerPlugin
