#include "api/ApiRequestException.h"

using UKControllerPluginUtils::Api::ApiRequestException;
using UKControllerPluginUtils::Http::HttpStatusCode;

namespace UKControllerPluginUtilsTest::Api {
    class ApiRequestExceptionTest : public testing::Test
    {
        public:
        ApiRequestExceptionTest() : exception("testuri", HttpStatusCode::ServerError, false)
        {
        }

        ApiRequestException exception;
    };

    TEST_F(ApiRequestExceptionTest, ItHasAUri)
    {
        EXPECT_EQ("testuri", exception.Uri());
    }

    TEST_F(ApiRequestExceptionTest, ItHasAStatusCode)
    {
        EXPECT_EQ(HttpStatusCode::ServerError, exception.StatusCode());
    }

    TEST_F(ApiRequestExceptionTest, ItHasWhetherJsonWasInvalid)
    {
        EXPECT_FALSE(exception.InvalidJson());
    }

    TEST_F(ApiRequestExceptionTest, ItHasAnExplanation)
    {
        EXPECT_EQ("Api request resulted in status " + std::to_string(500), exception.what());
    }
} // namespace UKControllerPluginUtilsTest::Api
