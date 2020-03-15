#include "pch/pch.h"
#include "api/ApiHelper.h"
#include "curl/CurlInterface.h"
#include "api/ApiException.h"
#include "mock/MockCurlApi.h"
#include "api/ApiResponse.h"
#include "curl/CurlRequest.h"
#include "helper/ApiRequestHelperFunctions.h"
#include "api/ApiNotFoundException.h"
#include "api/ApiNotAuthorisedException.h"
#include "mock/MockWinApi.h"
#include "squawk/ApiSquawkAllocation.h"

using UKControllerPlugin::Api::ApiHelper;
using UKControllerPlugin::Api::ApiResponse;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Curl::CurlResponse;
using UKControllerPlugin::Curl::CurlInterface;
using UKControllerPluginTest::Curl::MockCurlApi;
using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Api::ApiRequestBuilder;
using UKControllerPlugin::Api::ApiNotFoundException;
using UKControllerPlugin::Api::ApiNotAuthorisedException;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPlugin::Squawk::ApiSquawkAllocation;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::_;

namespace UKControllerPluginTest {
namespace Api {

class ApiHelperTest : public Test
{
    public:

        ApiHelperTest()
            : helper(mockCurlApi, GetApiRequestBuilder(), mockWinApi)
        {

        }

        ApiHelper helper;
        NiceMock<MockWinApi> mockWinApi;
        NiceMock<MockCurlApi> mockCurlApi;
};

TEST_F(ApiHelperTest, TestItThrowsNotFoundExceptionIf404)
{
    CurlResponse response("{\"version_disabled\": false, \"update_available\": false}", false, 404);

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/version/1.0.0/status", CurlRequest::METHOD_GET)))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_THROW(this->helper.UpdateCheck("1.0.0"), ApiNotFoundException);
}

TEST_F(ApiHelperTest, TestItThrowsNotAuthorisedExceptionIf401)
{
    CurlResponse response("{\"version_disabled\": false, \"update_available\": false}", false, 401);

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/version/1.0.0/status", CurlRequest::METHOD_GET)))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_THROW(this->helper.UpdateCheck("1.0.0"), ApiNotAuthorisedException);
}

TEST_F(ApiHelperTest, TestItThrowsNotAuthorisedExceptionIf403)
{
    CurlResponse response("{\"version_disabled\": false, \"update_available\": false}", false, 401);

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/version/1.0.0/status", CurlRequest::METHOD_GET)))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_THROW(this->helper.UpdateCheck("1.0.0"), ApiNotAuthorisedException);
}

TEST_F(ApiHelperTest, TestItThrowsApiExceptionIfServiceUnavailable)
{
    CurlResponse response("{\"version_disabled\": false, \"update_available\": false}", false, 503);

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/version/1.0.0/status", CurlRequest::METHOD_GET)))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_THROW(this->helper.UpdateCheck("1.0.0"), ApiException);
}

TEST_F(ApiHelperTest, TestItThrowsApiExceptionIfServerError)
{
    CurlResponse response("{\"version_disabled\": false, \"update_available\": false}", false, 500);

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/version/1.0.0/status", CurlRequest::METHOD_GET)))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_THROW(this->helper.UpdateCheck("1.0.0"), ApiException);
}

TEST_F(ApiHelperTest, ItThrowsAnExceptionIfBadRequest)
{
    CurlResponse response("{\"version_disabled\": false, \"update_available\": false}", false, 401);

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/version/1.0.0/status", CurlRequest::METHOD_GET)))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_THROW(this->helper.UpdateCheck("1.0.0"), ApiException);
}

TEST_F(ApiHelperTest, ItThrowsAnExceptionIfUnknownResponseCode)
{
    CurlResponse response("{\"version_disabled\": false, \"update_available\": false}", false, 666);

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/version/1.0.0/status", CurlRequest::METHOD_GET)))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_THROW(this->helper.UpdateCheck("1.0.0"), ApiException);
}

TEST_F(ApiHelperTest, UpdateCheckReturnsOkIfVersionOk)
{
    CurlResponse response("{\"version_disabled\": false, \"update_available\": false}", false, 200);

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/version/1.0.0/status", CurlRequest::METHOD_GET)))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_EQ(ApiHelper::UPDATE_UP_TO_DATE, this->helper.UpdateCheck("1.0.0"));
}

TEST_F(ApiHelperTest, UpdateCheckReturnsUpdateAvailableWhenUpdateAvailable)
{
    CurlResponse response("{\"version_disabled\": false, \"update_available\": true}", false, 200);

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/version/1.0.0/status", CurlRequest::METHOD_GET)))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_EQ(ApiHelper::UPDATE_VERSION_NEEDS_UPDATE, this->helper.UpdateCheck("1.0.0"));
}

TEST_F(ApiHelperTest, UpdateCheckReturnsVersionDisabledIfDisabled)
{
    CurlResponse response("{\"version_disabled\": true, \"update_available\": false}", false, 200);

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(GetApiCurlRequest("/version/1.0.0/status", CurlRequest::METHOD_GET)))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_EQ(ApiHelper::UPDATE_VERSION_DISABLED, this->helper.UpdateCheck("1.0.0"));
}

TEST_F(ApiHelperTest, FetchRemoteFileReturnsFileString)
{
    nlohmann::json responseJson;
    responseJson["test"] = "hi!";
    CurlResponse response(responseJson.dump(), false, 200);

    EXPECT_CALL(
            this->mockCurlApi,
            MakeCurlRequest(CurlRequest("http://test.com/averynicefile", CurlRequest::METHOD_GET))
        )
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_TRUE(responseJson.dump() == this->helper.FetchRemoteFile("http://test.com/averynicefile"));
}

TEST_F(ApiHelperTest, GetSquawkAssignmentHandlesNonJsonResponse)
{
    CurlResponse response("<html>here is some html that means something went wrong</html>", false, 200);

    EXPECT_CALL(
        this->mockCurlApi,
        MakeCurlRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_GET))
    )
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_THROW(this->helper.GetAssignedSquawk("BAW123"), ApiException);
}

