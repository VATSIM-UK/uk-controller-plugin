#pragma once

namespace UKControllerPlugin {
    namespace Prenote {
        class AbstractPrenote;
        class DeparturePrenote;
        class AirfieldPairingPrenote;
    }  // namespace Prenote
    namespace Controller {
        class ControllerPositionHierarchyFactory;
    }  // namespace Controller
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Prenote {

        /*
            Class responsible for creating a prenote object from json.
        */
        class PrenoteFactory
        {
            public:
                PrenoteFactory(
                    const UKControllerPlugin::Controller::ControllerPositionHierarchyFactory & controllerFactory
                );

                std::unique_ptr<UKControllerPlugin::Prenote::AbstractPrenote> CreateFromJson(
                    const nlohmann::json & json
                ) const;

                // The type field of a SID prenote
                const std::string typeSid = "sid";

                // The type field of an airfield pairing prenote
                const std::string typePair = "airfieldPairing";

            private:

                std::unique_ptr<UKControllerPlugin::Prenote::AirfieldPairingPrenote> CreateAirfieldPairingPrenote(
                    const nlohmann::json & json
                ) const;

                std::unique_ptr<UKControllerPlugin::Prenote::DeparturePrenote> CreateDeparturePrenote(
                    const nlohmann::json & json
                ) const;

                // Can create a hierarchy of controller positions
                const UKControllerPlugin::Controller::ControllerPositionHierarchyFactory & controllerFactory;
        };
    }  // namespace Prenote
}  // namespace UKControllerPlugin
