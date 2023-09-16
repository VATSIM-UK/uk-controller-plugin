#pragma once

namespace UKControllerPlugin::Curl {
    class CurlInterface;
} // namespace UKControllerPlugin::Curl

namespace UKControllerPlugin::ECFMP {

    /**
     * An HTTP client instance that can be passed to ECFMP.
     */
    class HttpClient : public ::ECFMP::Http::HttpClient
    {
        public:
        HttpClient(Curl::CurlInterface& curl);
        [[nodiscard]] auto Get(const std::string& url) -> ::ECFMP::Http::HttpResponse override;

        private:
        static const int CURL_ERROR_STATUS_CODE = 999L;

        // The curl API
        Curl::CurlInterface& curl;
    };

} // namespace UKControllerPlugin::ECFMP