TEST_F(ApiHelperTest, GetSquawkAssignmentReturnsSquawkAllocation)
{
    CurlResponse response("{\"squawk\": \"1234\"}", false, 200);

    EXPECT_CALL(
            this->mockCurlApi,
            MakeCurlRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_GET))
        )
        .Times(1)
        .WillOnce(Return(response));

    ApiSquawkAllocation allocation = this->helper.GetAssignedSquawk("BAW123");
    EXPECT_TRUE("1234" == allocation.squawk);
    EXPECT_TRUE("BAW123" == allocation.callsign);
}

TEST_F(ApiHelperTest, GetSquawkAssignmentThrowsExceptionSquawkNotAllowed)
{
    CurlResponse response("{\"squawk\": \"7500\"}", false, 200);

    EXPECT_CALL(
            this->mockCurlApi,
            MakeCurlRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_GET))
        )
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_THROW(this->helper.GetAssignedSquawk("BAW123"), ApiException);
}

TEST_F(ApiHelperTest, GetSquawkAssignmentThrowsExceptionSquawkNotValid)
{
    CurlResponse response("{\"squawk\": \"abcd\"}", false, 200);

    EXPECT_CALL(
            this->mockCurlApi,
            MakeCurlRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_GET))
        )
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_THROW(this->helper.GetAssignedSquawk("BAW123"), ApiException);
}

TEST_F(ApiHelperTest, CreateGeneralSquawkAssignmentReturnsSquawk)
{
    CurlResponse response("{\"squawk\": \"1234\"}", false, 200);
    nlohmann::json requestBody;
    requestBody["type"] = "general";
    requestBody["origin"] = "EGKK";
    requestBody["destination"] = "EGCC";

    CurlRequest r1 = GetApiRequestBuilder().BuildGeneralSquawkAssignmentRequest("BAW123", "EGKK", "EGCC");
    CurlRequest r2 = GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_PUT, requestBody);
    bool eq = r1 == r2;

    EXPECT_CALL(
            this->mockCurlApi,
            MakeCurlRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_PUT, requestBody))
        )
        .Times(1)
        .WillOnce(Return(response));

    ApiSquawkAllocation allocation = this->helper.CreateGeneralSquawkAssignment("BAW123", "EGKK", "EGCC");
    EXPECT_TRUE("1234" == allocation.squawk);
    EXPECT_TRUE("BAW123" == allocation.callsign);
}

