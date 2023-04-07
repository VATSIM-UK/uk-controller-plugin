#pragma once
#include "integration/IntegrationDataInitialiser.h"
#include "integration/MessageType.h"

namespace UKControllerPluginTest::Integration {

    class MockIntegrationDataInitialiser : public UKControllerPlugin::Integration::IntegrationDataInitialiser
    {
        public:
        MockIntegrationDataInitialiser();
        virtual ~MockIntegrationDataInitialiser();
        MOCK_METHOD(UKControllerPlugin::Integration::MessageType, InitialisesFor, (), (const, override));
        MOCK_METHOD(void, Initialise, (const UKControllerPlugin::Integration::IntegrationClient& client), (override));
    };

} // namespace UKControllerPluginTest::Integration
