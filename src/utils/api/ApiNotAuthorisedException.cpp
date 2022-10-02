
#include "api/ApiNotAuthorisedException.h"

namespace UKControllerPlugin::Api {
    ApiNotAuthorisedException::ApiNotAuthorisedException(const std::string& message)
        : ApiException("ApiNotAuthorisedException: " + message)
    {
    }
} // namespace UKControllerPlugin::Api
