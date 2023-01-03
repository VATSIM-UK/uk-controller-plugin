#pragma once
#include "integration/SocketInterface.h"

namespace UKControllerPluginTest::Integration {
    class MockSocket : public UKControllerPlugin::Integration::SocketInterface
    {
        public:
        MockSocket();
        virtual ~MockSocket();
        MOCK_METHOD(bool, Active, (), (const, override));
        MOCK_METHOD(void, ExtractStringOverride, ());
        MOCK_METHOD(std::string, ExtractStreamOverride, ());
        MOCK_METHOD(std::string, InsertStringOverride, (std::string));
        SocketInterface& operator<<(std::string& message) override;
        SocketInterface& operator>>(std::string& message) override;
        SocketInterface& operator>>(std::stringstream& inboundStream) override;
    };
} // namespace UKControllerPluginTest::Integration
