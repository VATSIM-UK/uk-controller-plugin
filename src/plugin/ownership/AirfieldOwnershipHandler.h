#pragma once
#include "controller/ActiveCallsignEventHandlerInterface.h"
#include "command/CommandHandlerInterface.h"

namespace UKControllerPlugin {
    namespace Controller {
        class ActiveCallsign;
        class ControllerPosition;
    } // namespace Controller
    namespace Ownership {
        class AirfieldOwnershipManager;
    } // namespace Ownership
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace Message {
        class UserMessager;
    } // namespace Message
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Ownership {

    /*
        A class to handle the process of controllers coming and going and how this
        affects who owns which airfield.
    */
    class AirfieldOwnershipHandler : public UKControllerPlugin::Controller::ActiveCallsignEventHandlerInterface,
                                     public UKControllerPlugin::Command::CommandHandlerInterface
    {
        public:
        AirfieldOwnershipHandler(
            UKControllerPlugin::Ownership::AirfieldOwnershipManager& airfieldOwnership,
            UKControllerPlugin::Message::UserMessager& userMessager);
        auto ProcessCommand(std::string command) -> bool override;

        // Inherited via ActiveCallsignEventHandlerInterface
        void ActiveCallsignAdded(const UKControllerPlugin::Controller::ActiveCallsign& callsign) override;
        void ActiveCallsignRemoved(const UKControllerPlugin::Controller::ActiveCallsign& callsign) override;
        void CallsignsFlushed() override;

        private:
        void ProcessAffectedAirfields(const UKControllerPlugin::Controller::ControllerPosition& controller);

        // All the airfields
        UKControllerPlugin::Ownership::AirfieldOwnershipManager& airfieldOwnership;

        // For sending user messages
        UKControllerPlugin::Message::UserMessager& userMessager;
    };
} // namespace UKControllerPlugin::Ownership
