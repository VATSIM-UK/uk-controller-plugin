#pragma once
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "tag/TagItemInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
    } // namespace Euroscope
    namespace Datablock {
        class DisplayTime;
    } // namespace Datablock
    namespace Flightplan {
        class StoredFlightplanCollection;
    } // namespace Flightplan
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Datablock {

    class EstimatedDepartureTimeEventHandler : public UKControllerPlugin::Tag::TagItemInterface,
                                               public UKControllerPlugin::Flightplan::FlightPlanEventHandlerInterface
    {
        public:
        EstimatedDepartureTimeEventHandler(
            const UKControllerPlugin::Flightplan::StoredFlightplanCollection& storedFlightplans,
            const UKControllerPlugin::Datablock::DisplayTime& displayTime);
        void FlightPlanEvent(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget) override;
        void
        FlightPlanDisconnectEvent(UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan) override;
        void ControllerFlightPlanDataEvent(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType) override;
        [[nodiscard]] auto GetTagItemDescription(int tagItemId) const -> std::string override;
        void SetTagItemData(UKControllerPlugin::Tag::TagData& tagData) override;

        private:
        // Stored flightplans
        const UKControllerPlugin::Flightplan::StoredFlightplanCollection& storedFlightplans;

        // Class for converting timestamps to display times and getting the blank time format
        const UKControllerPlugin::Datablock::DisplayTime& displayTime;
    };
} // namespace UKControllerPlugin::Datablock
