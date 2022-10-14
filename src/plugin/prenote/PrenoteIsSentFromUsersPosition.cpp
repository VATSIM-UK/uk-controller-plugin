#include "PrenoteIsSentFromUsersPosition.h"
#include "PrenoteMessage.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"

namespace UKControllerPlugin::Prenote {

    PrenoteIsSentFromUsersPosition::PrenoteIsSentFromUsersPosition(
        const Controller::ActiveCallsignCollection& activeCallsigns)
        : activeCallsigns(activeCallsigns)
    {
    }

    auto PrenoteIsSentFromUsersPosition::IsRelevant(const PrenoteMessage& message) const -> bool
    {
        return activeCallsigns.UserHasCallsign() &&
               activeCallsigns.GetUserCallsign().GetNormalisedPosition().GetId() == message.GetSendingControllerId() &&
               message.GetTargetControllerId() != message.GetSendingControllerId();
    }
} // namespace UKControllerPlugin::Prenote
