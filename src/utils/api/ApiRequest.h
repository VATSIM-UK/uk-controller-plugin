#pragma once
#include "ApiRequestData.h"
#include "Response.h"

namespace UKControllerPluginUtils::Api {
    class ApiRequestPerformerInterface;

    class ApiRequest
    {
        public:
        ApiRequest(const ApiRequestData& data, ApiRequestPerformerInterface& performer, bool async = true);
        ApiRequest(ApiRequest&& request) noexcept;
        ~ApiRequest();
        auto Then(const std::function<Response(Response)>& function) -> ApiRequest;
        auto Catch(const std::function<void(std::exception_ptr exception)>& function) -> ApiRequest;
        auto Await() -> ApiRequest;

        private:
        // Allows continuation
        cti::continuable<Response> continuable;
        
        // Run on main thread or async
        bool async;
    };

} // namespace UKControllerPluginUtils::Api
