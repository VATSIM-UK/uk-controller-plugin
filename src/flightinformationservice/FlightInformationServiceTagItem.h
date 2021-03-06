#pragma once
#include "tag/TagItemInterface.h"
#include "flightplan/FlightplanEventHandlerInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope
    {
        class EuroscopePluginLoopbackInterface;
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
    } // namespace Euroscope

    namespace FlightInformationService {
        /*
            Handles the TAG item for Flight Information Services
        */
        class FlightInformationServiceTagItem : public Tag::TagItemInterface,
                                                public Flightplan::FlightPlanEventHandlerInterface
        {
            public:
                FlightInformationServiceTagItem(
                    Euroscope::EuroscopePluginLoopbackInterface& plugin,
                    int callbackId
                );
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

                void DisplayFlightInformationServicesMenu(
                    Euroscope::EuroScopeCFlightPlanInterface& flightplan,
                    Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
                    std::string context,
                    const POINT& mousePos
                );
                void MenuItemClicked(int functionId, std::string context);

                const int flightInformationServiceTagItem = 116;
                const int flightInformationServiceOrBlankTagItem = 117;
                const int callbackId;

            private:
                std::string GetNoServiceItemString(int tagItemId) const;

                // Maps aircraft to their service
                std::map<std::string, std::string> aircraftServiceMap;

                // Plugin loopback
                Euroscope::EuroscopePluginLoopbackInterface& plugin;

                // No ukfis selected menu item
                const std::string noUkFisSelected = "--";
        };
    }  // namespace FlightInformationService
}  // namespace UKControllerPlugin
