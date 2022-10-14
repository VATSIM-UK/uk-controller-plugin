#include "PrenoteMessage.h"
#include "PrenoteUserRelevanceChecker.h"
#include "SendPrenoteTimeoutChatAreaMessage.h"
#include "controller/ControllerPosition.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/GeneralSettingsEntries.h"
#include "euroscope/UserSetting.h"

namespace UKControllerPlugin::Prenote {

    SendPrenoteTimeoutChatAreaMessage::SendPrenoteTimeoutChatAreaMessage(
        std::shared_ptr<PrenoteUserRelevanceChecker> prenoteRelevance,
        Euroscope::EuroscopePluginLoopbackInterface& plugin,
        Euroscope::UserSetting& userSettings)
        : prenoteRelevance(prenoteRelevance), plugin(plugin), userSettings(userSettings)
    {
        assert(prenoteRelevance && "Prenote relevance is nullptr");
    }

    void SendPrenoteTimeoutChatAreaMessage::MessageTimeout(const PrenoteMessage& message)
    {
        if (message.IsAcknowledged() || !UserWantsChatAreaMessages() || !prenoteRelevance->IsRelevant(message)) {
            return;
        }

        plugin.ChatAreaMessage(
            "UKCP_COORDINATION",
            "UKCP",
            "Prenote message to " + message.GetTargetController()->GetCallsign() + " for " + message.GetCallsign() +
                " has timed out without acknowledgement.",
            true,
            true,
            true,
            true,
            true);
    }

    auto SendPrenoteTimeoutChatAreaMessage::UserWantsChatAreaMessages() const -> bool
    {
        return userSettings.GetBooleanEntry(Euroscope::GeneralSettingsEntries::prenoteChatAreaMessagesSettingsKey);
    }
} // namespace UKControllerPlugin::Prenote
