#pragma once
#include "message/MessageSerializableInterface.h"

namespace UKControllerPlugin::Airfield {
    class AirfieldModel;
} // namespace UKControllerPlugin::Airfield

namespace UKControllerPlugin::Ownership {

    /*
        A message returning which user owns a given airfield.
    */
    class AirfieldsOwnedQueryMessage : public UKControllerPlugin::Message::MessageSerializableInterface
    {
        public:
        AirfieldsOwnedQueryMessage(
            std::vector<std::shared_ptr<UKControllerPlugin::Airfield::AirfieldModel>> airfields,
            std::string userCallsign);
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
        const std::vector<std::shared_ptr<UKControllerPlugin::Airfield::AirfieldModel>> airfields;

        // The user callsign
        const std::string userCallsign;
    };
} // namespace UKControllerPlugin::Ownership
