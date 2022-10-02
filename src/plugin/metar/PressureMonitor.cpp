#include "MetarComponents.h"
#include "ParsedMetar.h"
#include "PressureChangeMessage.h"
#include "PressureComponent.h"
#include "PressureMonitor.h"
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"
#include "euroscope/GeneralSettingsEntries.h"

using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Euroscope::GeneralSettingsEntries;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Metar::PressureChangeMessage;

namespace UKControllerPlugin::Metar {

    PressureMonitor::PressureMonitor(UserMessager& userMessager, const ActiveCallsignCollection& activeCallsigns)
        : userMessager(userMessager), activeCallsigns(activeCallsigns)
    {
    }

    /*
        Return the QNH stored for a station.
    */
    auto PressureMonitor::GetStoredPressure(const std::string& airfield) const -> std::shared_ptr<PressureComponent>
    {
        return this->pressures.count(airfield) != 0 ? this->pressures.at(airfield) : nullptr;
    }

    /*
        Are notifications enabled
    */
    bool PressureMonitor::NotificationsEnabled(void) const
    {
        return this->notificationsEnabled;
    }

    /*
        Turn notifications on or off
    */
    void PressureMonitor::SetNotficationsEnabled(bool enabled)
    {
        this->notificationsEnabled = enabled;
    }

    /*
        Update the stored QNH with that of a new METAR. Notify the user if the
        QNH has changed.
    */
    void PressureMonitor::MetarUpdated(const ParsedMetar& metar)
    {
        if (metar.Components().pressure == nullptr) {
            return;
        }

        const auto pressureComponent = metar.Components().pressure;
        const auto airfield = metar.Airfield();
        if (!this->PressureHasUpdated(airfield, *pressureComponent)) {
            this->pressures[airfield] = pressureComponent;
            return;
        }

        this->SendNotificationIfRequired(airfield, *pressureComponent);
        this->pressures[airfield] = pressureComponent;
    }

    /*
        User settings have been updated, update local variables from them.
    */
    void PressureMonitor::UserSettingsUpdated(UserSetting& userSettings)
    {
        this->notificationsEnabled =
            userSettings.GetBooleanEntry(GeneralSettingsEntries::pressureMonitorSendMessageKey, false);
    }

    void
    PressureMonitor::SendNotificationIfRequired(const std::string& airfield, const PressureComponent& pressure) const
    {
        if (!this->notificationsEnabled || !this->activeCallsigns.UserHasCallsign()) {
            return;
        }

        if (!this->activeCallsigns.GetUserCallsign().GetNormalisedPosition().HasTopdownAirfield(airfield)) {
            return;
        }

        this->userMessager.SendMessageToUser(PressureChangeMessage(airfield, *this->pressures.at(airfield), pressure));
    }

    auto PressureMonitor::PressureHasUpdated(const std::string& airfield, const PressureComponent& pressure) const
        -> bool
    {
        return this->pressures.count(airfield) != 0 &&
               this->pressures.at(airfield)->QnhHectopascals() != pressure.QnhHectopascals();
    }
} // namespace UKControllerPlugin::Metar
