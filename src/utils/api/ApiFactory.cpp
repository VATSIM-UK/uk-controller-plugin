#include "AbstractApiRequestPerformerFactory.h"
#include "ApiAuthorisationRequestFactory.h"
#include "ApiFactory.h"
#include "ApiRequestBuilder.h"
#include "ApiRequestFactory.h"
#include "ApiSettings.h"
#include "ApiSettingsProviderInterface.h"

using UKControllerPlugin::Api::ApiRequestBuilder;

namespace UKControllerPluginUtils::Api {

    ApiFactory::ApiFactory(
        std::shared_ptr<ApiSettingsProviderInterface> settingsProvider,
        std::shared_ptr<AbstractApiRequestPerformerFactory> requestPerformerFactory)
        : settingsProvider(settingsProvider), requestPerformerFactory(std::move(requestPerformerFactory))
    {
    }

    ApiFactory::~ApiFactory() = default;

    auto ApiFactory::SettingsProvider() -> const std::shared_ptr<ApiSettingsProviderInterface>
    {
        return settingsProvider;
    }

    auto ApiFactory::RequestFactory() -> ApiRequestFactory&
    {
        if (requestFactory == nullptr) {
            requestFactory =
                std::make_unique<ApiRequestFactory>(requestPerformerFactory->Make(SettingsProvider()->Get()));
        }

        return *requestFactory;
    }

    auto ApiFactory::LegacyRequestBuilder() -> const ApiRequestBuilder&
    {
        if (legacyRequestBuilder == nullptr) {
            legacyRequestBuilder = std::make_unique<ApiRequestBuilder>(SettingsProvider()->Get());
        }

        return *legacyRequestBuilder;
    }

    auto ApiFactory::AuthorisationRequestFactory() -> const ApiAuthorisationRequestFactory&
    {
        if (authorisationRequestFactory == nullptr) {
            authorisationRequestFactory = std::make_unique<ApiAuthorisationRequestFactory>(RequestFactory());
        }

        return *authorisationRequestFactory;
    }
} // namespace UKControllerPluginUtils::Api
