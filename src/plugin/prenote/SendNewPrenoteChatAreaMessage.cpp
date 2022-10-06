#include "PrenoteMessage.h"
#include "PrenoteUserRelevanceChecker.h"
#include "SendNewPrenoteChatAreaMessage.h"
#include "controller/ControllerPosition.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/GeneralSettingsEntries.h"
#include "euroscope/UserSetting.h"

namespace UKControllerPlugin::Prenote {

    SendNewPrenoteChatAreaMessage::SendNewPrenoteChatAreaMessage(
        std::shared_ptr<PrenoteUserRelevanceChecker> prenoteRelevance,
        Euroscope::EuroscopePluginLoopbackInterface& plugin,
        Euroscope::UserSetting& userSettings)
        : prenoteRelevance(prenoteRelevance), plugin(plugin), userSettings(userSettings)
    {
        assert(prenoteRelevance && "Prenote relevance is nullptr");
    }

    void SendNewPrenoteChatAreaMessage::NewMessage(const PrenoteMessage& message)
    {
        if (!UserWantsChatAreaMessages() || !prenoteRelevance->IsRelevant(message)) {
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

    auto SendNewPrenoteChatAreaMessage::UserWantsChatAreaMessages() const -> bool
    {
        return userSettings.GetBooleanEntry(Euroscope::GeneralSettingsEntries::prenoteChatAreaMessagesSettingsKey);
    }
} // namespace UKControllerPlugin::Prenote
