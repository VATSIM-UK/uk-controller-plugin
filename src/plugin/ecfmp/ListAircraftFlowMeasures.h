#pragma once

namespace UKControllerPlugin {
    namespace Dialog {
        class DialogManager;
    } // namespace Dialog
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin::ECFMP {
    class AircraftFlowMeasureMapInterface;

    class ListAircraftFlowMeasures
    {
        public:
        ListAircraftFlowMeasures(
            std::shared_ptr<const AircraftFlowMeasureMapInterface> flowMeasureMap,
            const Dialog::DialogManager& dialogManager);
        void ListForAircraft(const Euroscope::EuroScopeCFlightPlanInterface& flightplan);

        private:
        // The flow measure map
        std::shared_ptr<const AircraftFlowMeasureMapInterface> flowMeasureMap;

        // Dialog manager
        const Dialog::DialogManager& dialogManager;
    };

} // namespace UKControllerPlugin::ECFMP
