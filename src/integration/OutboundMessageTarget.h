#pragma once

namespace UKControllerPlugin::Integration {

    struct MessageType;

    /*
     * Stores information about the target of an outbound message.
     */
    class OutboundMessageTarget
    {
        public:
            OutboundMessageTarget(
                std::wstring windowName,
                ULONG_PTR messageDataTypeIdentifier,
                std::set<MessageType> interestedMessages
            );

            const std::wstring& Identifier() const;
            const std::wstring& WindowName() const;
            const ULONG_PTR& MessageDataTypeIdentifier() const;
            const std::set<MessageType>& InterestedMessages() const;
            bool InterestedInMessage(const MessageType& message) const;

            bool operator==(const OutboundMessageTarget& compare) const;

        private:
            // The the name of the window that we're looking to send messages to
            const std::wstring windowName;

            // The data type identifier to attach to messages
            const ULONG_PTR messageDataTypeIdentifier;

            // The messages that this target is interested in receiving
            const std::set<MessageType> interestedMessages;
    };
} // namespace UKControllerPlugin::Integration
