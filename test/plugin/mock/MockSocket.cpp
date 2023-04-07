#include "MockSocket.h"

using UKControllerPlugin::Integration::SocketInterface;

namespace UKControllerPluginTest::Integration {
    MockSocket::MockSocket() = default;
    MockSocket::~MockSocket() = default;

    SocketInterface& MockSocket::operator<<(std::string& message)
    {
        this->InsertStringOverride(message);
        return *this;
    }

    SocketInterface& MockSocket::operator>>(std::string& message)
    {
        this->ExtractStringOverride();
        return *this;
    }

    SocketInterface& MockSocket::operator>>(std::stringstream& inboundStream)
    {
        inboundStream << this->ExtractStreamOverride();
        return *this;
    }
} // namespace UKControllerPluginTest::Integration
