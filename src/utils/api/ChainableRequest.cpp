#include "ApiRequestData.h"
#include "ApiRequestException.h"
#include "ApiRequestPerformerInterface.h"
#include "ChainableRequest.h"

namespace UKControllerPluginUtils::Api {
    ChainableRequest::ChainableRequest(
        const ApiRequestData& data, ApiRequestPerformerInterface& performer, std::function<void(void)> onCompletion)
        : continuable(cti::make_continuable<Response>([data, &performer](auto&& promise) {
              try {
                  promise.set_value(performer.Perform(data));
              } catch (ApiRequestException& exception) {
                  promise.set_exception(std::make_exception_ptr(exception));
              }
          })),
          onCompletion(onCompletion)
    {
    }

    ChainableRequest::~ChainableRequest()
    {
        if (!this->executed) {
            this->ApplyOnCompletion();
        }
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
        auto complete = this->onCompletion;
        continuable = std::move(continuable).fail([function, complete, this](std::exception_ptr exception) {
            try {
                std::rethrow_exception(exception);
            } catch (const ApiRequestException& requestException) {
                function(requestException);
                this->executed = true;
                complete();
            } catch (const std::exception&) {
                // Everythings over now
            }

            return cti::cancel();
        });
    }

    void ChainableRequest::Await()
    {
        this->ApplyOnCompletion();
        std::move(continuable).apply(cti::transforms::wait());
    }

    void ChainableRequest::ApplyOnCompletion()
    {
        auto complete = this->onCompletion;
        this->Then([this, complete]() {
            this->executed = true;
            complete();
        });
    }
} // namespace UKControllerPluginUtils::Api
