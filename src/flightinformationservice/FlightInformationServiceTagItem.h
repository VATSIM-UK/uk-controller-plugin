#pragma once
#include "tag/TagItemInterface.h"
#include "flightplan/FlightplanEventHandlerInterface.h"

namespace UKControllerPlugin {
    namespace FlightInformationService {
        /*
            Handles the TAG item for Flight Information Services
        */
        class FlightInformationServiceTagItem : public Tag::TagItemInterface,
                                                public Flightplan::FlightPlanEventHandlerInterface
        {
            public:
                std::string GetTagItemDescription(int tagItemId) const override;
                void SetTagItemData(Tag::TagData& tagData) override;
                std::string GetServiceForAircraft(std::string aircraft) const;
                void SetServiceForAircraft(std::string aircraft, std::string service);

                void FlightPlanEvent(
                    Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
                    Euroscope::EuroScopeCRadarTargetInterface& radarTarget
                ) override {};
                void FlightPlanDisconnectEvent(
                    Euroscope::EuroScopeCFlightPlanInterface& flightPlan
                ) override;
                void ControllerFlightPlanDataEvent(
                    Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
                    int dataType
                ) override {};

                const int flightInformationServiceTagItem = 116;
                const int flightInformationServiceOrBlankTagItem = 117;

            private:
                std::string GetNoServiceItemString(int tagItemId) const;

                // Maps aircraft to their service
                std::map<std::string, std::string> aircraftServiceMap;
        };
    }  // namespace FlightInformationService
}  // namespace UKControllerPlugin
