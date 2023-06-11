#include "DepartureHandoffResolver.h"
#include "HandoffEventHandler.h"
#include "HandoffFrequencyUpdatedMessage.h"
#include "ResolvedHandoff.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"
#include "datablock/DatablockFunctions.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "tag/TagData.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Integration::OutboundIntegrationEventHandler;
using UKControllerPlugin::Tag::TagData;

namespace UKControllerPlugin::Handoff {

    HandoffEventHandler::HandoffEventHandler(std::shared_ptr<DepartureHandoffResolver> resolver)
        : resolver(std::move(resolver))
    {
        assert(this->resolver && "Resolver cannot be null");
    }

    auto HandoffEventHandler::GetTagItemDescription(int tagItemId) const -> std::string
    {
        return "Departure Handoff Next Controller";
    }

    void HandoffEventHandler::SetTagItemData(TagData& tagData)
    {
        const auto handoff = resolver->Resolve(tagData.GetFlightplan());
        if (!handoff) {
            LogError("Failed to resolve handoff for " + tagData.GetFlightplan().GetCallsign());
            return;
        }

        tagData.SetItemString(Datablock::FrequencyStringFromDouble(handoff->resolvedController->GetFrequency()));
    }

    void HandoffEventHandler::FlightPlanEvent(
        EuroScopeCFlightPlanInterface& flightPlan, EuroScopeCRadarTargetInterface& radarTarget)
    {
        resolver->Invalidate(flightPlan);
        static_cast<void>(resolver->Resolve(flightPlan));
    }

    void HandoffEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface& flightPlan)
    {
        resolver->Invalidate(flightPlan);
    }

    void HandoffEventHandler::ControllerFlightPlanDataEvent(EuroScopeCFlightPlanInterface& flightPlan, int dataType)
    {
        // No change required here.
    }
} // namespace UKControllerPlugin::Handoff
