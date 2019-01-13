#pragma once
#include "curl/CurlInterface.h"
#include "curl/CurlRequest.h"

namespace UKControllerPluginTest {
    namespace Curl {
        class MockCurlApi : public UKControllerPlugin::Curl::CurlInterface
        {
            public:
                MOCK_METHOD1(
                    MakeCurlRequest,
                    UKControllerPlugin::Curl::CurlResponse(const UKControllerPlugin::Curl::CurlRequest & curlRequest)
                );
            };
    }  // namespace Curl
}  // namespace UKControllerPluginTest
