#pragma once

// Forward declarations
namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCControllerInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Controller {

        /*
            An interface implemented by classes that can handle events pertaining to
            Controllers, such as log-ons and disconnects.
        */
        class ControllerStatusEventHandlerInterface
        {
            public:
                virtual ~ControllerStatusEventHandlerInterface(void) {}  // namespace Controller
                virtual void ControllerUpdateEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCControllerInterface & controller
                ) = 0;
                virtual void ControllerDisconnectEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCControllerInterface & controller
                ) = 0;
                virtual void SelfDisconnectEvent(void) = 0;
        };
    }  // namespace Controller
}  // namespace UKControllerPlugin
