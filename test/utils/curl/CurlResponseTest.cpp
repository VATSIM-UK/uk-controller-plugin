#include "pch/pch.h"
#include "curl/CurlResponse.h"

using UKControllerPlugin::Curl::CurlResponse;

namespace UKControllerPluginUtilsTest
{
    namespace Curl {

        TEST(CurlResponse, GetResponseReturnsResponse)
        {
            CurlResponse response("TestResponse", false, 200);
            EXPECT_TRUE(response.GetResponse() == "TestResponse");
        }

        TEST(CurlResponse, GetStatusCodeReturnsStatusCode)
        {
            CurlResponse response("TestResponse", false, 200);
            EXPECT_TRUE(response.GetStatusCode() == 200);
        }

        TEST(CurlResponse, StatusOkReturnsTrueIfOk)
        {
            CurlResponse response("TestResponse", false, 200);
            EXPECT_TRUE(response.StatusOk());
        }

        TEST(CurlResponse, StatusOkReturnsFalseIfNotOk)
        {
            CurlResponse response("TestResponse", false, 403);
            EXPECT_FALSE(response.StatusOk());
        }

        TEST(CurlResponse, IsCurlErrorReturnsError)
        {
            CurlResponse response1("TestResponse", false, 403);
            CurlResponse response2("TestResponse", true, 403);

            EXPECT_FALSE(response1.IsCurlError());
            EXPECT_TRUE(response2.IsCurlError());
        }
    }  // namespace Curl
} // namespace UKControllerPluginUtilsTest
