#include "bootstrap/BootstrapWarningMessage.h"

namespace UKControllerPlugin {
    namespace Bootstrap {

        // Set the static properties
        const std::string BootstrapWarningMessage::handler = "UKCP_Startup";
        const std::string BootstrapWarningMessage::sender = "UKCP";

        BootstrapWarningMessage::BootstrapWarningMessage(const std::string message) : message(message)
        {
        }

        std::string BootstrapWarningMessage::MessageHandler(void) const
        {
            return this->handler;
        }

        std::string BootstrapWarningMessage::MessageSender(void) const
        {
            return this->sender;
        }

        std::string BootstrapWarningMessage::MessageString(void) const
        {
            return this->message;
        }

        bool BootstrapWarningMessage::MessageShowHandler(void) const
        {
            return true;
        }

        bool BootstrapWarningMessage::MessageMarkUnread(void) const
        {
            return true;
        }

        bool BootstrapWarningMessage::MessageOverrideBusy(void) const
        {
            return true;
        }

        bool BootstrapWarningMessage::MessageFlashHandler(void) const
        {
            return true;
        }

        bool BootstrapWarningMessage::MessageRequiresConfirm(void) const
        {
            return true;
        }
    } // namespace Bootstrap
} // namespace UKControllerPlugin
