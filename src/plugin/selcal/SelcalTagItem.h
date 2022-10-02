#pragma once
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "tag/TagItemInterface.h"

namespace UKControllerPlugin::Selcal {
    class ParsedSelcal;
    class SelcalParser;

    /**
     * Generates the TAG item for parsed SELCAL code.
     */
    class SelcalTagItem : public Tag::TagItemInterface, public Flightplan::FlightPlanEventHandlerInterface
    {
        public:
        SelcalTagItem(std::shared_ptr<SelcalParser> parser);
        void FlightPlanEvent(
            Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) override;
        void FlightPlanDisconnectEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan) override;
        void ControllerFlightPlanDataEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType) override;
        [[nodiscard]] auto GetTagItemDescription(int tagItemId) const -> std::string override;
        void SetTagItemData(Tag::TagData& tagData) override;

        private:
        [[nodiscard]] auto GetFromCache(const std::string& callsign, const int type) const -> std::string;

        // Parses the SELCALs
        const std::shared_ptr<SelcalParser> parser;

        // Caches SELCALs
        std::map<std::string, std::shared_ptr<ParsedSelcal>> cache;

        // The TAG Item IDs
        const static int SELCAL_TAG_ITEM_ID = 128;
        const static int SELCAL_SEPARATOR_TAG_ITEM_ID = 129;
    };
} // namespace UKControllerPlugin::Selcal
