#pragma once
#include "Response.h"

namespace UKControllerPluginUtils::Api {
    class ApiRequestData;
    class ApiRequestException;
    class ApiRequestPerformerInterface;

    /**
     * Hides the details of the continuation library.
     */
    class ChainableRequest
    {
        public:
        ChainableRequest(
            const ApiRequestData& data,
            ApiRequestPerformerInterface& performer,
            std::function<void(void)> onCompletion);
        ~ChainableRequest();
        void Then(const std::function<void(Response)>& function);
        void Then(const std::function<void(void)>& function);
        void Catch(const std::function<void(const ApiRequestException&)>& function);
        void Await();

        private:
        void ApplyOnCompletion();

        // Continuable instance
        cti::continuable<Response> continuable;

        // A function to run when we're all done
        std::function<void(void)> onCompletion;

        // Has the request executed
        bool executed = false;
    };
} // namespace UKControllerPluginUtils::Api
