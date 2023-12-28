#include "GlideslopeDriftEstimator.h"
#include "GlideslopeDriftTagItem.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "runway/Runway.h"
#include "runway/RunwayCollection.h"
#include "tag/TagData.h"

namespace UKControllerPlugin::Approach {
    GlideslopeDriftTagItem::GlideslopeDriftTagItem(
        std::shared_ptr<const GlideslopeDriftEstimator> glideslopeDriftEstimator,
        std::shared_ptr<const Runway::RunwayCollection> runways)
        : glideslopeDriftEstimator(glideslopeDriftEstimator), runways(runways)
    {
        assert(this->glideslopeDriftEstimator != nullptr && "Glideslope drift estimator cannot be null");
        assert(this->runways != nullptr && "Runways cannot be null");
    }

    std::string GlideslopeDriftTagItem::GetTagItemDescription(int tagItemId) const
    {
        switch (tagItemId) {
        case 132:
            return "Glideslope Deviation";
        default:
            throw std::invalid_argument("Invalid tag item ID");
        }
    }

    void GlideslopeDriftTagItem::SetTagItemData(Tag::TagData& tagData)
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

        // Calculate the drift and make sure we're somewhat close
        const auto drift = glideslopeDriftEstimator->CalculateGlideslopeDrift(tagData.GetRadarTarget(), *runway);
        if (drift.perpendicularDistanceFromLocaliser > 15) {
            return;
        }

        if (drift.localiserRange > 25) {
            return;
        }

        // Set the tag colour based on the drift
        if (std::abs(drift.drift) < 300) {
            tagData.SetTagColour(RGB(2, 48, 32));
        } else {
            tagData.SetTagColour(RGB(255, 0, 0));
        }

        // If we're massively out, abbreviate the string
        if (drift.drift > 999) {
            tagData.SetItemString(">1k");
            return;
        } else if (drift.drift < -999) {
            tagData.SetItemString("<1k");
            return;
        }

        // Set the tag item string
        const auto driftSign = drift.drift >= 0 ? "+" : "";
        tagData.SetItemString(driftSign + std::to_string(drift.drift));
    }
} // namespace UKControllerPlugin::Approach
