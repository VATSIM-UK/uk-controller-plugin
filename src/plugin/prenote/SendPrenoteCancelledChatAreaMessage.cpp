#include "PrenoteMessage.h"
#include "PrenoteUserRelevanceChecker.h"
#include "SendPrenoteCancelledChatAreaMessage.h"
#include "controller/ControllerPosition.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/GeneralSettingsEntries.h"
#include "euroscope/UserSetting.h"

namespace UKControllerPlugin::Prenote {

    SendPrenoteCancelledChatAreaMessage::SendPrenoteCancelledChatAreaMessage(
        std::shared_ptr<PrenoteUserRelevanceChecker> prenoteRelevance,
        Euroscope::EuroscopePluginLoopbackInterface& plugin,
        Euroscope::UserSetting& userSettings)
        : prenoteRelevance(prenoteRelevance), plugin(plugin), userSettings(userSettings)
    {
        assert(prenoteRelevance && "Prenote relevance is nullptr");
    }

    void SendPrenoteCancelledChatAreaMessage::MessageCancelled(const PrenoteMessage& message)
    {
        if (!UserWantsChatAreaMessages() || !prenoteRelevance->IsRelevant(message)) {
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

    auto SendPrenoteCancelledChatAreaMessage::UserWantsChatAreaMessages() const -> bool
    {
        return userSettings.GetBooleanEntry(Euroscope::GeneralSettingsEntries::prenoteChatAreaMessagesSettingsKey);
    }
} // namespace UKControllerPlugin::Prenote
