#include "AircraftEnteredHoldingAreaEventHandler.h"
#include "HoldManager.h"
#include "ProximityHold.h"
#include "navaids/NavaidCollection.h"
#include "api/ApiRequestFactory.h"
#include "time/ParseTimeStrings.h"

namespace UKControllerPlugin::Hold {

    AircraftEnteredHoldingAreaEventHandler::AircraftEnteredHoldingAreaEventHandler(
        HoldManager& holdManager, const Navaids::NavaidCollection& navaids)
        : holdManager(holdManager), navaids(navaids)
    {
    }

    void AircraftEnteredHoldingAreaEventHandler::ProcessPushEvent(const Push::PushEvent& message)
    {
        this->ProcessData(message.data);
    }

    auto AircraftEnteredHoldingAreaEventHandler::GetPushEventSubscriptions() const
        -> std::set<Push::PushEventSubscription>
    {
        return {{Push::PushEventSubscription::SUB_TYPE_EVENT, "hold.area-entered"}};
    }

    void AircraftEnteredHoldingAreaEventHandler::PluginEventsSynced()
    {
        ApiRequest().Get("hold/proximity").Then([this](const UKControllerPluginUtils::Api::Response response) {
            if (!response.Data().is_array()) {
                LogWarning("Aircraft holding proximity sync data invalid");
                return;
            }

            for (const auto& item : response.Data()) {
                ProcessData(item);
            }
        });
    }

    void AircraftEnteredHoldingAreaEventHandler::ProcessData(const nlohmann::json& data) const
    {
        if (!DataValid(data)) {
            LogWarning("Invalid aircraft entered holding area event" + data.dump());
            return;
        }

        holdManager.AddAircraftToProximityHold(std::make_shared<ProximityHold>(
            data.at("callsign").get<std::string>(),
            navaids.Get(data.at("navaid_id").get<int>()).identifier,
            Time::ParseIsoZuluString(data.at("entered_at").get<std::string>())));
    }

    auto AircraftEnteredHoldingAreaEventHandler::DataValid(const nlohmann::json& data) const -> bool
    {
        return data.is_object() && data.contains("callsign") && data.at("callsign").is_string() &&
               data.contains("navaid_id") && data.at("navaid_id").is_number_integer() &&
               navaids.Get(data.at("navaid_id").get<int>()) != navaids.invalidNavaid && data.contains("entered_at") &&
               data.at("entered_at").is_string() &&
               Time::ParseIsoZuluString(data.at("entered_at").get<std::string>()) != Time::invalidTime;
    }
} // namespace UKControllerPlugin::Hold
