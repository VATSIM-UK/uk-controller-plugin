#include "api/Response.h"

using UKControllerPluginUtils::Api::Response;
using UKControllerPluginUtils::Http::HttpStatusCode;

namespace UKControllerPluginUtilsTest::Api {
    class ResponseTest : public testing::Test
    {
        public:
        ResponseTest() : response(HttpStatusCode::Ok, {{"foo", "bar"}})
        {
        }

        Response response;
    };

    TEST_F(ResponseTest, ItHasAStatusCode)
    {
        EXPECT_EQ(HttpStatusCode::Ok, response.StatusCode());
    }

    TEST_F(ResponseTest, ItHasData)
    {
        EXPECT_EQ(nlohmann::json({{"foo", "bar"}}), response.Data());
    }
} // namespace UKControllerPluginUtilsTest::Api
