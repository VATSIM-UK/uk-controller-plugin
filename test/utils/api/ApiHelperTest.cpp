#include "api/ApiException.h"
#include "api/ApiHelper.h"
#include "api/ApiNotAuthorisedException.h"
#include "api/ApiNotFoundException.h"
#include "curl/CurlInterface.h"
#include "helper/ApiRequestHelperFunctions.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Api::ApiHelper;
using UKControllerPlugin::Api::ApiNotAuthorisedException;
using UKControllerPlugin::Api::ApiNotFoundException;
using UKControllerPlugin::Api::ApiRequestBuilder;
using UKControllerPlugin::Api::ApiResponse;
using UKControllerPlugin::Curl::CurlInterface;
using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Curl::CurlResponse;
using UKControllerPlugin::Squawk::ApiSquawkAllocation;
using UKControllerPlugin::Srd::SrdSearchParameters;
using UKControllerPluginTest::Curl::MockCurlApi;

namespace UKControllerPluginUtilsTest::Api {

    class ApiHelperTest : public Test
    {
        public:
        ApiHelperTest() : helper(mockCurlApi, GetApiRequestBuilder())
        {
        }

        ApiHelper helper;
        NiceMock<MockCurlApi> mockCurlApi;
    };

    TEST_F(ApiHelperTest, TestItReturnsApiAuthorisedIf200)
    {
        CurlResponse response(R"({"message": "teapots"})", false, 200);

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/authorise", CurlRequest::METHOD_GET)))
            .Times(1)
            .WillOnce(Return(response));

        EXPECT_TRUE(this->helper.CheckApiAuthorisation());
    }

    TEST_F(ApiHelperTest, TestItReturnsNotApiAuthorisedIfNot200)
    {
        CurlResponse response(R"({"message": "teapots"})", false, 201);

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/authorise", CurlRequest::METHOD_GET)))
            .Times(1)
            .WillOnce(Return(response));

        EXPECT_FALSE(this->helper.CheckApiAuthorisation());
    }

    TEST_F(ApiHelperTest, TestItThrowsNotFoundExceptionIf404)
    {
        CurlResponse response(R"({"message": "teapots"})", false, 404);

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/authorise", CurlRequest::METHOD_GET)))
            .Times(1)
            .WillOnce(Return(response));

        EXPECT_THROW(static_cast<void>(this->helper.CheckApiAuthorisation()), ApiNotFoundException);
    }

    TEST_F(ApiHelperTest, TestItThrowsNotAuthorisedExceptionIf401)
    {
        CurlResponse response(R"({"message": "teapots"})", false, 401);

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/authorise", CurlRequest::METHOD_GET)))
            .Times(1)
            .WillOnce(Return(response));

        EXPECT_THROW(static_cast<void>(this->helper.CheckApiAuthorisation()), ApiNotAuthorisedException);
    }

    TEST_F(ApiHelperTest, TestItThrowsNotAuthorisedExceptionIf403)
    {
        CurlResponse response(R"({"message": "teapots"})", false, 403);

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/authorise", CurlRequest::METHOD_GET)))
            .Times(1)
            .WillOnce(Return(response));

        EXPECT_THROW(static_cast<void>(this->helper.CheckApiAuthorisation()), ApiNotAuthorisedException);
    }

    TEST_F(ApiHelperTest, TestItThrowsApiExceptionIfServiceUnavailable)
    {
        CurlResponse response(R"({"message": "teapots"})", false, 503);

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/authorise", CurlRequest::METHOD_GET)))
            .Times(1)
            .WillOnce(Return(response));

        EXPECT_THROW(static_cast<void>(this->helper.CheckApiAuthorisation()), ApiException);
    }

    TEST_F(ApiHelperTest, TestItThrowsApiExceptionIfBadGateway)
    {
        CurlResponse response(R"({"message": "teapots"})", false, 502);

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/authorise", CurlRequest::METHOD_GET)))
            .Times(1)
            .WillOnce(Return(response));

        EXPECT_THROW(static_cast<void>(this->helper.CheckApiAuthorisation()), ApiException);
    }

    TEST_F(ApiHelperTest, TestItThrowsApiExceptionIfServerError)
    {
        CurlResponse response(R"({"message": "teapots"})", false, 500);

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/authorise", CurlRequest::METHOD_GET)))
            .Times(1)
            .WillOnce(Return(response));

        EXPECT_THROW(static_cast<void>(this->helper.CheckApiAuthorisation()), ApiException);
    }

