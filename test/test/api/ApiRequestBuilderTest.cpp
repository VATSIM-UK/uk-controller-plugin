#include "pch/pch.h"
#include "api/ApiRequestBuilder.h"
#include "srd/SrdSearchParameters.h"

using UKControllerPlugin::Api::ApiRequestBuilder;
using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Srd::SrdSearchParameters;
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

        TEST_F(ApiRequestBuilderTest, ApiDomainCanBeUpdated)
        {
            this->builder.SetApiDomain("http://nottesturl.com");
            EXPECT_TRUE("http://nottesturl.com" == this->builder.GetApiDomain());
        }


        TEST_F(ApiRequestBuilderTest, ApiKeyCanBeUpdated)
        {
            this->builder.SetApiKey("notapikey");
            EXPECT_TRUE("notapikey" == this->builder.GetApiKey());
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsAuthCheckRequests)
        {
            CurlRequest expectedRequest("http://testurl.com/authorise", CurlRequest::METHOD_GET);
            expectedRequest.AddHeader("Authorization", "Bearer apikey");
            expectedRequest.AddHeader("Accept", "application/json");
            expectedRequest.AddHeader("Content-Type", "application/json");
            EXPECT_TRUE(expectedRequest == this->builder.BuildAuthCheckRequest());
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsDependencyListRequests)
        {
            CurlRequest expectedRequest("http://testurl.com/dependency", CurlRequest::METHOD_GET);
            expectedRequest.AddHeader("Authorization", "Bearer apikey");
            expectedRequest.AddHeader("Accept", "application/json");
            expectedRequest.AddHeader("Content-Type", "application/json");
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
            expectedRequest.AddHeader("Content-Type", "application/json");
            EXPECT_TRUE(expectedRequest == this->builder.BuildVersionCheckRequest("1.0.0"));
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsSquawkAssignmentDeletionRequests)
        {
            CurlRequest expectedRequest("http://testurl.com/squawk-assignment/BAW123", CurlRequest::METHOD_DELETE);
            expectedRequest.AddHeader("Authorization", "Bearer apikey");
            expectedRequest.AddHeader("Accept", "application/json");
            expectedRequest.AddHeader("Content-Type", "application/json");
            EXPECT_TRUE(expectedRequest == this->builder.BuildSquawkAssignmentDeletionRequest("BAW123"));
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsSquawkAssignmentCheckRequests)
        {
            CurlRequest expectedRequest("http://testurl.com/squawk-assignment/BAW123", CurlRequest::METHOD_GET);
            expectedRequest.AddHeader("Authorization", "Bearer apikey");
            expectedRequest.AddHeader("Accept", "application/json");
            expectedRequest.AddHeader("Content-Type", "application/json");
            EXPECT_TRUE(expectedRequest == this->builder.BuildSquawkAssignmentCheckRequest("BAW123"));
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsGeneralSquawkAssignmentRequests)
        {
            CurlRequest expectedRequest("http://testurl.com/squawk-assignment/BAW123", CurlRequest::METHOD_PUT);
            expectedRequest.AddHeader("Authorization", "Bearer apikey");
            expectedRequest.AddHeader("Accept", "application/json");
            expectedRequest.AddHeader("Content-Type", "application/json");

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
            expectedRequest.AddHeader("Content-Type", "application/json");

            nlohmann::json expectedBodyJson;
            expectedBodyJson["type"] = "local";
            expectedBodyJson["unit"] = "EGKK";
            expectedBodyJson["rules"] = "V";
            expectedRequest.SetBody(expectedBodyJson.dump());

            EXPECT_TRUE(expectedRequest == this->builder.BuildLocalSquawkAssignmentRequest("BAW123", "EGKK", "V"));
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsHoldDependencyDataRequests)
        {
            CurlRequest expectedRequest("http://testurl.com/hold", CurlRequest::METHOD_GET);
            expectedRequest.AddHeader("Authorization", "Bearer apikey");
            expectedRequest.AddHeader("Accept", "application/json");
            expectedRequest.AddHeader("Content-Type", "application/json");
            EXPECT_TRUE(expectedRequest == this->builder.BuildHoldDependencyRequest());
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsUserHoldProfileDownloadRequests)
        {
            CurlRequest expectedRequest("http://testurl.com/hold/profile", CurlRequest::METHOD_GET);
            expectedRequest.AddHeader("Authorization", "Bearer apikey");
            expectedRequest.AddHeader("Accept", "application/json");
            expectedRequest.AddHeader("Content-Type", "application/json");
            EXPECT_TRUE(expectedRequest == this->builder.BuildUserHoldProfilesRequest());
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsUserHoldProfileDeleteRequests)
        {
            CurlRequest expectedRequest("http://testurl.com/hold/profile/1", CurlRequest::METHOD_DELETE);
            expectedRequest.AddHeader("Authorization", "Bearer apikey");
            expectedRequest.AddHeader("Accept", "application/json");
            expectedRequest.AddHeader("Content-Type", "application/json");
            EXPECT_TRUE(expectedRequest == this->builder.BuildDeleteUserHoldProfileRequest(1));
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsUserHoldProfileCreateRequests)
        {
            nlohmann::json expectedData;
            expectedData["name"] = "Test";
            expectedData["holds"] = { 1, 2 };

            CurlRequest expectedRequest("http://testurl.com/hold/profile", CurlRequest::METHOD_PUT);
            expectedRequest.SetBody(expectedData.dump());
            expectedRequest.AddHeader("Authorization", "Bearer apikey");
            expectedRequest.AddHeader("Accept", "application/json");
            expectedRequest.AddHeader("Content-Type", "application/json");

            std::set<unsigned int> ids;
            ids.insert(1);
            ids.insert(2);
            EXPECT_TRUE(expectedRequest == this->builder.BuildCreateUserHoldProfileRequest("Test", ids));
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsUserHoldProfileUpdateRequests)
        {
            nlohmann::json expectedData;
            expectedData["name"] = "Test";
            expectedData["holds"] = { 1, 2 };

            CurlRequest expectedRequest("http://testurl.com/hold/profile/1", CurlRequest::METHOD_PUT);
            expectedRequest.SetBody(expectedData.dump());
            expectedRequest.AddHeader("Authorization", "Bearer apikey");
            expectedRequest.AddHeader("Accept", "application/json");
            expectedRequest.AddHeader("Content-Type", "application/json");

            std::set<unsigned int> ids;
            ids.insert(1);
            ids.insert(2);
            EXPECT_TRUE(expectedRequest == this->builder.BuildUpdateUserHoldProfileRequest(1, "Test", ids));
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsAWebsocketChannelAuthRequest)
        {
            CurlRequest expectedRequest("http://testurl.com/broadcasting/auth", CurlRequest::METHOD_POST);
            expectedRequest.AddHeader("Authorization", "Bearer apikey");
            expectedRequest.AddHeader("Accept", "application/json");
            expectedRequest.AddHeader("Content-Type", "application/json");

            nlohmann::json expectedData;
            expectedData["socket_id"] = "somesocket";
            expectedData["channel_name"] = "somelovelychannel";
            expectedRequest.SetBody(expectedData.dump());

            EXPECT_TRUE(
                expectedRequest == this->builder.BuildWebsocketChannelAuthRequest("somesocket", "somelovelychannel")
            );
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsAMinStackRequest)
        {
            CurlRequest expectedRequest("http://testurl.com/msl", CurlRequest::METHOD_GET);
            expectedRequest.AddHeader("Authorization", "Bearer apikey");
            expectedRequest.AddHeader("Accept", "application/json");
            expectedRequest.AddHeader("Content-Type", "application/json");

            EXPECT_TRUE(expectedRequest == this->builder.BuildMinStackLevelRequest());
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsARegionalPressureRequest)
        {
            CurlRequest expectedRequest("http://testurl.com/regional-pressure", CurlRequest::METHOD_GET);
            expectedRequest.AddHeader("Authorization", "Bearer apikey");
            expectedRequest.AddHeader("Accept", "application/json");
            expectedRequest.AddHeader("Content-Type", "application/json");

            EXPECT_TRUE(expectedRequest == this->builder.BuildRegionalPressureRequest());
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsAGetUriRequest)
        {
            CurlRequest expectedRequest("someuri", CurlRequest::METHOD_GET);
            expectedRequest.AddHeader("Authorization", "Bearer apikey");
            expectedRequest.AddHeader("Accept", "application/json");
            expectedRequest.AddHeader("Content-Type", "application/json");

            EXPECT_TRUE(expectedRequest == this->builder.BuildGetUriRequest("someuri"));
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsASrdSearchRequest)
        {
            SrdSearchParameters params;
            params.origin = "EGKK";
            params.destination = "EGLL";

            CurlRequest expectedRequest(
                "http://testurl.com/srd/route/search?origin=EGKK&destination=EGLL",
                CurlRequest::METHOD_GET
            );
            expectedRequest.AddHeader("Authorization", "Bearer apikey");
            expectedRequest.AddHeader("Accept", "application/json");
            expectedRequest.AddHeader("Content-Type", "application/json");

            EXPECT_TRUE(expectedRequest == this->builder.BuildSrdQueryRequest(params));
        }

        TEST_F(ApiRequestBuilderTest, ItBuildsASrdSearchRequestWithRequestedLevel)
        {
            SrdSearchParameters params;
            params.origin = "EGKK";
            params.destination = "EGLL";
            params.requestedLevel = 15000;

            CurlRequest expectedRequest(
                "http://testurl.com/srd/route/search?origin=EGKK&destination=EGLL&requestedLevel=15000",
                CurlRequest::METHOD_GET
            );
            expectedRequest.AddHeader("Authorization", "Bearer apikey");
            expectedRequest.AddHeader("Accept", "application/json");
            expectedRequest.AddHeader("Content-Type", "application/json");

            EXPECT_TRUE(expectedRequest == this->builder.BuildSrdQueryRequest(params));
        }
    }  // namespace Api
}  // namespace UKControllerPluginTest
