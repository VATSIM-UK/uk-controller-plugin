#pragma once
#include "message/MessageSerializableInterface.h"

namespace UKControllerPlugin {
    namespace Bootstrap {

        /*
            Represents a warning as a result of something going wrong in bootstrap
        */
        class BootstrapWarningMessage : public UKControllerPlugin::Message::MessageSerializableInterface
        {
            public:
            explicit BootstrapWarningMessage(const std::string message);

            // Handler to handle this message
            static const std::string handler;

            // Who this message comes from
            static const std::string sender;

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
            const std::string message;
        };
    } // namespace Bootstrap
} // namespace UKControllerPlugin
