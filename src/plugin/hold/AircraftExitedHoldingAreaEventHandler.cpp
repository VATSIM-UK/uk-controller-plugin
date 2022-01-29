#include "AircraftExitedHoldingAreaEventHandler.h"
#include "HoldManager.h"
#include "navaids/NavaidCollection.h"

namespace UKControllerPlugin::Hold {

    AircraftExitedHoldingAreaEventHandler::AircraftExitedHoldingAreaEventHandler(
        HoldManager& holdManager, const Navaids::NavaidCollection& navaids)
        : holdManager(holdManager), navaids(navaids)
    {
    }

    void AircraftExitedHoldingAreaEventHandler::ProcessPushEvent(const Push::PushEvent& message)
    {
        const auto data = message.data;
        if (!DataValid(data)) {
            LogWarning("Invalid aircraft exited holding area event" + data.dump());
            return;
        }

        holdManager.RemoveAircraftFromProximityHold(
            data.at("callsign").get<std::string>(), navaids.Get(data.at("navaid_id").get<int>()).identifier);
    }

    auto AircraftExitedHoldingAreaEventHandler::GetPushEventSubscriptions() const
        -> std::set<Push::PushEventSubscription>
    {
        return {{Push::PushEventSubscription::SUB_TYPE_EVENT, "hold.area-exited"}};
    }

    auto AircraftExitedHoldingAreaEventHandler::DataValid(const nlohmann::json& data) const -> bool
    {
        return data.is_object() && data.contains("callsign") && data.at("callsign").is_string() &&
               data.contains("navaid_id") && data.at("navaid_id").is_number_integer();
    }
} // namespace UKControllerPlugin::Hold
