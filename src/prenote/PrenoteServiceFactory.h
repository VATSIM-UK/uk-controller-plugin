#pragma once

namespace UKControllerPlugin {
    namespace Prenote {
        class PrenoteService;
        class PrenoteFactory;
    }  // namespace Prenote
    namespace Airfield {
        class AirfieldOwnershipManager;
    }  // namespace Airfield
    namespace Controller {
        class ActiveCallsignCollection;
    }  // namespace Controller
    namespace Message {
        class UserMessager;
    }  // namespace Message
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Prenote {

        /*
            Creates the Prenote Event Handler
        */
        class PrenoteServiceFactory
        {
            public:
                PrenoteServiceFactory(
                    const UKControllerPlugin::Prenote::PrenoteFactory & prenoteFactory,
                    UKControllerPlugin::Message::UserMessager & userMessager
                );

                std::unique_ptr<UKControllerPlugin::Prenote::PrenoteService> Create(
                    const UKControllerPlugin::Airfield::AirfieldOwnershipManager & airfieldOwnership,
                    const UKControllerPlugin::Controller::ActiveCallsignCollection & activeCallsigns,
                    const nlohmann::json & json
                );

            private:

                // Creates prenotes
                const UKControllerPlugin::Prenote::PrenoteFactory & prenoteFactory;

                // Deals with messages to the user
                UKControllerPlugin::Message::UserMessager & userMessager;
        };
    }  // namespace Prenote
}  // namespace UKControllerPlugin
