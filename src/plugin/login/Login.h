#pragma once
#include "controller/ControllerStatusEventHandlerInterface.h"
#include "timedevent/AbstractTimedEvent.h"

// Forward declarations
namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace Controller {
        class ControllerStatusEventHandlerCollection;
    } // namespace Controller
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Controller {

    /*
        Class for tracking the EuroScope login status.
    */
    class Login : public UKControllerPlugin::TimedEvent::AbstractTimedEvent,
                  public UKControllerPlugin::Controller::ControllerStatusEventHandlerInterface
    {
        public:
        Login(
            const UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface& plugin,
            const UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection& controllerStatuses);
        // Inherited via ControllerStatusEventHandlerInterface
        void ControllerUpdateEvent(UKControllerPlugin::Euroscope::EuroScopeCControllerInterface& controller) override;
        void
        ControllerDisconnectEvent(UKControllerPlugin::Euroscope::EuroScopeCControllerInterface& controller) override;
        [[nodiscard]] auto GetLoginStatus() const -> int;
        [[nodiscard]] auto GetLoginTime() const -> std::chrono::system_clock::time_point;
        [[nodiscard]] auto GetDefaultLoginTime() const -> std::chrono::system_clock::time_point;
        [[nodiscard]] auto GetSecondsLoggedIn() const -> std::chrono::seconds;
        void SelfDisconnectEvent() override;
        void SetLoginTime(std::chrono::system_clock::time_point time);
        void SetLoginStatus(int status);
        void TimedEventTrigger() override;

        // Constant to represent a proxied login - ie not the primary ES instance.
        inline static const int proxied = -1;

        // Constant to represent a logged out state - no ES connection.
        inline static const int loggedOut = 0;

        // Constant to represent a proper login - ie the primary ES instance.
        inline static const int loggedIn = 1;

        private:
        auto HasLoggedIn(int previous, int current) -> bool;
        auto HasLoggedOut(int previous, int current) -> bool;
        void StatusChange(int previous, int current);

        // Whether or not we're logged in, in some capacity
        int euroscopeStatus = EuroScopePlugIn::CONNECTION_TYPE_NO;

        // Default timestamp for when nobody has logged in.
        const std::chrono::system_clock::time_point defaultLoginTime = (std::chrono::system_clock::time_point::min)();

        // A timestamp for the time we logged in.
        std::chrono::system_clock::time_point loginTime = (std::chrono::system_clock::time_point::min)();

        // A link to the plugin that ES is connected to.
        const UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // A link to the controller status collection
        const UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection& controllerStatuses;
    };
} // namespace UKControllerPlugin::Controller
