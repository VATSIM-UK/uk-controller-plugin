#pragma once
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "tag/TagItemInterface.h"

namespace UKControllerPlugin {
    namespace Datablock {
        class DisplayTime;
    } // namespace Datablock
    namespace Flightplan {
        class StoredFlightplanCollection;
    } // namespace Flightplan
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Datablock {

    /*
        Handles events to do with actual off block times, including
        TAGs.
    */
    class ActualOffBlockTimeEventHandler : public UKControllerPlugin::Flightplan::FlightPlanEventHandlerInterface,
                                           public UKControllerPlugin::Tag::TagItemInterface
    {
        public:
        ActualOffBlockTimeEventHandler(
            const UKControllerPlugin::Flightplan::StoredFlightplanCollection& flightplans,
            const UKControllerPlugin::Datablock::DisplayTime& displayTime);

        // Inherited via FlightPlanEventHandlerInterface
        void FlightPlanEvent(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget) override;
        void
        FlightPlanDisconnectEvent(UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan) override;
        void ControllerFlightPlanDataEvent(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType) override;

        // Inherited via TagItemInterface
        [[nodiscard]] auto GetTagItemDescription(int tagItemId) const -> std::string override;
        void SetTagItemData(UKControllerPlugin::Tag::TagData& tagData) override;

        private:
        // Stored flightplans
        const UKControllerPlugin::Flightplan::StoredFlightplanCollection& flightplans;

        // Converts times to display format.
        const UKControllerPlugin::Datablock::DisplayTime& displayTime;
    };
} // namespace UKControllerPlugin::Datablock
