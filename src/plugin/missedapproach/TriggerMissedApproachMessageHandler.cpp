#include "TriggerMissedApproach.h"
#include "TriggerMissedApproachMessageHandler.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "integration/MessageInterface.h"

namespace UKControllerPlugin::MissedApproach {

    TriggerMissedApproachMessageHandler::TriggerMissedApproachMessageHandler(
        const TriggerMissedApproach& trigger, const Euroscope::EuroscopePluginLoopbackInterface& plugin)
        : trigger(trigger), plugin(plugin)
    {
    }

    auto TriggerMissedApproachMessageHandler::ActionsToProcess() const -> std::vector<Integration::MessageType>
    {
        return {{"trigger_missed_approach", 1}};
    }

    void TriggerMissedApproachMessageHandler::ProcessAction(
        std::shared_ptr<Integration::MessageInterface> message,
        std::function<void(void)> success,
        std::function<void(std::vector<std::string>)> fail)
    {

        const auto& data = message->GetMessageData();
        if (!data.contains("callsign") || !data.at("callsign").is_string()) {
            fail({"Missed approach message data is invalid"});
            return;
        }

        const auto callsign = data.at("callsign").get<std::string>();
        const auto flightplan = this->plugin.GetFlightplanForCallsign(callsign);
        const auto radarTarget = this->plugin.GetRadarTargetForCallsign(callsign);

        if (!flightplan || !radarTarget) {
            fail({"Flightplan or radar target not found for missed approach"});
            return;
        }

        this->trigger.Trigger(*flightplan, *radarTarget, false, success, fail);
    }
} // namespace UKControllerPlugin::MissedApproach
