#pragma once

namespace UKControllerPluginUtils::Api {
    class ApiConfigurationListenerInterface
    {
        public:
        virtual ~ApiConfigurationListenerInterface() = default;

        /**
         * Get the host this listener is listening on.
         */
        [[nodiscard]] virtual auto Host() const -> const std::string& = 0;

        /**
         * Get the host this listener is listening on.
         */
        [[nodiscard]] virtual auto Port() const -> int = 0;
    };
} // namespace UKControllerPluginUtils::Api
