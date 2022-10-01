#include "PrenoteIsRelevantToUser.h"
#include "PrenoteMessage.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"

namespace UKControllerPlugin::Prenote {

    PrenoteIsRelevantToUser::PrenoteIsRelevantToUser(const Controller::ActiveCallsignCollection& activeCallsigns)
        : activeCallsigns(activeCallsigns)
    {
    }

    auto PrenoteIsRelevantToUser::IsRelevant(const PrenoteMessage& message) const -> bool
    {
        return activeCallsigns.UserHasCallsign() &&
               activeCallsigns.GetUserCallsign().GetNormalisedPosition().GetId() == message.GetTargetControllerId() &&
               message.GetTargetControllerId() != message.GetSendingControllerId();
    }
} // namespace UKControllerPlugin::Prenote
