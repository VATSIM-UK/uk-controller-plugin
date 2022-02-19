#include "http/HttpStatusCode.h"

using UKControllerPluginUtils::Http::HttpStatusCode;
using UKControllerPluginUtils::Http::IsAuthenticationError;
using UKControllerPluginUtils::Http::IsServerError;
using UKControllerPluginUtils::Http::IsSuccessful;

namespace UKControllerPluginUtilsTest::Http {
    class HttpStatusCodeTest : public testing::Test
    {
    };

    TEST_F(HttpStatusCodeTest, IsEqualToUnsignedIntIfMatchesBackedValue)
    {
        EXPECT_TRUE(200L == HttpStatusCode::Ok);
    }

    TEST_F(HttpStatusCodeTest, IsNotEqualToUnsignedIntIfDoesntMatcheBackedValue)
    {
        EXPECT_FALSE(400L == HttpStatusCode::Ok);
    }

    TEST_F(HttpStatusCodeTest, ItIsAnAuthenticationErrorIfNotAuthorised)
    {
        EXPECT_TRUE(IsAuthenticationError(HttpStatusCode::Unauthorised));
    }

    TEST_F(HttpStatusCodeTest, ItIsAnAuthenticationErrorIfForbidden)
    {
        EXPECT_TRUE(IsAuthenticationError(HttpStatusCode::Forbidden));
    }

    TEST_F(HttpStatusCodeTest, ItIsNotAnAuthenticationErrorIfOk)
    {
        EXPECT_FALSE(IsAuthenticationError(HttpStatusCode::Ok));
    }

    TEST_F(HttpStatusCodeTest, ItIsAServerErrorIfServerError)
    {
        EXPECT_TRUE(IsServerError(HttpStatusCode::ServerError));
    }

    TEST_F(HttpStatusCodeTest, ItIsAServerErrorIfBadGateway)
    {
        EXPECT_TRUE(IsServerError(HttpStatusCode::BadGateway));
    }

    TEST_F(HttpStatusCodeTest, ItIsAServerErrorIfUnknown)
    {
        EXPECT_TRUE(IsServerError(HttpStatusCode::Unknown));
    }

    TEST_F(HttpStatusCodeTest, ItIsNotAnServerErrorIfOk)
    {
        EXPECT_FALSE(IsServerError(HttpStatusCode::Ok));
    }

    TEST_F(HttpStatusCodeTest, ItIsSuccessfulIfOk)
    {
        EXPECT_TRUE(IsSuccessful(HttpStatusCode::Ok));
    }

    TEST_F(HttpStatusCodeTest, ItIsSuccessfulIfCreated)
    {
        EXPECT_TRUE(IsSuccessful(HttpStatusCode::Created));
    }

    TEST_F(HttpStatusCodeTest, ItIsSuccessfulIfNoContent)
    {
        EXPECT_TRUE(IsSuccessful(HttpStatusCode::NoContent));
    }

    TEST_F(HttpStatusCodeTest, ItIsNotSuccessfulIfUnauthorised)
    {
        EXPECT_FALSE(IsSuccessful(HttpStatusCode::Unauthorised));
    }

    TEST_F(HttpStatusCodeTest, ItIsNotSuccessfulIfForbidden)
    {
        EXPECT_FALSE(IsSuccessful(HttpStatusCode::Forbidden));
    }

    TEST_F(HttpStatusCodeTest, ItIsNotSuccessfulIfUnprocessable)
    {
        EXPECT_FALSE(IsSuccessful(HttpStatusCode::Unprocessable));
    }

    TEST_F(HttpStatusCodeTest, ItIsNotSuccessfulIfServerError)
    {
        EXPECT_FALSE(IsSuccessful(HttpStatusCode::ServerError));
    }

    TEST_F(HttpStatusCodeTest, ItIsNotSuccessfulIfUnknown)
    {
        EXPECT_FALSE(IsSuccessful(HttpStatusCode::Unknown));
    }
} // namespace UKControllerPluginUtilsTest::Http
