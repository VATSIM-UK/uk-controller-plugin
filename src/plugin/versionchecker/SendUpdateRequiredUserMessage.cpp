#include "SendUpdateRequiredUserMessage.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"

namespace UKControllerPlugin::VersionChecker {

    SendUpdateRequiredUserMessage::SendUpdateRequiredUserMessage(Euroscope::EuroscopePluginLoopbackInterface& plugin_)
        : plugin(plugin_)
    {
    }

    void SendUpdateRequiredUserMessage::OnEvent(const UpdateRequiredEvent& event)
    {
        plugin.ChatAreaMessage(
            "UKCP_VERSION_CHECKER",
            "",
            std::format("{} (current {} / latest {})", event.message, event.current, event.latest),
            true,
            true,
            true,
            true,
            true);
    }

} // namespace UKControllerPlugin::VersionChecker
