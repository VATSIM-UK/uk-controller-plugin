#include "ApiException.h"
#include "ApiRequestData.h"
#include "ApiRequestException.h"
#include "ApiRequestPerformerInterface.h"
#include "ChainableRequest.h"

using UKControllerPlugin::Api::ApiException;

namespace UKControllerPluginUtils::Api {
    ChainableRequest::ChainableRequest(const ApiRequestData& data, ApiRequestPerformerInterface& performer)
        : continuable(cti::make_continuable<Response>([data, &performer](auto&& promise) {
              try {
                  promise.set_value(performer.Perform(data));
              } catch (std::exception exception) {
                  promise.set_exception(std::make_exception_ptr(exception));
              }
          }))
    {
    }

    void ChainableRequest::Then(const std::function<Response(Response)>& function)
    {
        continuable = std::move(continuable).then(function);
    }

    void ChainableRequest::Then(const std::function<void(Response)>& function)
    {
        continuable = std::move(continuable).then([function](Response response) {
            function(response);
            return response;
        });
    }

    void ChainableRequest::Then(const std::function<void(void)>& function)
    {
        continuable = std::move(continuable).then([function](Response response) {
            function();
            return response;
        });
    }

    void ChainableRequest::Catch(const std::function<void(const ApiRequestException&)>& function)
    {
        continuable = std::move(continuable).fail([function](std::exception_ptr exception) {
            try {
                std::rethrow_exception(exception);
            } catch (const ApiRequestException& requestException) {
                function(requestException);
            } catch (const std::exception& otherException) {
                LogError("Exception thrown during API request: " + std::string(otherException.what()));
            }
        });
    }

    void ChainableRequest::Await()
    {
        std::move(continuable).freeze().apply(cti::transforms::wait());
    }
} // namespace UKControllerPluginUtils::Api
