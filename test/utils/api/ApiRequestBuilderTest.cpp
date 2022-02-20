#include "api/ApiRequestBuilder.h"
#include "srd/SrdSearchParameters.h"

using ::testing::Test;

using UKControllerPlugin::Api::ApiRequestBuilder;
using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Srd::SrdSearchParameters;

namespace UKControllerPluginUtilsTest::Api {

    class ApiRequestBuilderTest : public Test
    {
        public:
        ApiRequestBuilderTest() : builder("http://testurl.com", "apikey")
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
        expectedRequest.SetMaxRequestTime(0L);

        EXPECT_TRUE(expectedRequest == this->builder.BuildGetUriRequest("someuri"));
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsASrdSearchRequest)
    {
        SrdSearchParameters params;
        params.origin = "EGKK";
        params.destination = "EGLL";

        CurlRequest expectedRequest(
            "http://testurl.com/srd/route/search?origin=EGKK&destination=EGLL", CurlRequest::METHOD_GET);
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
            CurlRequest::METHOD_GET);
        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        EXPECT_TRUE(expectedRequest == this->builder.BuildSrdQueryRequest(params));
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsGetAssignedHoldsRequest)
    {
        CurlRequest expectedRequest("http://testurl.com/hold/assigned", CurlRequest::METHOD_GET);
        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        EXPECT_TRUE(expectedRequest == this->builder.BuildAllAssignedHoldsRequest());
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsSetAssignedHoldRequest)
    {
        CurlRequest expectedRequest("http://testurl.com/hold/assigned", CurlRequest::METHOD_PUT);

        nlohmann::json expectedData;
        expectedData["callsign"] = "BAW123";
        expectedData["navaid"] = "TIMBA";
        expectedRequest.SetBody(expectedData.dump());

        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        EXPECT_TRUE(expectedRequest == this->builder.BuildSetAssignedHoldRequest("BAW123", "TIMBA"));
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsDeleteAssignedHoldRequest)
    {
        CurlRequest expectedRequest("http://testurl.com/hold/assigned/BAW123", CurlRequest::METHOD_DELETE);

        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        EXPECT_TRUE(expectedRequest == this->builder.BuildDeleteAssignedHoldRequest("BAW123"));
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsBuildEnrouteReleaseRequest)
    {
        CurlRequest expectedRequest("http://testurl.com/release/enroute", CurlRequest::METHOD_POST);

        nlohmann::json expectedData{
            {"callsign", "BAW123"},
            {"type", 1},
            {"initiating_controller", "LON_S_CTR"},
            {"target_controller", "LON_C_CTR"},
        };
        expectedRequest.SetBody(expectedData.dump());

        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        EXPECT_TRUE(expectedRequest == this->builder.BuildEnrouteReleaseRequest("BAW123", "LON_S_CTR", "LON_C_CTR", 1));
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsBuildEnrouteReleaseRequestWithReleasePoint)
    {
        CurlRequest expectedRequest("http://testurl.com/release/enroute", CurlRequest::METHOD_POST);

        nlohmann::json expectedData{
            {"callsign", "BAW123"},
            {"type", 1},
            {"initiating_controller", "LON_S_CTR"},
            {"target_controller", "LON_C_CTR"},
            {"release_point", "LAM"},
        };
        expectedRequest.SetBody(expectedData.dump());

        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        EXPECT_TRUE(
            expectedRequest ==
            this->builder.BuildEnrouteReleaseRequestWithReleasePoint("BAW123", "LON_S_CTR", "LON_C_CTR", 1, "LAM"));
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsGetAssignedStandsRequest)
    {
        CurlRequest expectedRequest("http://testurl.com/stand/assignment", CurlRequest::METHOD_GET);
        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        EXPECT_TRUE(expectedRequest == this->builder.BuildGetStandAssignmentsRequest());
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsSetAssignedStandForAircraftRequest)
    {
        CurlRequest expectedRequest("http://testurl.com/stand/assignment", CurlRequest::METHOD_PUT);

        nlohmann::json expectedData;
        expectedData["callsign"] = "BAW123";
        expectedData["stand_id"] = 1;
        expectedRequest.SetBody(expectedData.dump());

        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        EXPECT_TRUE(expectedRequest == this->builder.BuildAssignStandToAircraftRequest("BAW123", 1));
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsDeleteAssignedStandForAircraftRequest)
    {
        CurlRequest expectedRequest("http://testurl.com/stand/assignment/BAW123", CurlRequest::METHOD_DELETE);

        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        EXPECT_TRUE(expectedRequest == this->builder.BuildDeleteStandAssignmentForAircraftRequest("BAW123"));
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsGetAllNotificationsRequest)
    {
        CurlRequest expectedRequest("http://testurl.com/notifications", CurlRequest::METHOD_GET);

        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        EXPECT_TRUE(expectedRequest == this->builder.BuildGetAllNotificationsRequest());
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsGetUnreadNotificationsRequest)
    {
        CurlRequest expectedRequest("http://testurl.com/notifications/unread", CurlRequest::METHOD_GET);

        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        EXPECT_TRUE(expectedRequest == this->builder.BuildGetUnreadNotificationsRequest());
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsReadNotificationRequest)
    {
        CurlRequest expectedRequest("http://testurl.com/notifications/read/1", CurlRequest::METHOD_PUT);

        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        EXPECT_TRUE(expectedRequest == this->builder.BuildReadNotificationRequest(1));
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsLatestVersionDetailsRequest)
    {
        CurlRequest expectedRequest("http://testurl.com/version/latest?channel=beta", CurlRequest::METHOD_GET);

        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        EXPECT_TRUE(expectedRequest == this->builder.BuildLatestGithubVersionRequest("beta"));
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsPluginEventsSyncRequest)
    {
        CurlRequest expectedRequest("http://testurl.com/plugin-events/sync", CurlRequest::METHOD_GET);

        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        EXPECT_TRUE(expectedRequest == this->builder.BuildPluginEventSyncRequest());
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsGetLastestPluginEventsTest)
    {
        CurlRequest expectedRequest("http://testurl.com/plugin-events/recent?previous=5", CurlRequest::METHOD_GET);

        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        EXPECT_TRUE(expectedRequest == this->builder.BuildGetLatestPluginEventsRequest(5));
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsDepartureReleaseRequest)
    {
        CurlRequest expectedRequest("http://testurl.com/departure/release/request", CurlRequest::METHOD_POST);

        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        nlohmann::json expectedData;
        expectedData["callsign"] = "BAW123";
        expectedData["requesting_controller_id"] = 1;
        expectedData["target_controller_id"] = 3;
        expectedData["expires_in_seconds"] = 54;
        expectedRequest.SetBody(expectedData.dump());

        EXPECT_TRUE(expectedRequest == this->builder.BuildDepartureReleaseRequest("BAW123", 1, 3, 54));
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsApproveDepartureReleaseRequest)
    {
        CurlRequest expectedRequest(
            "http://testurl.com/departure/release/request/1/approve", CurlRequest::METHOD_PATCH);

        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        nlohmann::json expectedData;
        expectedData["controller_position_id"] = 2;
        expectedData["remarks"] = "Some remarks";
        expectedData["released_at"] = "2021-05-09 12:31:00";
        expectedData["expires_in_seconds"] = 120;
        expectedRequest.SetBody(expectedData.dump());

        std::chrono::system_clock::time_point timePoint;
        std::istringstream inputStream("2021-05-09 12:31:00");
        inputStream >> std::chrono::parse(std::string{"%Y-%m-%d %H:%M:%S"}, timePoint);

        EXPECT_TRUE(
            expectedRequest == this->builder.BuildApproveDepartureReleaseRequest(1, 2, timePoint, 120, "Some remarks"));
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsApproveDepartureReleaseRequestWithNoExpiry)
    {
        CurlRequest expectedRequest(
            "http://testurl.com/departure/release/request/1/approve", CurlRequest::METHOD_PATCH);

        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        nlohmann::json expectedData;
        expectedData["controller_position_id"] = 2;
        expectedData["remarks"] = "Some remarks";
        expectedData["released_at"] = "2021-05-09 12:31:00";
        expectedData["expires_in_seconds"] = nlohmann::json::value_t::null;
        expectedRequest.SetBody(expectedData.dump());

        std::chrono::system_clock::time_point timePoint;
        std::istringstream inputStream("2021-05-09 12:31:00");
        inputStream >> std::chrono::parse(std::string{"%Y-%m-%d %H:%M:%S"}, timePoint);

        EXPECT_TRUE(
            expectedRequest == this->builder.BuildApproveDepartureReleaseRequest(1, 2, timePoint, -1, "Some remarks"));
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsRejectDepartureReleaseRequest)
    {
        CurlRequest expectedRequest("http://testurl.com/departure/release/request/1/reject", CurlRequest::METHOD_PATCH);

        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        nlohmann::json expectedData;
        expectedData["controller_position_id"] = 2;
        expectedData["remarks"] = "Some remarks";
        expectedRequest.SetBody(expectedData.dump());

        EXPECT_TRUE(expectedRequest == this->builder.BuildRejectDepartureReleaseRequest(1, 2, "Some remarks"));
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsAcknowledgeDepartureReleaseRequest)
    {
        CurlRequest expectedRequest(
            "http://testurl.com/departure/release/request/1/acknowledge", CurlRequest::METHOD_PATCH);

        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        nlohmann::json expectedData;
        expectedData["controller_position_id"] = 2;
        expectedRequest.SetBody(expectedData.dump());

        EXPECT_TRUE(expectedRequest == this->builder.BuildAcknowledgeDepartureReleaseRequest(1, 2));
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsCancelDepartureReleaseRequest)
    {
        CurlRequest expectedRequest("http://testurl.com/departure/release/request/1", CurlRequest::METHOD_DELETE);

        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        EXPECT_TRUE(expectedRequest == this->builder.BuildCancelReleaseRequest(1));
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsCreatePrenoteMessageWithAllValues)
    {
        CurlRequest expectedRequest("http://testurl.com/prenotes/messages", CurlRequest::METHOD_POST);

        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        nlohmann::json expectedData = {
            {"callsign", "BAW123"},
            {"departure_airfield", "EGLC"},
            {"departure_sid", "CPT2T"},
            {"destination_airfield", "EGGD"},
            {"requesting_controller_id", 1},
            {"target_controller_id", 2},
            {"expires_in_seconds", 50},
        };
        expectedRequest.SetBody(expectedData.dump());

        EXPECT_TRUE(
            expectedRequest ==
            this->builder.BuildCreatePrenoteMessageRequest("BAW123", "EGLC", "CPT2T", "EGGD", 1, 2, 50));
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsCreatePrenoteMessageWithMissingValues)
    {
        CurlRequest expectedRequest("http://testurl.com/prenotes/messages", CurlRequest::METHOD_POST);

        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        nlohmann::json expectedData = {
            {"callsign", "BAW123"},
            {"departure_airfield", "EGLC"},
            {"departure_sid", nlohmann::json::value_t::null},
            {"destination_airfield", nlohmann::json::value_t::null},
            {"requesting_controller_id", 1},
            {"target_controller_id", 2},
            {"expires_in_seconds", 50},
        };
        expectedRequest.SetBody(expectedData.dump());

        EXPECT_TRUE(
            expectedRequest == this->builder.BuildCreatePrenoteMessageRequest("BAW123", "EGLC", "", "", 1, 2, 50));
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsAcknowledgePrenoteMessage)
    {
        CurlRequest expectedRequest("http://testurl.com/prenotes/messages/55/acknowledge", CurlRequest::METHOD_PATCH);

        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        nlohmann::json expectedData = {
            {"controller_position_id", 1},
        };
        expectedRequest.SetBody(expectedData.dump());

        EXPECT_TRUE(expectedRequest == this->builder.BuildAcknowledgePrenoteMessageRequest(55, 1));
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsDeletePrenoteMessage)
    {
        CurlRequest expectedRequest("http://testurl.com/prenotes/messages/55", CurlRequest::METHOD_DELETE);

        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        EXPECT_TRUE(expectedRequest == this->builder.BuildDeletePrenoteMessageRequest(55));
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsMissedApproachMessage)
    {
        CurlRequest expectedRequest("http://testurl.com/missed-approaches", CurlRequest::METHOD_POST);

        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        nlohmann::json expectedData = {{"callsign", "BAW123"}};
        expectedRequest.SetBody(expectedData.dump());

        EXPECT_TRUE(expectedRequest == this->builder.BuildMissedApproachMessage("BAW123"));
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsGetAllMetarsMessage)
    {
        CurlRequest expectedRequest("http://testurl.com/metar", CurlRequest::METHOD_GET);
        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        EXPECT_TRUE(expectedRequest == this->builder.BuildGetAllMetarsRequest());
    }

    TEST_F(ApiRequestBuilderTest, ItBuildsMissedApproachAcknowledge)
    {
        CurlRequest expectedRequest("http://testurl.com/missed-approaches/1", CurlRequest::METHOD_PATCH);
        expectedRequest.AddHeader("Authorization", "Bearer apikey");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        nlohmann::json expectedData = {{"remarks", "Some remarks"}};
        expectedRequest.SetBody(expectedData.dump());

        EXPECT_TRUE(expectedRequest == this->builder.BuildMissedApproachAcknowledgeMessage(1, "Some remarks"));
    }
} // namespace UKControllerPluginUtilsTest::Api
