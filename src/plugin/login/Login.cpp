#include "login/Login.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "euroscope/EuroScopeCControllerInterface.h"

using UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection;
using UKControllerPlugin::Euroscope::EuroScopeCControllerInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::TimedEvent::AbstractTimedEvent;

namespace UKControllerPlugin {
    namespace Controller {

        Login::Login(
            const EuroscopePluginLoopbackInterface& plugin,
            const ControllerStatusEventHandlerCollection& controllerStatuses)
            : plugin(plugin), controllerStatuses(controllerStatuses)
        {
            this->loginTime = this->defaultLoginTime;
        }

        /*
            Returns the current login status, based on the ES connection type.
        */
        int Login::GetLoginStatus(void) const
        {
            int status;
            switch (this->euroscopeStatus) {
            case EuroScopePlugIn::CONNECTION_TYPE_NO:
            case EuroScopePlugIn::CONNECTION_TYPE_PLAYBACK:
                status = loggedOut;
                break;
            case EuroScopePlugIn::CONNECTION_TYPE_VIA_PROXY:
                status = this->proxied;
                break;
            default:
                status = this->loggedIn;
            }

            return status;
        }

        /*
            Returns the time at which we logged in.
        */
        std::chrono::system_clock::time_point Login::GetLoginTime(void) const
        {
            return this->loginTime;
        }

        /*
            Returns how many seconds the user has been logged in.
        */
        std::chrono::seconds Login::GetSecondsLoggedIn(void) const
        {
            if (this->loginTime == this->defaultLoginTime) {
                return std::chrono::seconds(0);
            }

            return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - this->loginTime);
        }

        /*
            Sets the current login status, based on EuroScopes connection type.
            Returns true if the status has changed, false otherwise.
        */
        void Login::TimedEventTrigger()
        {
            const int previousStatus = this->euroscopeStatus;

            // No change, nothing to do.
            this->euroscopeStatus = this->plugin.GetEuroscopeConnectionStatus();
            if (previousStatus == this->euroscopeStatus) {
                return;
            }

            this->StatusChange(previousStatus, this->euroscopeStatus);
        }

        /*
            Returns true if the user has just logged in.
        */
        bool Login::HasLoggedIn(const int previous, const int current)
        {
            return (previous == EuroScopePlugIn::CONNECTION_TYPE_NO ||
                    previous == EuroScopePlugIn::CONNECTION_TYPE_PLAYBACK ||
                    previous == EuroScopePlugIn::CONNECTION_TYPE_VIA_PROXY) &&
                   (current != EuroScopePlugIn::CONNECTION_TYPE_NO &&
                    current != EuroScopePlugIn::CONNECTION_TYPE_PLAYBACK &&
                    current != EuroScopePlugIn::CONNECTION_TYPE_VIA_PROXY);
        }

        /*
            Returns true if the user has just logged out.
        */
        bool Login::HasLoggedOut(const int previous, const int current)
        {
            return (previous != EuroScopePlugIn::CONNECTION_TYPE_NO &&
                    previous != EuroScopePlugIn::CONNECTION_TYPE_PLAYBACK &&
                    previous != EuroScopePlugIn::CONNECTION_TYPE_VIA_PROXY) &&
                   (current == EuroScopePlugIn::CONNECTION_TYPE_NO ||
                    current == EuroScopePlugIn::CONNECTION_TYPE_PLAYBACK ||
                    current == EuroScopePlugIn::CONNECTION_TYPE_VIA_PROXY);
        }

        /*
            Handles a change in login status.
        */
        void Login::StatusChange(const int previous, const int current)
        {
            // If we've logged in, set the login time.
            if (this->HasLoggedIn(previous, current)) {
                LogInfo("User has connected to VATSIM");
                this->loginTime = std::chrono::system_clock::now();
            } else if (this->HasLoggedOut(previous, current)) {
                LogInfo("User has disconnected from VATSIM");
                this->loginTime = this->defaultLoginTime;
                this->controllerStatuses.SelfDisconnectEvent();
            }
        }

        /*
            If we get a controller update and it's from us, we may have logged into the
            network, so use this to trigger a status update if we need to.
        */
        void Login::ControllerUpdateEvent(EuroScopeCControllerInterface& controller)
        {
            if (!controller.IsCurrentUser()) {
                return;
            }

            this->TimedEventTrigger();
        }

        /*
            EuroScope never triggers this for the user, so nothing to do.
        */
        void Login::ControllerDisconnectEvent(EuroScopeCControllerInterface& controller)
        {
        }

        /*
            We trigger this event, so nout to do here.
        */
        void Login::SelfDisconnectEvent(void)
        {
        }

        /*
            Force set the login time, this mainly for testing.
        */
        void Login::SetLoginTime(std::chrono::system_clock::time_point time)
        {
            this->loginTime = time;
        }

        /*
            Force the login status, this is mainly for testing.
        */
        void Login::SetLoginStatus(int status)
        {
            this->euroscopeStatus = status;
        }

        auto Login::GetDefaultLoginTime() const -> std::chrono::system_clock::time_point
        {
            return this->defaultLoginTime;
        }
    } // namespace Controller
} // namespace UKControllerPlugin
