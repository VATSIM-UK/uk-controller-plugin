#pragma once

namespace UKControllerPluginUtils::Api {
    class ApiFactory;
    class ApiRequestFactory;
} // namespace UKControllerPluginUtils::Api

[[nodiscard]] auto ApiRequest() -> const UKControllerPluginUtils::Api::ApiRequestFactory&;
void SetApiRequestFactory(std::shared_ptr<UKControllerPluginUtils::Api::ApiFactory> factory);
