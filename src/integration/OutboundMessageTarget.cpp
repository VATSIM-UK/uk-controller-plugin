#include "pch/stdafx.h"
#include "integration/OutboundMessageTarget.h"
#include "integration/MessageType.h"

namespace UKControllerPlugin::Integration {

    OutboundMessageTarget::OutboundMessageTarget(
        std::wstring windowName,
        ULONG_PTR messageDataTypeIdentifier,
        std::set<MessageType> interestedMessages
    ): windowName(std::move(windowName)), messageDataTypeIdentifier(messageDataTypeIdentifier),
       interestedMessages(std::move(interestedMessages))
    {}

    const std::wstring& OutboundMessageTarget::Identifier() const
    {
        return this->WindowName();
    }

    const std::wstring& OutboundMessageTarget::WindowName() const
    {
        return this->windowName;
    }

    const ULONG_PTR& OutboundMessageTarget::MessageDataTypeIdentifier() const
    {
        return this->messageDataTypeIdentifier;
    }

    const std::set<MessageType>& OutboundMessageTarget::InterestedMessages() const
    {
        return this->interestedMessages;
    }

    bool OutboundMessageTarget::InterestedInMessage(const MessageType& message) const
    {
        return this->interestedMessages.find(message) != this->interestedMessages.cend();
    }

    bool OutboundMessageTarget::operator==(const OutboundMessageTarget& compare) const
    {
        return this->Identifier() == compare.Identifier();
    }
} // namespace UKControllerPlugin::Integration
