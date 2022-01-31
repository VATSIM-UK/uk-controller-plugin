#pragma once
#include "Response.h"

namespace UKControllerPluginUtils::Api {
    class ApiRequestData;
    class ApiRequestPerformerInterface;

    /**
     * Hides the details of the continuation library.
     */
    class ChainableRequest
    {
        public:
        ChainableRequest(const ApiRequestData& data, ApiRequestPerformerInterface& performer);
        void Then(const std::function<Response(Response)>& function);
        void Catch(const std::function<void(std::exception_ptr exception)>& function);
        void Await();

        private:
        
        // Continuable instance
        cti::continuable<Response> continuable;
    };
} // namespace UKControllerPluginUtils::Api