TEST_F(ApiHelperTest, CreateGeneralThrowsExceptionIfSquawkNotAllowed)
{
    CurlResponse response("{\"squawk\": \"7500\"}", false, 200);
    nlohmann::json requestBody;
    requestBody["type"] = "general";
    requestBody["origin"] = "EGKK";
    requestBody["destination"] = "EGCC";

    CurlRequest r1 = GetApiRequestBuilder().BuildGeneralSquawkAssignmentRequest("BAW123", "EGKK", "EGCC");
    CurlRequest r2 = GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_PUT, requestBody);
    bool eq = r1 == r2;

    EXPECT_CALL(
            this->mockCurlApi,
            MakeCurlRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_PUT, requestBody))
        )
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_THROW(this->helper.CreateGeneralSquawkAssignment("BAW123", "EGKK", "EGCC"), ApiException);
}

TEST_F(ApiHelperTest, CreateGeneralThrowsExceptionIfNoSquawkInResponse)
{
    CurlResponse response("{}", false, 200);
    nlohmann::json requestBody;
    requestBody["type"] = "general";
    requestBody["origin"] = "EGKK";
    requestBody["destination"] = "EGCC";

    CurlRequest r1 = GetApiRequestBuilder().BuildGeneralSquawkAssignmentRequest("BAW123", "EGKK", "EGCC");
    CurlRequest r2 = GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_PUT, requestBody);

    EXPECT_CALL(
        this->mockCurlApi,
        MakeCurlRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_PUT, requestBody))
    )
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_THROW(this->helper.CreateGeneralSquawkAssignment("BAW123", "EGKK", "EGCC"), ApiException);
}

TEST_F(ApiHelperTest, CreateGeneralThrowsExceptionIfSquawkInvalid)
{
    CurlResponse response("{\"squawk\": \"abcd\"}", false, 200);
    nlohmann::json requestBody;
    requestBody["type"] = "general";
    requestBody["origin"] = "EGKK";
    requestBody["destination"] = "EGCC";

    CurlRequest r1 = GetApiRequestBuilder().BuildGeneralSquawkAssignmentRequest("BAW123", "EGKK", "EGCC");
    CurlRequest r2 = GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_PUT, requestBody);
    bool eq = r1 == r2;

    EXPECT_CALL(
            this->mockCurlApi,
            MakeCurlRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_PUT, requestBody))
        )
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_THROW(this->helper.CreateGeneralSquawkAssignment("BAW123", "EGKK", "EGCC"), ApiException);
}

TEST_F(ApiHelperTest, CreateLocalSquawkAssignmentReturnsSquawk)
{
    CurlResponse response("{\"squawk\": \"1234\"}", false, 200);

    nlohmann::json requestBody;
    requestBody["type"] = "local";
    requestBody["rules"] = "V";
    requestBody["unit"] = "EGCC";

    EXPECT_CALL(
            this->mockCurlApi,
            MakeCurlRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_PUT, requestBody))
        )
        .Times(1)
        .WillOnce(Return(response));

    ApiSquawkAllocation allocation = this->helper.CreateLocalSquawkAssignment("BAW123", "EGCC", "V");
    EXPECT_TRUE("1234" == allocation.squawk);
    EXPECT_TRUE("BAW123" == allocation.callsign);
}

TEST_F(ApiHelperTest, CreateLocalSquawkThrowsExceptionIfSquawkNotAllowed)
{
    CurlResponse response("{\"squawk\": \"7700\"}", false, 200);

    nlohmann::json requestBody;
    requestBody["type"] = "local";
    requestBody["rules"] = "V";
    requestBody["unit"] = "EGCC";

    EXPECT_CALL(
            this->mockCurlApi,
            MakeCurlRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_PUT, requestBody))
        )
        .Times(1)
        .WillOnce(Return(response));


    EXPECT_THROW(this->helper.CreateLocalSquawkAssignment("BAW123", "EGCC", "V"), ApiException);
}


TEST_F(ApiHelperTest, CreateLocalSquawkThrowsExceptionIfSquawkInvalid)
{
    CurlResponse response("{\"squawk\": \"abcd\"}", false, 200);

    nlohmann::json requestBody;
    requestBody["type"] = "local";
    requestBody["rules"] = "V";
    requestBody["unit"] = "EGCC";

    EXPECT_CALL(
            this->mockCurlApi,
            MakeCurlRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_PUT, requestBody))
        )
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_THROW(this->helper.CreateLocalSquawkAssignment("BAW123", "EGCC", "V"), ApiException);
}

