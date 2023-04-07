#include "IntegrationPersistenceContainer.h"
#include "InboundIntegrationMessageHandler.h"
#include "IntegrationServer.h"
#include "OutboundIntegrationEventHandler.h"

namespace UKControllerPlugin::Integration {

    IntegrationPersistenceContainer::IntegrationPersistenceContainer(
        std::shared_ptr<OutboundIntegrationEventHandler> outboundMessageHandler,
        std::shared_ptr<InboundIntegrationMessageHandler> inboundMessageHandler,
        std::unique_ptr<IntegrationServer> server,
        std::shared_ptr<IntegrationDataInitialisers> dataInitialisers)
        : outboundMessageHandler(std::move(outboundMessageHandler)),
          inboundMessageHandler(std::move(inboundMessageHandler)), server(std::move(server)),
          dataInitialisers(std::move(dataInitialisers))
    {
    }

    IntegrationPersistenceContainer::IntegrationPersistenceContainer(IntegrationPersistenceContainer&&) noexcept =
        default;

    auto IntegrationPersistenceContainer::operator=(IntegrationPersistenceContainer&&) noexcept
        -> IntegrationPersistenceContainer& = default;

    IntegrationPersistenceContainer::~IntegrationPersistenceContainer() = default;
} // namespace UKControllerPlugin::Integration
