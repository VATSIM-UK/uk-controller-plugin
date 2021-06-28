#pragma once

namespace UKControllerPlugin::Integration {

    /*
     * Stores information about the target of an outbound message.
     */
    class OutboundMessageTarget
    {
        public:
            OutboundMessageTarget(
                std::wstring windowName,
                ULONG_PTR messageDataTypeIdentifier,
                std::set<std::string> interestedMessages
            );

            const std::wstring& Identifier() const;
            const std::wstring& WindowName() const;
            const ULONG_PTR& MessageDataTypeIdentifier() const;
            const std::set<std::string> InterestedMessages() const;
            bool InterestedInMessage(std::string message) const;

        private:
            // The the name of the window that we're looking to send messages to
            const std::wstring windowName;

            // The data type identifier to attach to messages
            const ULONG_PTR messageDataTypeIdentifier;

            // The messages that this target is interested in receiving
            const std::set<std::string> interestedMessages;
    };
} // namespace UKControllerPlugin::Integration
