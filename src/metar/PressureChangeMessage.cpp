#include "pch/stdafx.h"
#include "metar/PressureChangeMessage.h"

namespace UKControllerPlugin {
    namespace Metar {

        PressureChangeMessage::PressureChangeMessage(std::string station, std::string qnhBefore, std::string qnhAfter)
            : station(station), qnhBefore(qnhBefore), qnhAfter(qnhAfter)
        {

        }

        std::string PressureChangeMessage::MessageHandler(void) const
        {
            return "UKCP_QNH";
        }

        std::string PressureChangeMessage::MessageSender(void) const
        {
            return "UKCP";
        }

        std::string PressureChangeMessage::MessageString(void) const
        {
            return "New QNH at " + station + ", Was: " + qnhBefore + ", Now: " + qnhAfter;
        }

        bool PressureChangeMessage::MessageShowHandler(void) const
        {
            return true;
        }

        bool PressureChangeMessage::MessageMarkUnread(void) const
        {
            return true;
        }

        bool PressureChangeMessage::MessageOverrideBusy(void) const
        {
            return true;
        }

        bool PressureChangeMessage::MessageFlashHandler(void) const
        {
            return true;
        }

        bool PressureChangeMessage::MessageRequiresConfirm(void) const
        {
            return true;
        }
    }  // namespace Metar
}  // namespace UKControllerPlugin
