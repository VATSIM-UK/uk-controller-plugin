#pragma once
#include "pch/stdafx.h"
#include "integration/IntegrationServer.h"
#include "integration/OutboundIntegrationMessageHandler.h"
#include "integration/InboundIntegrationMessageHandler.h"

namespace UKControllerPlugin::Integration {

    /*
     * Persists things relating to the integration
     * module so that we can manage their destruction
     * time more carefully
     */
    using IntegrationPersistenceContainer = struct
    {
        // Handles outbound messages
        std::shared_ptr<OutboundIntegrationMessageHandler> outboundMessageHandler;

        // Handles inbound messages
        std::shared_ptr<InboundIntegrationMessageHandler> inboundMessageHandler;

        // The server that listens for connections
        std::unique_ptr<IntegrationServer> server;
    };
} // namespace UKControllerPlugin::Integration
