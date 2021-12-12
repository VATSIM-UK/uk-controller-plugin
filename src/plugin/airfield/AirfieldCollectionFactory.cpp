#include "AirfieldCollection.h"
#include "AirfieldCollectionFactory.h"
#include "airfield/AirfieldModel.h"
#include "controller/ControllerPositionHierarchy.h"
#include "controller/ControllerPositionHierarchyFactory.h"
#include "prenote/PairedAirfieldPrenote.h"

using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Controller::ControllerPositionHierarchyFactory;
using UKControllerPlugin::Prenote::PairedAirfieldPrenote;

namespace UKControllerPlugin::Airfield {

    auto CreateAirfieldCollection(
        const nlohmann::json& airfields, const ControllerPositionHierarchyFactory& hierarchyFactory)
        -> std::unique_ptr<const UKControllerPlugin::Airfield::AirfieldCollection>
    {
        auto collection = std::make_unique<AirfieldCollection>();

        if (!airfields.is_array()) {
            LogError("Invalid airfield data");
            return collection;
        }

        for (const auto& airfield : airfields) {
            if (!AirfieldValid(airfield, hierarchyFactory)) {
                LogWarning("Invalid airfield detected: " + airfield.dump());
                continue;
            }

            std::vector<std::shared_ptr<PairedAirfieldPrenote>> pairedAirfieldPrenotes;
            for (const auto& pairingPrenote : airfield.at("pairing_prenotes")) {
                pairedAirfieldPrenotes.push_back(std::make_shared<Prenote::PairedAirfieldPrenote>(
                    pairingPrenote.at("airfield_id").get<int>(),
                    pairingPrenote.at("flight_rule_id").get<int>(),
                    pairingPrenote.at("prenote_id").get<int>()));
            }

            collection->AddAirfield(std::make_shared<AirfieldModel>(
                airfield.at("id").get<int>(),
                airfield.at("identifier").get<std::string>(),
                hierarchyFactory.CreateFromJsonById(airfield.at("top_down_controller_positions")),
                pairedAirfieldPrenotes,
                airfield.at("handoff_id").is_null() ? AirfieldModel::NO_HANDOFF_ID
                                                    : airfield.at("handoff_id").get<int>()));
        }

        LogInfo("Loaded " + std::to_string(collection->GetSize()) + " airfields");
        return collection;
    }

    auto AirfieldValid(const nlohmann::json& airfield, const ControllerPositionHierarchyFactory& hierarchyFactory)
        -> bool
    {
        return airfield.is_object() && airfield.contains("id") && airfield.at("id").is_number_integer() &&
               airfield.contains("identifier") && airfield.at("identifier").is_string() &&
               airfield.contains("top_down_controller_positions") &&
               airfield.at("top_down_controller_positions").is_array() &&
               hierarchyFactory.CreateFromJsonById(airfield.at("top_down_controller_positions")) != nullptr &&
               airfield.contains("pairing_prenotes") && airfield.at("pairing_prenotes").is_array() &&
               std::find_if_not(
                   airfield.at("pairing_prenotes").begin(),
                   airfield.at("pairing_prenotes").end(),
                   [](const nlohmann::json& prenote) -> bool {
                       return prenote.is_object() && prenote.contains("airfield_id") &&
                              prenote.at("airfield_id").is_number_integer() && prenote.contains("flight_rule_id") &&
                              prenote.at("flight_rule_id").is_number_integer() && prenote.contains("prenote_id") &&
                              prenote.at("prenote_id").is_number_integer();
                   }) == airfield.at("pairing_prenotes").cend() &&
               airfield.contains("handoff_id") &&
               (airfield.at("handoff_id").is_number_integer() || airfield.at("handoff_id").is_null());
    }
} // namespace UKControllerPlugin::Airfield