TEST_F(ApiHelperTest, DeleteSquawkAssignmentIsCalledCorrectly)
{
    CurlResponse response("{\"squawk\": \"1234\"}", false, 204);

    CurlRequest expectedRequest(GetApiCurlRequest("/squawk-assignment/BAW123", CurlRequest::METHOD_DELETE));

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_NO_THROW(this->helper.DeleteSquawkAssignment("BAW123"));
}

TEST_F(ApiHelperTest, GetHoldDependencyReturnsJsonData)
{
    nlohmann::json data;
    data["foo"] = "bar";
    data["big"] = "small";

    CurlResponse response(data.dump(), false, 200);
    CurlRequest expectedRequest(GetApiCurlRequest("/hold", CurlRequest::METHOD_GET));

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_EQ(data, this->helper.GetHoldDependency());
}

TEST_F(ApiHelperTest, GetGenericHoldProfilesReturnsReturnsData)
{
    nlohmann::json data;
    data["foo"] = "bar";
    data["big"] = "small";

    CurlResponse response(data.dump(), false, 200);
    CurlRequest expectedRequest(GetApiCurlRequest("/hold/profile", CurlRequest::METHOD_GET));

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_EQ(data, this->helper.GetGenericHoldProfiles());
}

TEST_F(ApiHelperTest, GetUserHoldProfilesReturnsReturnsData)
{
    nlohmann::json data;
    data["foo"] = "bar";
    data["big"] = "small";

    CurlResponse response(data.dump(), false, 200);
    CurlRequest expectedRequest(GetApiCurlRequest("/hold/profile", CurlRequest::METHOD_GET));

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_EQ(data, this->helper.GetUserHoldProfiles());
}

TEST_F(ApiHelperTest, DeleteUserHoldProfileMakesRequest)
{
    nlohmann::json data;
    data["foo"] = "bar";
    data["big"] = "small";

    CurlResponse response(data.dump(), false, 200);
    CurlRequest expectedRequest(GetApiCurlRequest("/hold/profile/1", CurlRequest::METHOD_DELETE));

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_NO_THROW(this->helper.DeleteUserHoldProfile(1));
}

TEST_F(ApiHelperTest, CreateUserHoldProfileMakesRequest)
{
    nlohmann::json data;
    data["id"] = 3;

    CurlResponse response(data.dump(), false, 200);

    nlohmann::json expectedBody;
    expectedBody["name"] = "Test";
    expectedBody["holds"] = { 1, 2 };
    CurlRequest expectedRequest(GetApiCurlRequest("/hold/profile", CurlRequest::METHOD_PUT, expectedBody));

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_EQ(3, this->helper.CreateUserHoldProfile("Test", { 1, 2 }));
}

TEST_F(ApiHelperTest, CreateUserHoldProfileThrowsExceptionIdNotANumber)
{
    nlohmann::json data;
    data["id"] = "test";

    CurlResponse response(data.dump(), false, 200);

    nlohmann::json expectedBody;
    expectedBody["name"] = "Test";
    expectedBody["holds"] = { 1, 2 };
    CurlRequest expectedRequest(GetApiCurlRequest("/hold/profile", CurlRequest::METHOD_PUT, expectedBody));

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_THROW(this->helper.CreateUserHoldProfile("Test", { 1, 2 }), ApiException);
}

TEST_F(ApiHelperTest, CreateUserHoldProfileThrowsExceptionNoIdReturned)
{
    nlohmann::json data;

    CurlResponse response(data.dump(), false, 200);

    nlohmann::json expectedBody;
    expectedBody["name"] = "Test";
    expectedBody["holds"] = { 1, 2 };
    CurlRequest expectedRequest(GetApiCurlRequest("/hold/profile", CurlRequest::METHOD_PUT, expectedBody));

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_THROW(this->helper.CreateUserHoldProfile("Test", { 1, 2 }), ApiException);
}


