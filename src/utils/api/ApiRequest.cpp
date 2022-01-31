#include "ApiException.h"
#include "ApiRequest.h"
#include "ApiRequestPerformerInterface.h"

using UKControllerPlugin::Api::ApiException;

namespace UKControllerPluginUtils::Api {

    ApiRequest::ApiRequest(const ApiRequestData& data, ApiRequestPerformerInterface& performer, bool async)
        : continuable(cti::make_continuable<Response>([data, &performer](auto&& promise) {
              try {
                  promise.set_value(performer.Perform(data));
              } catch (ApiException& exception) {
                  promise.set_exception(std::make_exception_ptr(exception));
              }
          })),
          async(async)
    {
    }

    ApiRequest::ApiRequest(ApiRequest&& request) noexcept
        : continuable(std::move(request.continuable)), async(request.async)
    {
    }

    ApiRequest::~ApiRequest()
    {
        if (!async) {
            continuable.freeze();
            std::move(continuable).apply(cti::transforms::wait());
        }
    }

    auto ApiRequest::Then(const std::function<Response(Response)>& function) -> ApiRequest
    {
        continuable = std::move(continuable).then(function);
        return std::move(*this);
    }

    auto ApiRequest::Catch(const std::function<void(std::exception_ptr exception)>& function) -> ApiRequest
    {
        continuable = std::move(continuable).fail(function);
        return std::move(*this);
    }

    auto ApiRequest::Await() -> ApiRequest
    {
        async = false;
        return std::move(*this);
    }
} // namespace UKControllerPluginUtils::Api
