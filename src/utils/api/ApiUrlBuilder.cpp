#include "ApiRequestData.h"
#include "ApiSettings.h"
#include "ApiUrlBuilder.h"
#include "string/StringTrimFunctions.h"

namespace UKControllerPluginUtils::Api {

    ApiUrlBuilder::ApiUrlBuilder(const ApiSettings& settings) : settings(settings)
    {
    }

    auto ApiUrlBuilder::BuildUrl(const ApiRequestData& requestData) const -> const std::string
    {
        return String::rtrim(settings.Url()) + "/" + String::trim(requestData.Uri());
    }
} // namespace UKControllerPluginUtils::Api
