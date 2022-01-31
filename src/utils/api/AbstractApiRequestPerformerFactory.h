#pragma once

namespace UKControllerPluginUtils::Api {
    class ApiRequestPerformerInterface;
    class ApiSettings;

    /**
     * Builds API request performers, allowing them to be subbed out for
     * mocks in tests etc.
     */
    class AbstractApiRequestPerformerFactory
    {
        public:
        virtual ~AbstractApiRequestPerformerFactory() = default;
        [[nodiscard]] virtual auto Make(const ApiSettings& apiSettings) -> ApiRequestPerformerInterface& = 0;
    };
} // namespace UKControllerPluginUtils::Api
