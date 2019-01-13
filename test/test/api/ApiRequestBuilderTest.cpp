#include "pch/pch.h"
#include "api/ApiRequestBuilder.h"
#include "curl/CurlRequest.h"

using UKControllerPlugin::Api::ApiRequestBuilder;
using UKControllerPlugin::Curl::CurlRequest;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Api {

        class ApiRequestBuilderTest : public Test
        {
            public:
                ApiRequestBuilderTest()
                    : builder("http://testurl.com", "apikey")
                {

                }
                ApiRequestBuilder builder;
        };

        TEST_F(ApiRequestBuilderTest, ItHasAnApiDomain)
        {
            EXPECT_TRUE("http://testurl.com" == this->builder.GetApiDomain());
        }

        TEST_F(ApiRequestBuilderTest, ItHasAnApiKey)
        {
            EXPECT_TRUE("apikey" == this->builder.GetApiKey());
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsAuthCheckRequests)
        {
            CurlRequest expectedRequest("http://testurl.com/", CurlRequest::METHOD_GET);
            expectedRequest.AddHeader("Authorization", "Bearer apikey");
            expectedRequest.AddHeader("Accept", "application/json");
            EXPECT_TRUE(expectedRequest == this->builder.BuildAuthCheckRequest());
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsDependencyListRequests)
        {
            CurlRequest expectedRequest("http://testurl.com/dependency", CurlRequest::METHOD_GET);
            expectedRequest.AddHeader("Authorization", "Bearer apikey");
            expectedRequest.AddHeader("Accept", "application/json");
            EXPECT_TRUE(expectedRequest == this->builder.BuildDependencyListRequest());
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsRemoteFileDownloadRequests)
        {
            CurlRequest expectedRequest("http://testurl.com/files/test1.json", CurlRequest::METHOD_GET);
            EXPECT_TRUE(expectedRequest == this->builder.BuildRemoteFileRequest("http://testurl.com/files/test1.json"));
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsVersionCheckRequests)
        {
            CurlRequest expectedRequest("http://testurl.com/version/1.0.0/status", CurlRequest::METHOD_GET);
            expectedRequest.AddHeader("Authorization", "Bearer apikey");
            expectedRequest.AddHeader("Accept", "application/json");
            EXPECT_TRUE(expectedRequest == this->builder.BuildVersionCheckRequest("1.0.0"));
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsSquawkAssignmentDeletionRequests)
        {
            CurlRequest expectedRequest("http://testurl.com/squawk-assignment/BAW123", CurlRequest::METHOD_DELETE);
            expectedRequest.AddHeader("Authorization", "Bearer apikey");
            expectedRequest.AddHeader("Accept", "application/json");
            EXPECT_TRUE(expectedRequest == this->builder.BuildSquawkAssignmentDeletionRequest("BAW123"));
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsSquawkAssignmentCheckRequests)
        {
            CurlRequest expectedRequest("http://testurl.com/squawk-assignment/BAW123", CurlRequest::METHOD_GET);
            expectedRequest.AddHeader("Authorization", "Bearer apikey");
            expectedRequest.AddHeader("Accept", "application/json");
            EXPECT_TRUE(expectedRequest == this->builder.BuildSquawkAssignmentCheckRequest("BAW123"));
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsGeneralSquawkAssignmentRequests)
        {
            CurlRequest expectedRequest("http://testurl.com/squawk-assignment/BAW123", CurlRequest::METHOD_PUT);
            expectedRequest.AddHeader("Authorization", "Bearer apikey");
            expectedRequest.AddHeader("Accept", "application/json");

            nlohmann::json expectedBodyJson;
            expectedBodyJson["type"] = "general";
            expectedBodyJson["origin"] = "EGKK";
            expectedBodyJson["destination"] = "EGLL";
            expectedRequest.SetBody(expectedBodyJson.dump());

            EXPECT_TRUE(expectedRequest == this->builder.BuildGeneralSquawkAssignmentRequest("BAW123", "EGKK", "EGLL"));
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsLocalSquawkAssignmentRequests)
        {
            CurlRequest expectedRequest("http://testurl.com/squawk-assignment/BAW123", CurlRequest::METHOD_PUT);
            expectedRequest.AddHeader("Authorization", "Bearer apikey");
            expectedRequest.AddHeader("Accept", "application/json");

            nlohmann::json expectedBodyJson;
            expectedBodyJson["type"] = "local";
            expectedBodyJson["unit"] = "EGKK";
            expectedBodyJson["rules"] = "V";
            expectedRequest.SetBody(expectedBodyJson.dump());

            EXPECT_TRUE(expectedRequest == this->builder.BuildLocalSquawkAssignmentRequest("BAW123", "EGKK", "V"));
        }
    }  // namespace Api
}  // namespace UKControllerPluginTest
