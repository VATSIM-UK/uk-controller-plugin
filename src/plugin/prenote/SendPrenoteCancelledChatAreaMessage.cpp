#include "PrenoteMessage.h"
#include "PrenoteUserRelevanceChecker.h"
#include "SendPrenoteCancelledChatAreaMessage.h"
#include "controller/ControllerPosition.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"

namespace UKControllerPlugin::Prenote {

    SendPrenoteCancelledChatAreaMessage::SendPrenoteCancelledChatAreaMessage(
        std::shared_ptr<PrenoteUserRelevanceChecker> prenoteRelevance,
        Euroscope::EuroscopePluginLoopbackInterface& plugin)
        : prenoteRelevance(prenoteRelevance), plugin(plugin)
    {
        assert(prenoteRelevance && "Prenote relevance is nullptr");
    }

    void SendPrenoteCancelledChatAreaMessage::MessageCancelled(const PrenoteMessage& message)
    {
        if (!prenoteRelevance->IsRelevant(message)) {
            return;
        }

        plugin.ChatAreaMessage(
            "UKCP_COORDINATION",
            "UKCP",
            "Prenote message for " + message.GetCallsign() + " from " + message.GetSendingController()->GetCallsign() +
                " has been cancelled.",
            true,
            true,
            true,
            true,
            true);
    }
} // namespace UKControllerPlugin::Prenote
