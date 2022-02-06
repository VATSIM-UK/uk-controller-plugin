#pragma once
#include "Response.h"

namespace UKControllerPluginUtils::Api {
    class ApiRequestData;

    /**
     * Performs API requests and returns a response or throws an exception.
     *
     * Provides a handy mocking point for API-related fun.
     */
    class ApiRequestPerformerInterface
    {
        public:
        virtual ~ApiRequestPerformerInterface() = default;
        /**
         * Perform the request.
         *
         * @throws ApiRequestException on failure
         */
        [[nodiscard]] virtual auto Perform(const ApiRequestData& data) -> Response = 0;
    };
} // namespace UKControllerPluginUtils::Api
