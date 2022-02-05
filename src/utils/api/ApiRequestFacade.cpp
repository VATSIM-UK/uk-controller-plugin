#include "ApiRequestFacade.h"
#include "ApiFactory.h"

namespace UKControllerPlugin::Api {
    std::shared_ptr<UKControllerPluginUtils::Api::ApiFactory> apiFactory;

    [[nodiscard]] auto ApiRequest() -> const UKControllerPluginUtils::Api::ApiRequestFactory&
    {
        return apiFactory->RequestFactory();
    }

    void SetApiRequestFactory(std::shared_ptr<UKControllerPluginUtils::Api::ApiFactory> factory)
    {
        if (apiFactory) {
            return;
        }

        apiFactory = factory;
    }
} // namespace UKControllerPlugin::Api
