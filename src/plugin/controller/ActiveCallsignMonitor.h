#pragma once
#include "command/CommandHandlerInterface.h"
#include "controller/ControllerStatusEventHandlerInterface.h"

namespace UKControllerPlugin {
    namespace Controller {
        class ActiveCallsignCollection;
        class ControllerPositionCollection;
        class ControllerPosition;
        class ControllerStatusEventHandlerInterface;
    } // namespace Controller
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Controller {

    /*
        A class to handle the process of controllers coming and going and how this
        affects who owns which airfield.
    */
    class ActiveCallsignMonitor : public UKControllerPlugin::Controller::ControllerStatusEventHandlerInterface
    {
        public:
        ActiveCallsignMonitor(
            const UKControllerPlugin::Controller::ControllerPositionCollection& controllers,
            UKControllerPlugin::Controller::ActiveCallsignCollection& activeCallsigns);
        void ControllerUpdateEvent(UKControllerPlugin::Euroscope::EuroScopeCControllerInterface& controller) override;
        void
        ControllerDisconnectEvent(UKControllerPlugin::Euroscope::EuroScopeCControllerInterface& controller) override;
        void SelfDisconnectEvent() override;

        private:
        void SetupPosition(
            UKControllerPlugin::Euroscope::EuroScopeCControllerInterface& callsign,
            const UKControllerPlugin::Controller::ControllerPosition& matchedPos);

        // All the controller positions
        const UKControllerPlugin::Controller::ControllerPositionCollection& controllers;

        // All the active callsigns
        UKControllerPlugin::Controller::ActiveCallsignCollection& activeCallsigns;

        inline static const double FREQUENCY_MATCH_DELTA = 0.001;
    };
} // namespace UKControllerPlugin::Controller
