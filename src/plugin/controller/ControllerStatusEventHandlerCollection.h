#pragma once

// Forward
namespace UKControllerPlugin {
    namespace Controller {
        class ControllerStatusEventHandlerInterface;
    } // namespace Controller
    namespace Euroscope {
        class EuroScopeCControllerInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Controller {

        /*
            A class that stores a collection of ControllerStatusEventHandlers
            and will call the appropriate handler when an event is triggered.
        */
        class ControllerStatusEventHandlerCollection
        {
            public:
            void ControllerUpdateEvent(UKControllerPlugin::Euroscope::EuroScopeCControllerInterface& controller) const;
            void
            ControllerDisconnectEvent(UKControllerPlugin::Euroscope::EuroScopeCControllerInterface& controller) const;
            int CountHandlers(void) const;
            void RegisterHandler(
                std::shared_ptr<UKControllerPlugin::Controller::ControllerStatusEventHandlerInterface> handler);
            void SelfDisconnectEvent(void) const;

            private:
            std::vector<std::shared_ptr<UKControllerPlugin::Controller::ControllerStatusEventHandlerInterface>>
                eventHandlers;
        };
    }; // namespace Controller
};     // namespace UKControllerPlugin
