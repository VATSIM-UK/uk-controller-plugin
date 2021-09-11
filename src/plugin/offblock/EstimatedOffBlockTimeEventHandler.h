#pragma once
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
    namespace Tag {
        class TagData;
    } // namespace Tag
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Datablock {

    class EstimatedOffBlockTimeEventHandler : public UKControllerPlugin::Tag::TagItemInterface
    {
        public:
        EstimatedOffBlockTimeEventHandler(
            const UKControllerPlugin::Flightplan::StoredFlightplanCollection& storedFlightplans,
            const UKControllerPlugin::Datablock::DisplayTime& displayTime);
        [[nodiscard]] auto GetTagItemDescription(int tagItemId) const -> std::string override;
        void SetTagItemData(UKControllerPlugin::Tag::TagData& tagData) override;

        private:
        // Stored flightplans
        const UKControllerPlugin::Flightplan::StoredFlightplanCollection& storedFlightplans;

        // Class for converting timestamps to display times and getting the no time format
        const UKControllerPlugin::Datablock::DisplayTime& displayTime;
    };
} // namespace UKControllerPlugin::Datablock
