#include "PrenoteMessageStatusView.h"
#include "TriggerPrenoteMessageStatusView.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

namespace UKControllerPlugin::Prenote {
    void TriggerPrenoteMessageStatusView::Trigger(Euroscope::EuroScopeCFlightPlanInterface& flightplan, POINT mousePos)
    {
        PrenoteMessageStatusView::MoveAllInstances(mousePos);
        PrenoteMessageStatusView::DisplayFor(flightplan.GetCallsign(), DISPLAY_DURATION);
    }
} // namespace UKControllerPlugin::Prenote
