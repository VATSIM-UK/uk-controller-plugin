#pragma once

namespace UKControllerPlugin::Integration {

    /*
     * Wrapper around windows SOCKET implementation
     * to allow us to mock it.
     */
    class SocketInterface
    {
        public:
            virtual ~SocketInterface() = default;
            virtual SocketInterface& operator>>(std::string& message) = 0;
            virtual SocketInterface& operator>>(std::stringstream& inboundStream) = 0;
            virtual SocketInterface& operator<<(std::string& message) = 0;
            virtual bool Active() const = 0;
    };
} // namespace UKControllerPlugin::Integration
