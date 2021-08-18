#include "pch/pch.h"

#include "controller/ControllerPositionHierarchy.h"
#include "controller/ControllerPositionHierarchyFactory.h"
#include "prenote/AbstractPrenote.h"
#include "prenote/AirfieldPairingPrenote.h"
#include "prenote/DeparturePrenote.h"
#include "prenote/PrenoteFactory.h"

using UKControllerPlugin::Controller::ControllerPositionHierarchyFactory;
using UKControllerPlugin::Prenote::AbstractPrenote;
using UKControllerPlugin::Prenote::AirfieldPairingPrenote;
using UKControllerPlugin::Prenote::DeparturePrenote;

namespace UKControllerPlugin::Prenote {

    PrenoteFactory::PrenoteFactory(const ControllerPositionHierarchyFactory& controllerFactory)
        : controllerFactory(controllerFactory)
    {
    }

    /*
        Creates and returns an airfield pairing prenote based on the
        JSON specification provided.
    */
    auto PrenoteFactory::CreateAirfieldPairingPrenote(const nlohmann::json& json) const
        -> std::unique_ptr<AirfieldPairingPrenote>
    {
        if (!json.at("origin").is_string()) {
            throw std::invalid_argument("Invalid pairing prenote origin");
        }

        if (!json.at("destination").is_string()) {
            throw std::invalid_argument("Invalid pairing prenote destination");
        }

        if (!json.contains("flight_rules") ||
            (!json.at("flight_rules").is_string() && !json.at("flight_rules").is_null())) {
            throw std::invalid_argument("Invalid pairing prenote flight rules");
        }

        return std::make_unique<AirfieldPairingPrenote>(
            this->controllerFactory.CreateFromJson(json.at("recipient")),
            json["origin"],
            json["destination"],
            json.at("flight_rules").is_string() ? json.at("flight_rules").get<std::string>() : "");
    }

    /*
        Creates and returns a departure (SID) prenote based on the JSON
        specification provided
    */
    auto PrenoteFactory::CreateDeparturePrenote(const nlohmann::json& json) const -> std::unique_ptr<DeparturePrenote>
    {
        if (!json.at("airfield").is_string()) {
            throw std::invalid_argument("Invalid prenote airfield");
        }

        if (!json.at("departure").is_string()) {
            throw std::invalid_argument("Invalid prenote departure");
        }

        return std::make_unique<DeparturePrenote>(
            this->controllerFactory.CreateFromJson(json.at("recipient")), json["airfield"], json["departure"]);
    }

    /*
        Creates prenote based on a JSON specification.
    */
    auto PrenoteFactory::CreateFromJson(const nlohmann::json& json) const -> std::unique_ptr<AbstractPrenote>
    {
        if (!json.is_object() || !json.at("type").is_string()) {
            LogError("Invalid prenote type: " + json.dump());
            throw std::invalid_argument("Invalid prenote type");
        }

        if (json["type"] == this->typeSid) {
            return this->CreateDeparturePrenote(json);
        }

        if (json["type"] == this->typePair) {
            return this->CreateAirfieldPairingPrenote(json);
        }

        LogError("Unknown prenote type " + json["type"].dump());
        throw std::invalid_argument("Invalid prenote type");
    }
} // namespace UKControllerPlugin::Prenote
