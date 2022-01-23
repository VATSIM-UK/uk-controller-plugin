#include "SidCollection.h"
#include "SidCollectionFactory.h"
#include "StandardInstrumentDeparture.h"

namespace UKControllerPlugin::Sid {

    auto MakeSidCollection(const nlohmann::json& sidData) -> std::unique_ptr<SidCollection>
    {
        std::unique_ptr<SidCollection> sids = std::make_unique<SidCollection>();

        if (!SidDataValid(sidData)) {
            LogWarning("Sids data is not valid");
            return std::move(sids);
        }

        for (auto sid : sidData) {
            sids->AddSid(std::make_shared<StandardInstrumentDeparture>(
                sid.at("id").get<int>(),
                sid.at("runway_id").get<int>(),
                sid.at("identifier").get<std::string>(),
                sid.at("initial_altitude").get<int>(),
                sid.at("initial_heading").is_number_integer() ? sid.at("initial_heading").get<int>() : 0,
                sid.at("handoff").is_number_integer() ? sid.at("handoff").get<int>() : 0,
                sid.at("prenotes").get<std::set<int>>()));
        }

        LogInfo("Loaded " + std::to_string(sids->CountSids()) + " standard instrument departures");
        return std::move(sids);
    }

    auto SidDataValid(const nlohmann::json& sidData) -> bool
    {
        return sidData.is_array() &&
               std::find_if_not(sidData.cbegin(), sidData.cend(), [](const nlohmann::json& sid) -> bool {
                   return sid.is_object() && sid.contains("id") && sid.at("id").is_number_integer() &&
                          sid.contains("runway_id") && sid.at("runway_id").is_number_integer() &&
                          sid.contains("identifier") && sid.at("identifier").is_string() &&
                          sid.contains("initial_altitude") && sid.at("initial_altitude").is_number_integer() &&
                          sid.contains("initial_heading") && InitialHeadingValid(sid) && sid.contains("handoff") &&
                          (sid.at("handoff").is_number_integer() || sid.at("handoff").is_null()) &&
                          sid.contains("prenotes") && sid.at("prenotes").is_array() &&
                          std::find_if_not(
                              sid.at("prenotes").begin(),
                              sid.at("prenotes").end(),
                              [](const nlohmann::json& prenote) -> bool { return prenote.is_number_integer(); }) ==
                              sid.at("prenotes").cend();
               }) == sidData.cend();
    }

    auto InitialHeadingValid(const nlohmann::json& sidData) -> bool
    {
        if (sidData.at("initial_heading").is_null()) {
            return true;
        }

        if (!sidData.at("initial_heading").is_number_integer()) {
            return false;
        }

        int initialHeading = sidData.at("initial_heading").get<int>();
        return initialHeading > 0 && initialHeading < 361;
    }
} // namespace UKControllerPlugin::Sid
