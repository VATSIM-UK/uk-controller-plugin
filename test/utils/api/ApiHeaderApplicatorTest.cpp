#include "api/ApiHeaderApplicator.h"
#include "api/ApiSettings.h"
#include "curl/CurlRequest.h"

using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPluginUtils::Api::ApiHeaderApplicator;
using UKControllerPluginUtils::Api::ApiSettings;

namespace UKControllerPluginUtilsTest::Api {
    class ApiHeaderApplicatorTest : public testing::Test
    {
        public:
        ApiHeaderApplicatorTest()
            : request("https://ukcp.vatsim.uk/authorise", CurlRequest::METHOD_GET),
              settings("https://ukcp.vatsim.uk", "apikey"), applicator(settings)
        {
        }

        CurlRequest request;
        ApiSettings settings;
        ApiHeaderApplicator applicator;
    };

    TEST_F(ApiHeaderApplicatorTest, ItAppliesHeaders)
    {
        CurlRequest expectedRequest("https://ukcp.vatsim.uk/authorise", CurlRequest::METHOD_GET);
        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        applicator.ApplyHeaders(request);
        EXPECT_EQ(expectedRequest, request);
    }
} // namespace UKControllerPluginUtilsTest::Api
