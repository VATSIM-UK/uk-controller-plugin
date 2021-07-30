#include "pch/stdafx.h"
#include "prenote/PrenoteFactory.h"
#include "prenote/AbstractPrenote.h"
#include "prenote/DeparturePrenote.h"
#include "controller/ControllerPositionHierarchyFactory.h"
#include "controller/ControllerPositionHierarchy.h"
#include "prenote/AirfieldPairingPrenote.h"

using UKControllerPlugin::Prenote::AbstractPrenote;
using UKControllerPlugin::Prenote::AirfieldPairingPrenote;
using UKControllerPlugin::Prenote::DeparturePrenote;
using UKControllerPlugin::Controller::ControllerPositionHierarchyFactory;

namespace UKControllerPlugin {
    namespace Prenote {

        const std::string noFlightRules = "";

        PrenoteFactory::PrenoteFactory(
            const ControllerPositionHierarchyFactory & controllerFactory
        )
            : controllerFactory(controllerFactory)
        {
        }

        /*
            Creates and returns an airfield pairing prenote based on the
            JSON specification provided.
        */
        std::unique_ptr<AirfieldPairingPrenote> PrenoteFactory::CreateAirfieldPairingPrenote(
            const nlohmann::json & json
        ) const {
            if (!json.at("origin").is_string()) {
                throw std::invalid_argument("Invalid pairing prenote origin");
            }

            if (!json.at("destination").is_string()) {
                throw std::invalid_argument("Invalid pairing prenote destination");
            }

            if (
                !json.contains("flight_rules") ||
                (!json.at("flight_rules").is_string() && !json.at("flight_rules").is_null())
            ) {
                throw std::invalid_argument("Invalid pairing prenote flight rules");
            }

            return std::make_unique<AirfieldPairingPrenote>(
                this->controllerFactory.CreateFromJson(json.at("recipient")),
                json["origin"],
                json["destination"],
                json.at("flight_rules").is_string() ? json["flight_rules"] : noFlightRules
            );
        }

        /*
            Creates and returns a departure (SID) prenote based on the JSON
            specification provided
        */
        std::unique_ptr<DeparturePrenote> PrenoteFactory::CreateDeparturePrenote(const nlohmann::json & json) const
        {
            if (!json.at("airfield").is_string()) {
                throw std::invalid_argument("Invalid prenote airfield");
            }

            if (!json.at("departure").is_string()) {
                throw std::invalid_argument("Invalid prenote departure");
            }

            return std::make_unique<DeparturePrenote>(
                this->controllerFactory.CreateFromJson(json.at("recipient")),
                json["airfield"],
                json["departure"]
            );
        }

        /*
            Creates prenote based on a JSON specification.
        */
        std::unique_ptr<AbstractPrenote> PrenoteFactory::CreateFromJson(const nlohmann::json & json) const
        {
            if (!json.is_object() || !json.at("type").is_string()) {
                LogError("Invalid prenote type: " + json.dump());
                throw std::invalid_argument("Invalid prenote type");
            }

            if (json["type"] == this->typeSid) {
                return this->CreateDeparturePrenote(json);
            } else if (json["type"] == this->typePair) {
                return this->CreateAirfieldPairingPrenote(json);
            }

            LogError("Unknown prenote type " + json["type"].dump());
            throw std::invalid_argument("Invalid prenote type");
        }
    }  // namespace Prenote
}  // namespace UKControllerPlugin
