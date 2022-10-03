#include "PrenoteMessage.h"
#include "PrenoteUserRelevanceChecker.h"
#include "SendPrenoteAcknowledgedChatAreaMessage.h"
#include "controller/ControllerPosition.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"

namespace UKControllerPlugin::Prenote {

    SendPrenoteAcknowledgedChatAreaMessage::SendPrenoteAcknowledgedChatAreaMessage(
        std::shared_ptr<PrenoteUserRelevanceChecker> prenoteRelevance,
        Euroscope::EuroscopePluginLoopbackInterface& plugin)
        : prenoteRelevance(prenoteRelevance), plugin(plugin)
    {
        assert(prenoteRelevance && "Prenote relevance is nullptr");
    }

    void SendPrenoteAcknowledgedChatAreaMessage::MessageAcknowledged(const PrenoteMessage& message)
    {
        if (!prenoteRelevance->IsRelevant(message)) {
            return;
        }

        plugin.ChatAreaMessage(
            "UKCP_COORDINATION",
            "UKCP",
            "Prenote message for " + message.GetCallsign() + " has been acknowledged by " +
                message.GetTargetController()->GetCallsign() + ".",
            true,
            true,
            true,
            true,
            true);
    }
} // namespace UKControllerPlugin::Prenote
