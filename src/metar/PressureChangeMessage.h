#pragma once
#include "message/MessageSerializableInterface.h"

namespace UKControllerPlugin {
    namespace Metar {

        /*
            A message to be used when the QNH
            changes at an airfield.
        */
        class PressureChangeMessage : public UKControllerPlugin::Message::MessageSerializableInterface
        {
            public:
                PressureChangeMessage(std::string station, std::string qnhBefore, std::string qnhAfter);

                // Inherited via MessageSerializableInterface
                std::string MessageHandler(void) const override;
                std::string MessageSender(void) const override;
                std::string MessageString(void) const override;
                bool MessageShowHandler(void) const override;
                bool MessageMarkUnread(void) const override;
                bool MessageOverrideBusy(void) const override;
                bool MessageFlashHandler(void) const override;
                bool MessageRequiresConfirm(void) const override;

            private:
                
                // The previous QNH stored
                const std::string qnhBefore;

                // The new QNH that has come in.
                const std::string qnhAfter;

                // The station with which the QNH is associated.
                const std::string station;
        };
    }  // namespace Metar
}  // namespace UKControllerPlugin