    TEST_F(ApiHelperTest, ItThrowsAnExceptionIfBadRequest)
    {
        CurlResponse response(R"({"message": "teapots"})", false, 401);

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/authorise", CurlRequest::METHOD_GET)))
            .Times(1)
            .WillOnce(Return(response));

        EXPECT_THROW(static_cast<void>(this->helper.CheckApiAuthorisation()), ApiException);
    }

    TEST_F(ApiHelperTest, ItThrowsAnExceptionIfUnknownResponseCode)
    {
        CurlResponse response(R"({"message": "teapots"})", false, 666);

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/authorise", CurlRequest::METHOD_GET)))
            .Times(1)
            .WillOnce(Return(response));

        EXPECT_THROW(static_cast<void>(this->helper.CheckApiAuthorisation()), ApiException);
    }

    TEST_F(ApiHelperTest, FetchRemoteFileReturnsFileString)
    {
        nlohmann::json responseJson;
        responseJson["test"] = "hi!";
        CurlResponse response(responseJson.dump(), false, 200);

        EXPECT_CALL(
            this->mockCurlApi, MakeCurlRequest(CurlRequest("http://test.com/averynicefile", CurlRequest::METHOD_GET)))
            .Times(1)
            .WillOnce(Return(response));

        EXPECT_TRUE(responseJson.dump() == this->helper.FetchRemoteFile("http://test.com/averynicefile"));
    }

