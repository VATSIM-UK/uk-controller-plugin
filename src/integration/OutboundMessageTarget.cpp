#include "pch/stdafx.h"
#include "integration/OutboundMessageTarget.h"

namespace UKControllerPlugin::Integration {

    OutboundMessageTarget::OutboundMessageTarget(
        std::wstring windowName,
        ULONG_PTR messageDataTypeIdentifier,
        std::set<std::string> interestedMessages
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

    const std::set<std::string> OutboundMessageTarget::InterestedMessages() const
    {
        return this->interestedMessages;
    }

    bool OutboundMessageTarget::InterestedInMessage(std::string message) const
    {
        return this->interestedMessages.find(message) != this->interestedMessages.cend();
    }
} // namespace UKControllerPlugin::Integration
