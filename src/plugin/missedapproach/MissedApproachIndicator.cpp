#include "MissedApproach.h"
#include "MissedApproachCollection.h"
#include "MissedApproachIndicator.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "tag/TagData.h"

namespace UKControllerPlugin::MissedApproach {
    MissedApproachIndicator::MissedApproachIndicator(const MissedApproachCollection& missedApproaches)
        : missedApproaches(missedApproaches)
    {
    }

    std::string MissedApproachIndicator::GetTagItemDescription(int tagItemId) const
    {
        return "Missed Approach Indicator";
    }

    void MissedApproachIndicator::SetTagItemData(Tag::TagData& tagData)
    {
        const auto missed =
            missedApproaches.FirstWhere([&tagData](const std::shared_ptr<class MissedApproach>& missed) -> bool {
                return missed->Callsign() == tagData.GetFlightplan().GetCallsign() && !missed->IsExpired() &&
                       missed->IsAcknowledged();
            });

        if (!missed) {
            return;
        }

        tagData.SetItemString("GO AROUND");
    }
} // namespace UKControllerPlugin::MissedApproach
