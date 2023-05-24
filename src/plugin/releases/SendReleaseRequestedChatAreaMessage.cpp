#include "DepartureReleaseRelevanceChecker.h"
#include "DepartureReleaseRequest.h"
#include "DepartureReleaseRequestedEvent.h"
#include "SendReleaseRequestedChatAreaMessage.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/GeneralSettingsEntries.h"
#include "euroscope/UserSetting.h"

namespace UKControllerPlugin::Releases {

    SendReleaseRequestedChatAreaMessage::SendReleaseRequestedChatAreaMessage(
        std::shared_ptr<const DepartureReleaseRelevanceChecker> relevanceChecker,
        Euroscope::EuroscopePluginLoopbackInterface& plugin,
        Euroscope::UserSetting& userSettings)
        : relevanceChecker(std::move(relevanceChecker)), plugin(plugin), userSettings(userSettings)
    {
        assert(this->relevanceChecker && "relevanceChecker must not be null");
    }

    void SendReleaseRequestedChatAreaMessage::OnEvent(const DepartureReleaseRequestedEvent& event)
    {
        if (!this->relevanceChecker->IsRelevant(*event.releaseRequest)) {
            LogDebug("Not sending release requested chat area message because it is not relevant");
            return;
        }

        if (!userSettings.GetBooleanEntry(Euroscope::GeneralSettingsEntries::releaseChatAreaMessagesSettingsKey)) {
            LogDebug("Not sending release requested chat area message because user does not want chat area messages");
            return;
        }

        plugin.ChatAreaMessage(
            "UKCP_COORDINATION",
            "UKCP",
            "Departure release requested for " + event.releaseRequest->Callsign() + ".",
            true,
            true,
            true,
            true,
            true);
    }
} // namespace UKControllerPlugin::Releases
