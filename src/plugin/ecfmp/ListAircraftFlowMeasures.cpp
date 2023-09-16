#include "AircraftFlowMeasuresDialog.h"
#include "AircraftFlowMeasureMapInterface.h"
#include "ListAircraftFlowMeasures.h"
#include "dialog/DialogManager.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "helper/HelperFunctions.h"

namespace UKControllerPlugin::ECFMP {
    ListAircraftFlowMeasures::ListAircraftFlowMeasures(
        std::shared_ptr<const AircraftFlowMeasureMapInterface> flowMeasureMap,
        const Dialog::DialogManager& dialogManager)
        : flowMeasureMap(std::move(flowMeasureMap)), dialogManager(dialogManager)
    {
        assert(this->flowMeasureMap != nullptr && "Flow measure map cannot be null");
    }

    void ListAircraftFlowMeasures::ListForAircraft(const Euroscope::EuroScopeCFlightPlanInterface& flightplan)
    {
        const auto flowMeasuresForCallsign = flowMeasureMap->GetFlowMeasuresForCallsign(flightplan.GetCallsign());
        if (flowMeasuresForCallsign.empty()) {
            return;
        }

        // Write the flow measures to a string
        std::string flowMeasureString;
        for (const auto& flowMeasure : flowMeasuresForCallsign) {
            flowMeasureString += "Measure: " + flowMeasure->Identifier() + "\r\n\r\n";
            flowMeasureString += flowMeasure->Measure().MeasureDescription() + "\r\n\r\n";

            flowMeasureString += "Applicable To:\r\n\r\n";

            for (const auto& description : flowMeasure->Filters().FilterDescriptions()) {
                flowMeasureString += description + "\r\n";
            }

            flowMeasureString += "\r\n";
            flowMeasureString += "End of measure " + flowMeasure->Identifier() + "\r\n\r\n";
        }

        // Open dialog
        AircraftFlowMeasuresDialogData dialogData{
            .callsign = HelperFunctions::ConvertToWideString(flightplan.GetCallsign()),
            .flowMeasures =
                HelperFunctions::ConvertToWideString(flowMeasureString.substr(0, flowMeasureString.size() - 4))};

        dialogManager.OpenDialog(IDD_FLOW_MEASURE_LIST, reinterpret_cast<LPARAM>(&dialogData));
    }
} // namespace UKControllerPlugin::ECFMP
