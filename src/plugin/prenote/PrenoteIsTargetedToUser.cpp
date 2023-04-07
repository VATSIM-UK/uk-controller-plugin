#include "PrenoteIsTargetedToUser.h"
#include "PrenoteMessage.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"

namespace UKControllerPlugin::Prenote {

    PrenoteIsTargetedToUser::PrenoteIsTargetedToUser(const Controller::ActiveCallsignCollection& activeCallsigns)
        : activeCallsigns(activeCallsigns)
    {
    }

    auto PrenoteIsTargetedToUser::IsRelevant(const PrenoteMessage& message) const -> bool
    {
        return activeCallsigns.UserHasCallsign() &&
               activeCallsigns.GetUserCallsign().GetNormalisedPosition().GetId() == message.GetTargetControllerId() &&
               message.GetTargetControllerId() != message.GetSendingControllerId();
    }
} // namespace UKControllerPlugin::Prenote
