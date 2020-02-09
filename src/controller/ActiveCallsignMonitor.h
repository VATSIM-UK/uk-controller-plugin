#pragma once
#include "controller/ControllerStatusEventHandlerInterface.h"
#include "controller/ActiveCallsignCollection.h"
#include "command/CommandHandlerInterface.h"

// Forward declarations
namespace UKControllerPlugin {
    namespace Controller {
        class ControllerPositionCollection;
        class ControllerPosition;
        class ControllerStatusEventHandlerInterface;
    }  // namespace Controller
    namespace Flightplan {
        class FlightplanCollection;
    }  // namespace Flightplan
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Controller {

        /*
            A class to handle the process of controllers coming and going and how this
            affects who owns which airfield.
        */
        class ActiveCallsignMonitor
            : public UKControllerPlugin::Controller::ControllerStatusEventHandlerInterface
        {
            public:
                ActiveCallsignMonitor(
                    const UKControllerPlugin::Controller::ControllerPositionCollection & controllers,
                    UKControllerPlugin::Controller::ActiveCallsignCollection & activeCallsigns
                );
                void ControllerUpdateEvent(UKControllerPlugin::Euroscope::EuroScopeCControllerInterface & controller);
                void ControllerDisconnectEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCControllerInterface & controller
                );
                void SelfDisconnectEvent(void);

            private:

                void SetupPosition(
                    UKControllerPlugin::Euroscope::EuroScopeCControllerInterface & callsign,
                    const UKControllerPlugin::Controller::ControllerPosition & matchedPos
                );

                // All the controller positions
                const UKControllerPlugin::Controller::ControllerPositionCollection & controllers;

                // All the active callsigns
                UKControllerPlugin::Controller::ActiveCallsignCollection & activeCallsigns;
        };
    }  // namespace Controller
}  // namespace UKControllerPlugin
