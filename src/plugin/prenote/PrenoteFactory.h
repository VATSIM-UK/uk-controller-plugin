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

namespace UKControllerPlugin::Prenote {

        /*
            Class responsible for creating a prenote object from json.
        */
        class PrenoteFactory
        {
            public:
                explicit PrenoteFactory(
                    const UKControllerPlugin::Controller::ControllerPositionHierarchyFactory & controllerFactory
                );

                [[nodiscard]] auto CreateFromJson(
                    const nlohmann::json & json
                ) const -> std::unique_ptr<UKControllerPlugin::Prenote::AbstractPrenote>;

            private:

                [[nodiscard]] auto CreateAirfieldPairingPrenote(
                    const nlohmann::json & json
                ) const -> std::unique_ptr<UKControllerPlugin::Prenote::AirfieldPairingPrenote>;

                [[nodiscard]] auto CreateDeparturePrenote(
                    const nlohmann::json & json
                ) const -> std::unique_ptr<UKControllerPlugin::Prenote::DeparturePrenote>;

                // Can create a hierarchy of controller positions
                const UKControllerPlugin::Controller::ControllerPositionHierarchyFactory & controllerFactory;
                
                // The type field of a SID prenote
                const std::string typeSid = "sid";

                // The type field of an airfield pairing prenote
                const std::string typePair = "airfieldPairing";
        };
    }  // namespace UKControllerPlugin::Prenote
