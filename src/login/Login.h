#pragma once
#include "timedevent/AbstractTimedEvent.h"
#include "controller/ControllerStatusEventHandlerInterface.h"

// Forward declarations
namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    }  // namespace Euroscope
    namespace Controller {
        class ControllerStatusEventHandlerCollection;
    }  // namespace Controller
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Controller {

        /*
            Class for tracking the EuroScope login status.
        */
        class Login : public UKControllerPlugin::TimedEvent::AbstractTimedEvent,
            public UKControllerPlugin::Controller::ControllerStatusEventHandlerInterface
        {
            public:
                Login(
                    const UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin,
                    const UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection & controllerStatuses
                );
                // Inherited via ControllerStatusEventHandlerInterface
                void ControllerUpdateEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCControllerInterface & controller
                ) override;
                void ControllerDisconnectEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCControllerInterface & controller
                ) override;
                const int GetLoginStatus(void) const;
                std::chrono::system_clock::time_point GetLoginTime(void) const;
                std::chrono::seconds GetSecondsLoggedIn(void) const;
                void SelfDisconnectEvent(void) override;
                void SetLoginTime(std::chrono::system_clock::time_point time);
                void SetLoginStatus(int status);
                void TimedEventTrigger(void);


                // Constant to represent a proxied login - ie not the primary ES instance.
                const int proxied = -1;

                // Constant to represent a logged out state - no ES connection.
                const int loggedOut = 0;

                // Constant to represent a proper login - ie the primary ES instance.
                const int loggedIn = 1;

                // Default timestamp for when nobody has logged in.
                const std::chrono::system_clock::time_point defaultLoginTime =
                    (std::chrono::system_clock::time_point::min)();

            private:

                bool HasLoggedIn(const int previous, const int current);
                bool HasLoggedOut(const int previous, const int current);
                void StatusChange(const int previous, const int current);

                // Whether or not we're logged in, in some capacity
                int euroscopeStatus = EuroScopePlugIn::CONNECTION_TYPE_NO;

                // A timestamp for the time we logged in.
                std::chrono::system_clock::time_point loginTime = (std::chrono::system_clock::time_point::min)();

                // A link to the plugin that ES is connected to.
                const UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin;

                // A link to the controller status collection
                const UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection & controllerStatuses;
        };
    }  // namespace Controller
}  // namespace UKControllerPlugin
