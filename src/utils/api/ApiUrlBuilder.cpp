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
        return String::rtrim(settings.Url(), URL_PATH_SEPARATOR) + URL_PATH_SEPARATOR +
               String::trim(requestData.Uri(), URL_PATH_SEPARATOR);
    }
} // namespace UKControllerPluginUtils::Api
