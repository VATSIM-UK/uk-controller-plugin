#include "PrenoteMessage.h"
#include "PrenoteUserRelevanceChecker.h"
#include "SendPrenoteAcknowledgedChatAreaMessage.h"
#include "controller/ControllerPosition.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/GeneralSettingsEntries.h"
#include "euroscope/UserSetting.h"

namespace UKControllerPlugin::Prenote {

    SendPrenoteAcknowledgedChatAreaMessage::SendPrenoteAcknowledgedChatAreaMessage(
        std::shared_ptr<PrenoteUserRelevanceChecker> prenoteRelevance,
        Euroscope::EuroscopePluginLoopbackInterface& plugin,
        Euroscope::UserSetting& userSettings)
        : prenoteRelevance(prenoteRelevance), plugin(plugin), userSettings(userSettings)
    {
        assert(prenoteRelevance && "Prenote relevance is nullptr");
    }

    void SendPrenoteAcknowledgedChatAreaMessage::MessageAcknowledged(const PrenoteMessage& message)
    {
        if (!UserWantsChatAreaMessages() || !prenoteRelevance->IsRelevant(message)) {
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

    auto SendPrenoteAcknowledgedChatAreaMessage::UserWantsChatAreaMessages() const -> bool
    {
        return userSettings.GetBooleanEntry(Euroscope::GeneralSettingsEntries::prenoteChatAreaMessagesSettingsKey);
    }
} // namespace UKControllerPlugin::Prenote
