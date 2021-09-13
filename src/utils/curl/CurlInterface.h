#pragma once
#include "CurlResponse.h"

namespace UKControllerPlugin::Curl {
    class CurlRequest;

    /*
        Interface to the libcurl library.
    */
    class CurlInterface
    {
        public:
        CurlInterface();
        virtual ~CurlInterface();
        CurlInterface(const CurlInterface&);
        CurlInterface(CurlInterface&&) noexcept = delete;
        [[nodiscard]] auto operator=(const CurlInterface&) -> CurlInterface&;
        [[nodiscard]] auto operator=(CurlInterface&&) noexcept -> CurlInterface& = delete;
        virtual auto MakeCurlRequest(const UKControllerPlugin::Curl::CurlRequest& request)
            -> UKControllerPlugin::Curl::CurlResponse = 0;
    };
} // namespace UKControllerPlugin::Curl
