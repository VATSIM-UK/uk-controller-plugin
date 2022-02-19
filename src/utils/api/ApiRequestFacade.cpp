#include "ApiFactory.h"
#include "ApiRequestFacade.h"

std::shared_ptr<UKControllerPluginUtils::Api::ApiFactory> apiFactory;

[[nodiscard]] auto ApiRequest() -> UKControllerPluginUtils::Api::ApiRequestFactory&
{
    return apiFactory->RequestFactory();
}

void SetApiRequestFactory(std::shared_ptr<UKControllerPluginUtils::Api::ApiFactory> factory)
{
    apiFactory = factory;
}

auto ApiRequestFactorySet() -> bool
{
    return apiFactory != nullptr;
}

void UnsetSetApiFactory()
{
    apiFactory.reset();
}
