#include "api/ApiBootstrap.h"
#include "curl/CurlRequest.h"
#include "curl/CurlResponse.h"
#include "setting/SettingRepository.h"

using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Curl::CurlResponse;
using UKControllerPlugin::Setting::SettingRepository;
using UKControllerPluginUtils::Api::Bootstrap;
using UKControllerPluginUtils::Api::BootstrapLegacy;

namespace UKControllerPluginUtilsTest::Api {
    class ApiBootstrapTest : public testing::Test
    {
        public:
        testing::NiceMock<UKControllerPluginTest::Windows::MockWinApi> windows;
        testing::NiceMock<UKControllerPluginTest::Curl::MockCurlApi> curl;
        SettingRepository settings;
    };

    TEST_F(ApiBootstrapTest, BootstrapReturnsAnApiFactory)
    {
        EXPECT_NE(nullptr, Bootstrap(settings, windows));
    }

    TEST_F(ApiBootstrapTest, BootstrapRegistersApiKeySetting)
    {
        static_cast<void>(Bootstrap(settings, windows));
        EXPECT_TRUE(settings.HasSetting("api-key"));
    }

    TEST_F(ApiBootstrapTest, BootstrapRegistersApiUrlSetting)
    {
        static_cast<void>(Bootstrap(settings, windows));
        EXPECT_TRUE(settings.HasSetting("api-url"));
    }

    TEST_F(ApiBootstrapTest, BootstrapLegacyReturnsLegacyApiInterface)
    {
        auto factory = Bootstrap(settings, windows);
        auto legacyInterface = BootstrapLegacy(*factory, curl);
        EXPECT_NE(nullptr, legacyInterface);

        CurlRequest expectedRequest("https://ukcp.vatsim.uk/authorise", CurlRequest::METHOD_GET);
        expectedRequest.AddHeader("Authorization", "Bearer ");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        EXPECT_CALL(curl, MakeCurlRequest(expectedRequest))
            .Times(1)
            .WillOnce(testing::Return(CurlResponse("", false, 200L)));

        static_cast<void>(legacyInterface->CheckApiAuthorisation());
    }
} // namespace UKControllerPluginUtilsTest::Api