TEST_F(ApiHelperTest, UpdateUserHoldProfileMakesRequest)
{
    nlohmann::json data;
    data["foo"] = "bar";
    data["big"] = "small";

    CurlResponse response(data.dump(), false, 200);

    nlohmann::json expectedBody;
    expectedBody["name"] = "Test";
    expectedBody["holds"] = { 1, 2 };
    CurlRequest expectedRequest(GetApiCurlRequest("/hold/profile/1", CurlRequest::METHOD_PUT, expectedBody));

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_NO_THROW(this->helper.UpdateUserHoldProfile(1, "Test", { 1, 2 }));
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

TEST_F(ApiHelperTest, AuthoriseWebsocketChannelReturnsTheAuthCode)
{
    nlohmann::json responseData;
    responseData["auth"] = "someauthcode";

    CurlResponse response(responseData.dump(), false, 200);
    CurlRequest expectedRequest(
        GetApiCurlRequest(
            "/broadcasting/auth",
            CurlRequest::METHOD_POST,
            { {"socket_id", "somesocket"}, {"channel_name", "somechannel"} }
        )
    );

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_EQ("someauthcode", this->helper.AuthoriseWebsocketChannel("somesocket", "somechannel"));
}

TEST_F(ApiHelperTest, AuthoriseWebsocketChannelThrowsExceptionIfAuthCodeMissing)
{
    nlohmann::json responseData;

    CurlResponse response(responseData.dump(), false, 200);
    CurlRequest expectedRequest(
        GetApiCurlRequest(
            "/broadcasting/auth",
            CurlRequest::METHOD_POST,
            { {"socket_id", "somesocket"}, {"channel_name", "somechannel"} }
        )
    );

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_THROW(this->helper.AuthoriseWebsocketChannel("somesocket", "somechannel"), ApiException);
}

TEST_F(ApiHelperTest, AuthoriseWebsocketChannelThrowsExceptionIfAuthCodeNotString)
{
    nlohmann::json responseData;
    responseData["auth"] = 12345;

    CurlResponse response(responseData.dump(), false, 200);
    CurlRequest expectedRequest(
        GetApiCurlRequest(
            "/broadcasting/auth",
            CurlRequest::METHOD_POST,
            { {"socket_id", "somesocket"}, {"channel_name", "somechannel"} }
        )
    );

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_THROW(this->helper.AuthoriseWebsocketChannel("somesocket", "somechannel"), ApiException);
}


TEST_F(ApiHelperTest, AuthoriseWebsocketChannelThrowsExceptionIfNotAuthorised)
{
    nlohmann::json responseData;
    responseData["auth"] = "someauthcode";

    CurlResponse response(responseData.dump(), false, 403);
    CurlRequest expectedRequest(
        GetApiCurlRequest(
            "/broadcasting/auth",
            CurlRequest::METHOD_POST,
            { {"socket_id", "somesocket"}, {"channel_name", "somechannel"} }
        )
    );

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_THROW(this->helper.AuthoriseWebsocketChannel("somesocket", "somechannel"), ApiNotAuthorisedException);
}

TEST_F(ApiHelperTest, GetMinStackLevelsReturnsMinStackData)
{
    nlohmann::json responseData;
    responseData["bla"] = "bla";
    CurlResponse response(responseData.dump(), false, 200);

    CurlRequest expectedRequest(
        GetApiCurlRequest(
            "/msl",
            CurlRequest::METHOD_GET
        )
    );

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_EQ(responseData, this->helper.GetMinStackLevels());
}

TEST_F(ApiHelperTest, GetMinStackLevelsReturnsRegionalPressureData)
{
    nlohmann::json responseData;
    responseData["bla"] = "bla";
    CurlResponse response(responseData.dump(), false, 200);

    CurlRequest expectedRequest(
        GetApiCurlRequest(
        "/regional-pressure",
        CurlRequest::METHOD_GET
    )
    );

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_EQ(responseData, this->helper.GetRegionalPressures());
}

TEST_F(ApiHelperTest, GetUriReturnsUriData)
{
    nlohmann::json responseData;
    responseData["bla"] = "bla";
    CurlResponse response(responseData.dump(), false, 200);

    CurlRequest expectedRequest(
        GetApiGetUriCurlRequest(
            "http://ukcp.test.com/someuri",
            CurlRequest::METHOD_GET
        )
    );

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(expectedRequest))
        .Times(1)
        .WillOnce(Return(response));

    EXPECT_EQ(responseData, this->helper.GetUri("http://ukcp.test.com/someuri"));
}

TEST_F(ApiHelperTest, GetUriThrowsExceptionIfNonUkcpRoute)
{

    EXPECT_CALL(this->mockCurlApi, MakeCurlRequest(_))
        .Times(0);

    EXPECT_THROW(this->helper.GetUri("http://ukcp.test.org/someuri"), ApiException);
}
}  // namespace Api
}  // namespace UKControllerPluginTest
