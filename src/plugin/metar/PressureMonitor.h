#pragma once
#include "MetarEventHandlerInterface.h"
#include "controller/ActiveCallsignCollection.h"
#include "euroscope/UserSetting.h"
#include "euroscope/UserSettingAwareInterface.h"
#include "message/UserMessager.h"

namespace UKControllerPlugin::Metar {
    class PressureComponent;

    /*
        Monitors changing QNHs as reported by EuroScope
        and notifies the user if the QNH has changed.
    */
    class PressureMonitor : public UKControllerPlugin::Metar::MetarEventHandlerInterface,
                            public UKControllerPlugin::Euroscope::UserSettingAwareInterface
    {
        public:
        PressureMonitor(
            UKControllerPlugin::Message::UserMessager& userMessager,
            const UKControllerPlugin::Controller::ActiveCallsignCollection& activeCallsigns);
        [[nodiscard]] auto GetStoredPressure(const std::string& airfield) const -> std::shared_ptr<PressureComponent>;
        bool NotificationsEnabled(void) const;
        void SetNotficationsEnabled(bool enabled);

        // Inherited via MetarEventHandlerInterface
        void MetarUpdated(const ParsedMetar& metar) override;

        // Inherited via UserSettingAwareInterface
        void UserSettingsUpdated(UKControllerPlugin::Euroscope::UserSetting& userSettings) override;

        private:
        [[nodiscard]] auto PressureHasUpdated(const std::string& airfield, const PressureComponent& pressure) const
            -> bool;
        void SendNotificationIfRequired(const std::string& airfield, const PressureComponent& pressure) const;

        // Whether or not to send notifications.
        bool notificationsEnabled = false;

        // A map of airfield -> last recorded QNH.
        std::map<std::string, std::shared_ptr<PressureComponent>> pressures;

        // Interface with ES for sending messages to the user about pressure changes.
        UKControllerPlugin::Message::UserMessager& userMessager;

        // All the active controller callsigns
        const UKControllerPlugin::Controller::ActiveCallsignCollection& activeCallsigns;
    };
} // namespace UKControllerPlugin::Metar
