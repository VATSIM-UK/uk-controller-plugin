#pragma once
#include "pch/pch.h"
#include "integration/SocketInterface.h"

namespace UKControllerPluginTest::Integration {
    class MockSocket : public UKControllerPlugin::Integration::SocketInterface
    {
        public:
            MOCK_METHOD(bool, Active, (), (const, override));
            MOCK_METHOD(void, ExtractStringOverride, ());
            MOCK_METHOD(std::string, ExtractStreamOverride, ());
            MOCK_METHOD(std::string, InsertStringOverride, (std::string));

            ~MockSocket() override = default;

            SocketInterface& operator<<(std::string& message) override
            {
                this->InsertStringOverride(message);
                return *this;
            }

            SocketInterface& operator>>(std::string& message) override
            {
                this->ExtractStringOverride();
                return *this;
            }

            SocketInterface& operator>>(std::stringstream& inboundStream) override
            {
                inboundStream << this->ExtractStreamOverride();
                return *this;
            }
    };
} // namespace UKControllerPluginTest::Integration
