#include "MessageSerializableInterface.h"

namespace UKControllerPlugin::Message {
    MessageSerializableInterface::MessageSerializableInterface() = default;
    MessageSerializableInterface::~MessageSerializableInterface() = default;
    MessageSerializableInterface::MessageSerializableInterface(const MessageSerializableInterface&) = default;
    auto MessageSerializableInterface::operator=(const MessageSerializableInterface&)
        -> MessageSerializableInterface& = default;
} // namespace UKControllerPlugin::Message