    TEST_F(ApiHelperTest, GetSquawkAssignmentHandlesNonJsonResponse)
    {
        CurlResponse response("<html>here is some html that means something went wrong</html>", false, 200);

        EXPECT_CALL(
            this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_GET)))
            .Times(1)
            .WillOnce(Return(response));

        EXPECT_THROW(static_cast<void>(this->helper.GetAssignedSquawk("BAW123")), ApiException);
    }

    TEST_F(ApiHelperTest, GetSquawkAssignmentReturnsSquawkAllocation)
    {
        CurlResponse response(R"({"squawk": "1234"})", false, 200);

        EXPECT_CALL(
            this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_GET)))
            .Times(1)
            .WillOnce(Return(response));

        ApiSquawkAllocation allocation = this->helper.GetAssignedSquawk("BAW123");
        EXPECT_TRUE("1234" == allocation.squawk);
        EXPECT_TRUE("BAW123" == allocation.callsign);
    }

    TEST_F(ApiHelperTest, GetSquawkAssignmentThrowsExceptionSquawkNotAllowed)
    {
        CurlResponse response(R"({"squawk": "7500"})", false, 200);

        EXPECT_CALL(
            this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_GET)))
            .Times(1)
            .WillOnce(Return(response));

        EXPECT_THROW(static_cast<void>(this->helper.GetAssignedSquawk("BAW123")), ApiException);
    }

    TEST_F(ApiHelperTest, GetSquawkAssignmentThrowsExceptionSquawkNotValid)
    {
        CurlResponse response(R"({"squawk": "abcd"})", false, 200);

        EXPECT_CALL(
            this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_GET)))
            .Times(1)
            .WillOnce(Return(response));

        EXPECT_THROW(static_cast<void>(this->helper.GetAssignedSquawk("BAW123")), ApiException);
    }

    TEST_F(ApiHelperTest, CreateGeneralSquawkAssignmentReturnsSquawk)
    {
        CurlResponse response(R"({"squawk": "1234"})", false, 200);
        nlohmann::json requestBody;
        requestBody["type"] = "general";
        requestBody["origin"] = "EGKK";
        requestBody["destination"] = "EGCC";

        EXPECT_CALL(
            this->mockCurlApi,
            MakeCurlRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_PUT, requestBody)))
            .Times(1)
            .WillOnce(Return(response));

        ApiSquawkAllocation allocation = this->helper.CreateGeneralSquawkAssignment("BAW123", "EGKK", "EGCC");
        EXPECT_TRUE("1234" == allocation.squawk);
        EXPECT_TRUE("BAW123" == allocation.callsign);
    }

    TEST_F(ApiHelperTest, CreateGeneralThrowsExceptionIfSquawkNotAllowed)
    {
        CurlResponse response(R"({"squawk": "7500"})", false, 200);
        nlohmann::json requestBody;
        requestBody["type"] = "general";
        requestBody["origin"] = "EGKK";
        requestBody["destination"] = "EGCC";

        EXPECT_CALL(
            this->mockCurlApi,
            MakeCurlRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_PUT, requestBody)))
            .Times(1)
            .WillOnce(Return(response));

        EXPECT_THROW(
            static_cast<void>(this->helper.CreateGeneralSquawkAssignment("BAW123", "EGKK", "EGCC")), ApiException);
    }

    TEST_F(ApiHelperTest, CreateGeneralThrowsExceptionIfNoSquawkInResponse)
    {
        CurlResponse response("{}", false, 200);
        nlohmann::json requestBody;
        requestBody["type"] = "general";
        requestBody["origin"] = "EGKK";
        requestBody["destination"] = "EGCC";

        EXPECT_CALL(
            this->mockCurlApi,
            MakeCurlRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_PUT, requestBody)))
            .Times(1)
            .WillOnce(Return(response));

        EXPECT_THROW(
            static_cast<void>(this->helper.CreateGeneralSquawkAssignment("BAW123", "EGKK", "EGCC")), ApiException);
    }

    TEST_F(ApiHelperTest, CreateGeneralThrowsExceptionIfSquawkInvalid)
    {
        CurlResponse response(R"({"squawk": "abcd"})", false, 200);
        nlohmann::json requestBody;
        requestBody["type"] = "general";
        requestBody["origin"] = "EGKK";
        requestBody["destination"] = "EGCC";

        EXPECT_CALL(
            this->mockCurlApi,
            MakeCurlRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_PUT, requestBody)))
            .Times(1)
            .WillOnce(Return(response));

        EXPECT_THROW(
            static_cast<void>(this->helper.CreateGeneralSquawkAssignment("BAW123", "EGKK", "EGCC")), ApiException);
    }

    TEST_F(ApiHelperTest, CreateLocalSquawkAssignmentReturnsSquawk)
    {
        CurlResponse response(R"({"squawk": "1234"})", false, 200);

        nlohmann::json requestBody;
        requestBody["type"] = "local";
        requestBody["rules"] = "V";
        requestBody["unit"] = "EGCC";

        EXPECT_CALL(
            this->mockCurlApi,
            MakeCurlRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_PUT, requestBody)))
            .Times(1)
            .WillOnce(Return(response));

        ApiSquawkAllocation allocation = this->helper.CreateLocalSquawkAssignment("BAW123", "EGCC", "V");
        EXPECT_TRUE("1234" == allocation.squawk);
        EXPECT_TRUE("BAW123" == allocation.callsign);
    }

    TEST_F(ApiHelperTest, CreateLocalSquawkThrowsExceptionIfSquawkNotAllowed)
    {
        CurlResponse response(R"({"squawk": "7700"})", false, 200);

        nlohmann::json requestBody;
        requestBody["type"] = "local";
        requestBody["rules"] = "V";
        requestBody["unit"] = "EGCC";

        EXPECT_CALL(
            this->mockCurlApi,
            MakeCurlRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_PUT, requestBody)))
            .Times(1)
            .WillOnce(Return(response));

        EXPECT_THROW(static_cast<void>(this->helper.CreateLocalSquawkAssignment("BAW123", "EGCC", "V")), ApiException);
    }

    TEST_F(ApiHelperTest, CreateLocalSquawkThrowsExceptionIfSquawkInvalid)
    {
        CurlResponse response(R"({"squawk": "abcd"})", false, 200);

        nlohmann::json requestBody;
        requestBody["type"] = "local";
        requestBody["rules"] = "V";
        requestBody["unit"] = "EGCC";

        EXPECT_CALL(
            this->mockCurlApi,
            MakeCurlRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_PUT, requestBody)))
            .Times(1)
            .WillOnce(Return(response));

        EXPECT_THROW(static_cast<void>(this->helper.CreateLocalSquawkAssignment("BAW123", "EGCC", "V")), ApiException);
    }

    TEST_F(ApiHelperTest, DeleteSquawkAssignmentIsCalledCorrectly)
    {
        CurlResponse response(R"({"squawk": "1234"})", false, 204);

        CurlRequest expectedRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_DELETE));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_NO_THROW(this->helper.DeleteSquawkAssignment("BAW123"));
    }

    TEST_F(ApiHelperTest, GetHoldDependencyReturnsJsonData)
    {
        nlohmann::json data;
        data["foo"] = "bar";
        data["big"] = "small";

        CurlResponse response(data.dump(), false, 200);
        CurlRequest expectedRequest(GetApiCurlRequest("/hold", CurlRequest::METHOD_GET));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_EQ(data, this->helper.GetHoldDependency());
    }

    TEST_F(ApiHelperTest, ItHasAUrlToSendTo)
    {
        EXPECT_TRUE(this->helper.GetApiDomain() == mockApiUrl);
    }

    TEST_F(ApiHelperTest, ItHasAnApiKeyToUseForAuthentication)
    {
        EXPECT_TRUE(this->helper.GetApiKey() == mockApiKey);
    }

    TEST_F(ApiHelperTest, ItCanUpdateTheUrl)
    {
        this->helper.SetApiDomain("https://nottheurl");
        EXPECT_TRUE(this->helper.GetApiDomain() == "https://nottheurl");
    }

    TEST_F(ApiHelperTest, ItCanUpdateTheKey)
    {
        this->helper.SetApiKey("notthekey");
        EXPECT_TRUE(this->helper.GetApiKey() == "notthekey");
    }

    TEST_F(ApiHelperTest, GetMinStackLevelsReturnsMinStackData)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        CurlRequest expectedRequest(GetApiCurlRequest("/msl", CurlRequest::METHOD_GET));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_EQ(responseData, this->helper.GetMinStackLevels());
    }

    TEST_F(ApiHelperTest, GetMinStackLevelsReturnsRegionalPressureData)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        CurlRequest expectedRequest(GetApiCurlRequest("/regional-pressure", CurlRequest::METHOD_GET));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_EQ(responseData, this->helper.GetRegionalPressures());
    }

    TEST_F(ApiHelperTest, GetUriReturnsUriData)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        CurlRequest expectedRequest(GetApiGetUriCurlRequest("http://ukcp.test.com/someuri", CurlRequest::METHOD_GET));
        expectedRequest.SetMaxRequestTime(0L);

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_EQ(responseData, this->helper.GetUri("http://ukcp.test.com/someuri"));
    }

    TEST_F(ApiHelperTest, GetUriThrowsExceptionIfNonUkcpRoute)
    {

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(_)).Times(0);

        EXPECT_THROW(static_cast<void>(this->helper.GetUri("http://ukcp.test.org/someuri")), ApiException);
    }

    TEST_F(ApiHelperTest, SearchSrdReturnsData)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        SrdSearchParameters params;
        params.origin = "EGLL";
        params.destination = "EGGD";
        params.requestedLevel = 10000;

        CurlRequest expectedRequest(GetApiGetUriCurlRequest(
            "http://ukcp.test.com/srd/route/search?origin=EGLL&destination=EGGD&requestedLevel=10000",
            CurlRequest::METHOD_GET));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_EQ(responseData, this->helper.SearchSrd(params));
    }

    TEST_F(ApiHelperTest, GetAssignedHoldsReturnsData)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        CurlRequest expectedRequest(
            GetApiGetUriCurlRequest("http://ukcp.test.com/hold/assigned", CurlRequest::METHOD_GET));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_EQ(responseData, this->helper.GetAssignedHolds());
    }

    TEST_F(ApiHelperTest, AssignAircraftToHoldGeneratesRequest)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        nlohmann::json requestBody;
        requestBody["callsign"] = "BAW123";
        requestBody["navaid"] = "TIMBA";

        CurlRequest expectedRequest(GetApiCurlRequest("/hold/assigned", CurlRequest::METHOD_PUT, requestBody));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_NO_THROW(this->helper.AssignAircraftToHold("BAW123", "TIMBA"));
    }

    TEST_F(ApiHelperTest, UnassignAircraftHoldGeneratesRequest)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        CurlRequest expectedRequest(GetApiCurlRequest("/hold/assigned/BAW123", CurlRequest::METHOD_DELETE));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_NO_THROW(this->helper.UnassignAircraftHold("BAW123"));
    }

    TEST_F(ApiHelperTest, EnrouteReleaseGeneratesRequest)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        CurlRequest expectedRequest = GetApiCurlRequest(
            "/release/enroute",
            CurlRequest::METHOD_POST,
            {
                {"callsign", "BAW123"},
                {"type", 1},
                {"initiating_controller", "LON_S_CTR"},
                {"target_controller", "LON_C_CTR"},
            });

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_NO_THROW(this->helper.SendEnrouteRelease("BAW123", "LON_S_CTR", "LON_C_CTR", 1));
    }

    TEST_F(ApiHelperTest, EnrouteReleaseGeneratesRequestWithReleasePoint)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        CurlRequest expectedRequest = GetApiCurlRequest(
            "/release/enroute",
            CurlRequest::METHOD_POST,
            {{"callsign", "BAW123"},
             {"type", 1},
             {"initiating_controller", "LON_S_CTR"},
             {"target_controller", "LON_C_CTR"},
             {"release_point", "LAM"}});

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_NO_THROW(this->helper.SendEnrouteReleaseWithReleasePoint("BAW123", "LON_S_CTR", "LON_C_CTR", 1, "LAM"));
    }

    TEST_F(ApiHelperTest, GetAssignedStandsReturnsData)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        CurlRequest expectedRequest(
            GetApiGetUriCurlRequest("http://ukcp.test.com/stand/assignment", CurlRequest::METHOD_GET));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_EQ(responseData, this->helper.GetAssignedStands());
    }

    TEST_F(ApiHelperTest, AssignStandToAircraftGeneratesRequest)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        nlohmann::json requestBody;
        requestBody["callsign"] = "BAW123";
        requestBody["stand_id"] = 1;

        CurlRequest expectedRequest(GetApiCurlRequest("/stand/assignment", CurlRequest::METHOD_PUT, requestBody));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_NO_THROW(this->helper.AssignStandToAircraft("BAW123", 1));
    }

    TEST_F(ApiHelperTest, DeleteStandAssignmentForAircraftGeneratesRequest)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        CurlRequest expectedRequest(GetApiCurlRequest("/stand/assignment/BAW123", CurlRequest::METHOD_DELETE));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_NO_THROW(this->helper.DeleteStandAssignmentForAircraft("BAW123"));
    }

    TEST_F(ApiHelperTest, GetAllNotificationsMakesRequest)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        CurlRequest expectedRequest(GetApiCurlRequest("/notifications", CurlRequest::METHOD_GET));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_EQ(responseData, this->helper.GetAllNotifications());
    }

    TEST_F(ApiHelperTest, GetUnreadNotificationsMakesRequest)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        CurlRequest expectedRequest(GetApiCurlRequest("/notifications/unread", CurlRequest::METHOD_GET));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_EQ(responseData, this->helper.GetUnreadNotifications());
    }

    TEST_F(ApiHelperTest, ReadNotificationMakesRequest)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        CurlRequest expectedRequest(GetApiCurlRequest("/notifications/read/1", CurlRequest::METHOD_PUT));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_NO_THROW(this->helper.ReadNotification(1));
    }

    TEST_F(ApiHelperTest, SyncPluginEventsMakesRequest)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        CurlRequest expectedRequest(GetApiCurlRequest("/plugin-events/sync", CurlRequest::METHOD_GET));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_EQ(responseData, this->helper.SyncPluginEvents());
    }

    TEST_F(ApiHelperTest, GetLatestPluginEventsMakesRequest)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        CurlRequest expectedRequest(GetApiCurlRequest("/plugin-events/recent?previous=5", CurlRequest::METHOD_GET));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_EQ(responseData, this->helper.GetLatestPluginEvents(5));
    }

    TEST_F(ApiHelperTest, GetUpdateDetailsReturnsData)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        CurlRequest expectedRequest(
            GetApiGetUriCurlRequest("http://ukcp.test.com/version/latest", CurlRequest::METHOD_GET));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_EQ(responseData, this->helper.GetUpdateDetails());
    }

    TEST_F(ApiHelperTest, RequestDepartureReleaseMakesRequest)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        nlohmann::json expectedData;
        expectedData["callsign"] = "BAW123";
        expectedData["requesting_controller_id"] = 1;
        expectedData["target_controller_id"] = 3;
        expectedData["expires_in_seconds"] = 54;

        CurlRequest expectedRequest(
            GetApiCurlRequest("/departure/release/request", CurlRequest::METHOD_POST, expectedData));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_EQ(responseData, this->helper.RequestDepartureRelease("BAW123", 1, 3, 54));
    }

    TEST_F(ApiHelperTest, ApproveDepartureReleaseMakesRequest)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        nlohmann::json expectedData;
        expectedData["controller_position_id"] = 2;
        expectedData["released_at"] = "2021-05-09 12:31:00";
        expectedData["expires_in_seconds"] = 120;

        std::chrono::system_clock::time_point timePoint;
        std::istringstream inputStream("2021-05-09 12:31:00");
        inputStream >> date::parse("%Y-%m-%d %H:%M:%S", timePoint);

        CurlRequest expectedRequest(
            GetApiCurlRequest("/departure/release/request/1/approve", CurlRequest::METHOD_PATCH, expectedData));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        this->helper.ApproveDepartureReleaseRequest(1, 2, timePoint, 120);
    }

    TEST_F(ApiHelperTest, RejectDepartureReleaseMakesRequest)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        nlohmann::json expectedData;
        expectedData["controller_position_id"] = 2;

        CurlRequest expectedRequest(
            GetApiCurlRequest("/departure/release/request/1/reject", CurlRequest::METHOD_PATCH, expectedData));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        this->helper.RejectDepartureReleaseRequest(1, 2);
    }

    TEST_F(ApiHelperTest, AcknowledgeDepartureReleaseMakesRequest)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        nlohmann::json expectedData;
        expectedData["controller_position_id"] = 2;

        CurlRequest expectedRequest(
            GetApiCurlRequest("/departure/release/request/1/acknowledge", CurlRequest::METHOD_PATCH, expectedData));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        this->helper.AcknowledgeDepartureReleaseRequest(1, 2);
    }

    TEST_F(ApiHelperTest, CancelDepartureReleaseMakesRequest)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        CurlRequest expectedRequest(GetApiCurlRequest("/departure/release/request/1", CurlRequest::METHOD_DELETE));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        this->helper.CancelDepartureReleaseRequest(1);
    }

    TEST_F(ApiHelperTest, CreatePrenoteMessageRequestMakesRequest)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        nlohmann::json expectedData;
        expectedData["callsign"] = "BAW123";
        expectedData["departure_airfield"] = "EGGD";
        expectedData["departure_sid"] = "BADIM1X";
        expectedData["destination_airfield"] = "EGLC";
        expectedData["requesting_controller_id"] = 1;
        expectedData["target_controller_id"] = 2;
        expectedData["expires_in_seconds"] = 55;

        CurlRequest expectedRequest(GetApiCurlRequest("/prenotes/messages", CurlRequest::METHOD_POST, expectedData));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_EQ(responseData, this->helper.CreatePrenoteMessage("BAW123", "EGGD", "BADIM1X", "EGLC", 1, 2, 55));
    }

    TEST_F(ApiHelperTest, AcknowledgePrenoteMessageMakesRequest)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        nlohmann::json expectedData;
        expectedData["controller_position_id"] = 2;

        CurlRequest expectedRequest(
            GetApiCurlRequest("/prenotes/messages/55/acknowledge", CurlRequest::METHOD_PATCH, expectedData));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        this->helper.AcknowledgePrenoteMessage(55, 2);
    }

    TEST_F(ApiHelperTest, DeletePrenoteMessageMakesRequest)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        CurlRequest expectedRequest(GetApiCurlRequest("/prenotes/messages/55", CurlRequest::METHOD_DELETE));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        this->helper.DeletePrenoteMessage(55);
    }

    TEST_F(ApiHelperTest, CreateMissedApproachMakesRequest)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);

        nlohmann::json expectedData;
        expectedData["callsign"] = "BAW123";

        CurlRequest expectedRequest(GetApiCurlRequest("/missed-approaches", CurlRequest::METHOD_POST, expectedData));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_EQ(responseData, this->helper.CreateMissedApproach("BAW123"));
    }
  
    TEST_F(ApiHelperTest, GetAllMetarsMakesRequest)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);
      
        CurlRequest expectedRequest(GetApiCurlRequest("/metar", CurlRequest::METHOD_GET));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        EXPECT_EQ(responseData, this->helper.GetAllMetars());
    }
  
    TEST_F(ApiHelperTest, AcknowledgeMissedApproachMakesRequest)
    {
        nlohmann::json responseData;
        responseData["bla"] = "bla";
        CurlResponse response(responseData.dump(), false, 200);
      
        nlohmann::json expectedData;
        expectedData["remarks"] = "Some remarks";

        CurlRequest expectedRequest(GetApiCurlRequest("/missed-approaches/1", CurlRequest::METHOD_PATCH, expectedData));

        EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest)).Times(1).WillOnce(Return(response));

        this->helper.AcknowledgeMissedApproach(1, "Some remarks");
    }
} // namespace UKControllerPluginUtilsTest::Api
