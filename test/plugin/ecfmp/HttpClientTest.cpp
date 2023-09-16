#include "curl/CurlResponse.h"
#include "ecfmp/HttpClient.h"

namespace UKControllerPluginTest::ECFMP {
    class HttpClientTest : public testing::Test
    {
        public:
        HttpClientTest() : client(mockCurlApi)
        {
        }

        testing::NiceMock<Curl::MockCurlApi> mockCurlApi;
        UKControllerPlugin::ECFMP::HttpClient client;
    };

    TEST_F(HttpClientTest, GetReturnsCurlErrorStatusWhenCurlRequestFails)
    {
        EXPECT_CALL(mockCurlApi, MakeCurlRequest(testing::_))
            .WillOnce(testing::Return(UKControllerPlugin::Curl::CurlResponse{"", true, 0L}));
        const auto response = client.Get("http://www.example.com");
        EXPECT_EQ(999, response.statusCode);
        EXPECT_EQ("", response.body);
    }

    TEST_F(HttpClientTest, GetReturnsCurlResponse)
    {
        EXPECT_CALL(mockCurlApi, MakeCurlRequest(testing::_))
            .WillOnce(testing::Return(UKControllerPlugin::Curl::CurlResponse{"response body", false, 418L}));
        const auto response = client.Get("http://www.example.com");
        EXPECT_EQ("response body", response.body);
        EXPECT_EQ(418L, response.statusCode);
    }
} // namespace UKControllerPluginTest::ECFMP
