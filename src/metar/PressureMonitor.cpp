#include "pch/stdafx.h"
#include "metar/PressureMonitor.h"
#include "metar/MetarParsingFunctions.h"
#include "metar/PressureChangeMessage.h"
#include "euroscope/GeneralSettingsEntries.h"

using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Euroscope::GeneralSettingsEntries;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Metar::PressureChangeMessage;

namespace UKControllerPlugin {
    namespace Metar {


        PressureMonitor::PressureMonitor(UserMessager & userMessager)
            : userMessager(userMessager)
        {

        }

        /*
            Return the QNH stored for a station.
        */
        std::string PressureMonitor::GetStoredQnh(std::string station) const
        {
            return this->qnhs.count(station) ? this->qnhs.at(station) : this->qnhNotStored;
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
        void PressureMonitor::NewMetar(std::string station, std::string metar)
        {
            std::string newQnh = GetQnhString(metar);

            // Couldn't find QNH in METAR
            if (newQnh == noQnh) {
                LogInfo("Unable to parse QNH from METAR for " + station + ": " + metar);
                return;
            }

            if (!this->qnhs.count(station) || this->qnhs.at(station) == newQnh) {
                this->qnhs[station] = newQnh;
                return;
            }

            if (this->notificationsEnabled) {
               // Send message
                PressureChangeMessage message(station, this->qnhs.at(station), newQnh);
                this->userMessager.SendMessageToUser(message);
            }

            this->qnhs[station] = newQnh;
        }

        /*
            User settings have been updated, update local variables from them.
        */
        void PressureMonitor::UserSettingsUpdated(UserSetting & userSettings)
        {
            this->notificationsEnabled = userSettings.GetBooleanEntry(
                GeneralSettingsEntries::pressureMonitorSendMessageKey,
                false
            );
        }
    }  // namespace Metar
}  // namespace UKControllerPlugin
