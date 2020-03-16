#pragma once
#include "controller/ActiveCallsignEventHandlerInterface.h"
#include "controller/ActiveCallsignCollection.h"
#include "command/CommandHandlerInterface.h"

// Forward declarations
namespace UKControllerPlugin {
    namespace Ownership {
        class AirfieldOwnershipManager;
    }  // namespace Ownership
    namespace Flightplan {
        class FlightplanCollection;
    }  // namespace Flightplan
    namespace Message {
        class UserMessager;
    }  // namespace Message
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Ownership {

        /*
            A class to handle the process of controllers coming and going and how this
            affects who owns which airfield.
        */
        class AirfieldOwnershipHandler
            : public UKControllerPlugin::Controller::ActiveCallsignEventHandlerInterface,
            public UKControllerPlugin::Command::CommandHandlerInterface
        {
            public:
                AirfieldOwnershipHandler(
                    UKControllerPlugin::Ownership::AirfieldOwnershipManager & airfieldOwnership,
                    UKControllerPlugin::Message::UserMessager & userMessager
                );
                bool ProcessCommand(std::string command);

                // Inherited via ActiveCallsignEventHandlerInterface
                void ActiveCallsignAdded(
                    const UKControllerPlugin::Controller::ActiveCallsign& callsign,
                    bool userCallsign
                ) override;
                void ActiveCallsignRemoved(
                    const UKControllerPlugin::Controller::ActiveCallsign& callsign,
                    bool userCallsign
                )override;
                void CallsignsFlushed(void) override;

            private:

                void ProcessAffectedAirfields(
                    const UKControllerPlugin::Controller::ControllerPosition & controller
                );

                // All the airfields
                UKControllerPlugin::Ownership::AirfieldOwnershipManager & airfieldOwnership;

                // For sending user messages
                UKControllerPlugin::Message::UserMessager & userMessager;
        };
    }  // namespace Ownership
}  // namespace UKControllerPlugin
