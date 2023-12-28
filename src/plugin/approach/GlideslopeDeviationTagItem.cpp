#include "GlideslopeDeviationEstimator.h"
#include "GlideslopeDeviationTagItem.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "runway/Runway.h"
#include "runway/RunwayCollection.h"
#include "tag/TagData.h"

namespace UKControllerPlugin::Approach {
    GlideslopeDeviationTagItem::GlideslopeDeviationTagItem(
        std::shared_ptr<const GlideslopeDeviationEstimator> glideslopeDeviationEstimator,
        std::shared_ptr<const Runway::RunwayCollection> runways)
        : glideslopeDeviationEstimator(glideslopeDeviationEstimator), runways(runways)
    {
        assert(this->glideslopeDeviationEstimator != nullptr && "Glideslope deviation estimator cannot be null");
        assert(this->runways != nullptr && "Runways cannot be null");
    }

    std::string GlideslopeDeviationTagItem::GetTagItemDescription(int tagItemId) const
    {
        switch (tagItemId) {
        case 132:
            return "Glideslope Deviation";
        default:
            throw std::invalid_argument("Invalid tag item ID");
        }
    }

    void GlideslopeDeviationTagItem::SetTagItemData(Tag::TagData& tagData)
    {
        const auto& flightplan = tagData.GetFlightplan();

        // Get the runway
        const auto runway =
            runways->GetByAirfieldAndIdentifier(flightplan.GetDestination(), flightplan.GetArrivalRunway());
        if (runway == nullptr) {
            return;
        }

        // Make sure we're upwind of the runway
        if (std::abs(tagData.GetRadarTarget().GetPosition().DirectionTo(runway->Threshold()) - runway->Heading()) >
            90) {
            return;
        }

        // Calculate the deviation and make sure we're somewhat close
        const auto deviation =
            glideslopeDeviationEstimator->CalculateGlideslopeDeviation(tagData.GetRadarTarget(), *runway);
        if (deviation.perpendicularDistanceFromLocaliser > 15) {
            return;
        }

        if (deviation.localiserRange > 25) {
            return;
        }

        // Set the tag colour to red if we're massively out
        if (deviation.deviation > 300) {
            tagData.SetTagColour(RGB(255, 87, 51));
        }

        // If we're massively out, abbreviate the string
        if (deviation.deviation > 999) {
            tagData.SetItemString(">1k");
            return;
        } else if (deviation.deviation < -999) {
            tagData.SetItemString("<1k");
            return;
        }

        // Set the tag item string
        const auto deviationSign = deviation.deviation >= 0 ? "+" : "";
        tagData.SetItemString(deviationSign + std::to_string(deviation.deviation));
    }
} // namespace UKControllerPlugin::Approach
