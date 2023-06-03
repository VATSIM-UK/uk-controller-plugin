#include "SquawkAssignmentMenu.h"
#include "SquawkCodeValid.h"
#include "SquawkGeneratorInterface.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"

namespace UKControllerPlugin::Squawk {

    SquawkAssignmentMenu::SquawkAssignmentMenu(
        int callbackId,
        int manualSquawkCallbackId,
        SquawkGeneratorInterface& squawkGenerator,
        Controller::ActiveCallsignCollection& activeCallsigns,
        Euroscope::EuroscopePluginLoopbackInterface& plugin)
        : callbackId(callbackId), manualSquawkCallbackId(manualSquawkCallbackId), squawkGenerator(squawkGenerator),
          activeCallsigns(activeCallsigns), plugin(plugin)
    {
    }

    void SquawkAssignmentMenu::DisplaySquawkAssignmentMenu(
        Euroscope::EuroScopeCFlightPlanInterface& flightplan, const POINT& mousePos)
    {
        // If the user doesn't have a callsign or the callsign isn't active, don't show the menu
        if (!this->activeCallsigns.UserHasCallsign()) {
            return;
        }

        // If the flightplan is tracked by someone else, don't show
        if (flightplan.IsTracked() && !flightplan.IsTrackedByUser()) {
            return;
        }

        // Trigger the menu
        plugin.TriggerPopupList(
            {mousePos.x, mousePos.y, mousePos.x + MENU_WIDTH, mousePos.y + MENU_HEIGHT}, "Assign Squawk", 1);

        // Add the options
        this->AddMenuOptions();
    }

    void SquawkAssignmentMenu::AddMenuOptions()
    {
        Plugin::PopupMenuItem item;
        item.firstValue = "";
        item.secondValue = "";
        item.callbackFunctionId = this->callbackId;
        item.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        item.disabled = false;
        item.fixedPosition = false;

        for (const auto& menuItem : this->squawkOptions) {
            item.firstValue = menuItem;
            this->plugin.AddItemToPopupList(item);
        }
    }

    void SquawkAssignmentMenu::MenuOptionSelected(const std::string& option, const RECT& tagItemArea)
    {
        auto flightplan = this->plugin.GetSelectedFlightplan();
        auto radarTarget = this->plugin.GetSelectedRadarTarget();
        if (!flightplan || !radarTarget) {
            LogWarning("No flightplan selected, cant action squawk menu selection");
            return;
        }

        if (option == GENERAL_SQUAWK) {
            this->squawkGenerator.ForceGeneralSquawkForAircraft(*flightplan, *radarTarget);
            return;
        }

        if (option == LOCAL_SQUAWK) {
            this->squawkGenerator.ForceLocalSquawkForAircraft(*flightplan, *radarTarget);
            return;
        }

        if (option == CONSPICUITY) {
            this->squawkGenerator.DeleteApiSquawkAndSetTo("7000", *flightplan);
            return;
        }

        if (option == CIRCUIT) {
            this->squawkGenerator.DeleteApiSquawkAndSetTo("7010", *flightplan);
            return;
        }

        if (option == MANUAL) {
            plugin.ShowTextEditPopup(
                {tagItemArea.left, tagItemArea.top, tagItemArea.left + 50, tagItemArea.top + 25}, // NOLINT
                manualSquawkCallbackId,
                flightplan->GetAssignedSquawk());
            return;
        }
    }

    void SquawkAssignmentMenu::ManualSquawkEntered(const std::string& squawk)
    {
        auto flightplan = this->plugin.GetSelectedFlightplan();
        auto radarTarget = this->plugin.GetSelectedRadarTarget();
        if (!flightplan || !radarTarget) {
            LogWarning("No flightplan selected, cant action squawk menu selection");
            return;
        }

        if (!SquawkCodeValid(squawk)) {
            LogWarning("Invalid squawk entered: " + squawk);
            return;
        }

        this->squawkGenerator.DeleteApiSquawkAndSetTo(squawk, *flightplan);
    }
} // namespace UKControllerPlugin::Squawk
