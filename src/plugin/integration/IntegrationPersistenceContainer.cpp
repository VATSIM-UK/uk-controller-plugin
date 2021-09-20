#include "IntegrationPersistenceContainer.h"
#include "InboundIntegrationMessageHandler.h"
#include "IntegrationServer.h"
#include "OutboundIntegrationEventHandler.h"

namespace UKControllerPlugin::Integration {

    IntegrationPersistenceContainer::IntegrationPersistenceContainer(
        std::shared_ptr<OutboundIntegrationEventHandler> outboundMessageHandler,
        std::shared_ptr<InboundIntegrationMessageHandler> inboundMessageHandler,
        std::unique_ptr<IntegrationServer> server)
        : outboundMessageHandler(std::move(outboundMessageHandler)),
          inboundMessageHandler(std::move(inboundMessageHandler)), server(std::move(server))
    {
    }

    IntegrationPersistenceContainer::IntegrationPersistenceContainer(IntegrationPersistenceContainer&&) noexcept =
        default;

    auto IntegrationPersistenceContainer::operator=(IntegrationPersistenceContainer&&) noexcept
        -> IntegrationPersistenceContainer& = default;

    IntegrationPersistenceContainer::~IntegrationPersistenceContainer() = default;
} // namespace UKControllerPlugin::Integration
