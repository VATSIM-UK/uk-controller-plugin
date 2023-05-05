#include "curl/curl.h"

#include "api/ApiKeyRedirectUrlBuilder.h"
#include "api/ApiSettings.h"

namespace UKControllerPluginUtilsTest::Api {
    class ApiKeyRedirectUrlBuilderTest : public testing::Test
    {
        public:
        ApiKeyRedirectUrlBuilderTest() : settings("https://vatsim.uk", "abc"), builder(settings)
        {
        }

        UKControllerPluginUtils::Api::ApiSettings settings;
        UKControllerPluginUtils::Api::ApiKeyRedirectUrlBuilder builder;
    };

    TEST_F(ApiKeyRedirectUrlBuilderTest, ItReturnsUrl)
    {
        EXPECT_EQ(
            "https://vatsim.uk/admin/user-create-api-key/?redirect=http%3A%2F%2Flocalhost%3A62134",
            builder.BuildUrl("http://localhost", 62134));
    }
} // namespace UKControllerPluginUtilsTest::Api
