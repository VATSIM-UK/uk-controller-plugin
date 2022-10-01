#include "PrenoteMessage.h"
#include "PrenoteUserRelevanceChecker.h"
#include "SendNewPrenoteChatAreaMessage.h"
#include "controller/ControllerPosition.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"

namespace UKControllerPlugin::Prenote {

    SendNewPrenoteChatAreaMessage::SendNewPrenoteChatAreaMessage(
        std::shared_ptr<PrenoteUserRelevanceChecker> prenoteRelevance,
        Euroscope::EuroscopePluginLoopbackInterface& plugin)
        : prenoteRelevance(prenoteRelevance), plugin(plugin)
    {
        assert(prenoteRelevance && "Prenote relevance is nullptr");
    }

    void SendNewPrenoteChatAreaMessage::NewMessage(const PrenoteMessage& message)
    {
        if (!prenoteRelevance->IsRelevant(message)) {
            return;
        }

        plugin.ChatAreaMessage(
            "UKCP_COORDINATION",
            "UKCP",
            "Prenote message for " + message.GetCallsign() + " is pending from " +
                message.GetSendingController()->GetCallsign() + ".",
            true,
            true,
            true,
            true,
            true);
    }
} // namespace UKControllerPlugin::Prenote
