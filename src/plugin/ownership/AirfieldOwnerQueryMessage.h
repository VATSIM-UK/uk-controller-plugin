#pragma once
#include "message/MessageSerializableInterface.h"

namespace UKControllerPlugin {
    namespace Ownership {

        /*
            A message returning which user owns a given airfield.
        */
        class AirfieldOwnerQueryMessage : public UKControllerPlugin::Message::MessageSerializableInterface
        {
            public:
                AirfieldOwnerQueryMessage(std::string airfield, std::string callsign, std::string name);
                std::string MessageHandler(void) const override;
                std::string MessageSender(void) const override;
                std::string MessageString(void) const override;
                bool MessageShowHandler(void) const override;
                bool MessageMarkUnread(void) const override;
                bool MessageOverrideBusy(void) const override;
                bool MessageFlashHandler(void) const override;
                bool MessageRequiresConfirm(void) const override;

            private:
                // The airfield involved
                const std::string airfield;

                // The controller callsign
                const std::string callsign;

                // The controllers name
                const std::string name;
        };
    }  // namespace Ownership
}  // namespace UKControllerPlugin
