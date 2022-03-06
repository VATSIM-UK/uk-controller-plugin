#include "http/HttpMethod.h"

using UKControllerPluginUtils::Http::HttpMethod;

namespace UKControllerPluginUtilsTest::Http {
    class HttpMethodTest : public testing::Test
    {
    };

    TEST_F(HttpMethodTest, ItHasAGetMethod)
    {
        EXPECT_EQ("GET", static_cast<std::string>(HttpMethod::Get()));
    }

    TEST_F(HttpMethodTest, ItHasAPostMethod)
    {
        EXPECT_EQ("POST", static_cast<std::string>(HttpMethod::Post()));
    }

    TEST_F(HttpMethodTest, ItHasAPutMethod)
    {
        EXPECT_EQ("PUT", static_cast<std::string>(HttpMethod::Put()));
    }

    TEST_F(HttpMethodTest, ItHasAPatchMethod)
    {
        EXPECT_EQ("PATCH", static_cast<std::string>(HttpMethod::Patch()));
    }

    TEST_F(HttpMethodTest, ItHasADeleteMethod)
    {
        EXPECT_EQ("DELETE", static_cast<std::string>(HttpMethod::Delete()));
    }

    TEST_F(HttpMethodTest, EqualityIsTrueIfMethodsSame)
    {
        EXPECT_TRUE(HttpMethod::Delete() == HttpMethod::Delete());
    }

    TEST_F(HttpMethodTest, EqualityIsFalseIfMethodsDifferent)
    {
        EXPECT_FALSE(HttpMethod::Patch() == HttpMethod::Delete());
    }
} // namespace UKControllerPluginUtilsTest::Http
