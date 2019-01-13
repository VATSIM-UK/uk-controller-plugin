#pragma once
#include "controller/ControllerStatusEventHandlerInterface.h"
#include "controller/ActiveCallsignCollection.h"
#include "massevent/MassEvent.h"
#include "command/CommandHandlerInterface.h"

// Forward declarations
namespace UKControllerPlugin {
    namespace Controller {
        class ControllerPositionCollection;
        class ControllerPosition;
        class ControllerStatusEventHandlerInterface;
    }  // namespace Controller
    namespace Airfield {
        class AirfieldOwnershipManager;
    }  // namespace Airfield
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
    namespace Airfield {

        /*
            A class to handle the process of controllers coming and going and how this
            affects who owns which airfield.
        */
        class ControllerAirfieldOwnershipHandler
            : public UKControllerPlugin::Controller::ControllerStatusEventHandlerInterface,
            public UKControllerPlugin::Command::CommandHandlerInterface
        {
            public:
                ControllerAirfieldOwnershipHandler(
                    const UKControllerPlugin::Controller::ControllerPositionCollection & controllers,
                    UKControllerPlugin::Airfield::AirfieldOwnershipManager & airfieldOwnership,
                    UKControllerPlugin::Controller::ActiveCallsignCollection & activeCallsigns,
                    UKControllerPlugin::EventHandler::MassEvent massEventHandler,
                    UKControllerPlugin::Message::UserMessager & userMessager
                );
                void ControllerUpdateEvent(UKControllerPlugin::Euroscope::EuroScopeCControllerInterface & controller);
                void ControllerDisconnectEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCControllerInterface & controller
                );
                bool ProcessCommand(std::string command);
                void SelfDisconnectEvent(void);

            private:

                void SetupPosition(
                    UKControllerPlugin::Euroscope::EuroScopeCControllerInterface & callsign,
                    const UKControllerPlugin::Controller::ControllerPosition & matchedPos
                );
                void ProcessAffectedAirfields(
                    const UKControllerPlugin::Controller::ControllerPosition & controller
                );

                // All the controller positions
                const UKControllerPlugin::Controller::ControllerPositionCollection & controllers;

                // All the active callsigns
                UKControllerPlugin::Controller::ActiveCallsignCollection & activeCallsigns;

                // All the airfields
                UKControllerPlugin::Airfield::AirfieldOwnershipManager & airfieldOwnership;

                // Mass event handlers
                UKControllerPlugin::EventHandler::MassEvent massEventHandler;

                // For sending user messages
                UKControllerPlugin::Message::UserMessager & userMessager;
        };
    }  // namespace Airfield
}  // namespace UKControllerPlugin
