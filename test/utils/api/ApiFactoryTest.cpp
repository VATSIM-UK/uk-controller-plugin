#include "api/ApiFactory.h"
#include "api/ApiRequestBuilder.h"
#include "api/ApiRequestFactory.h"
#include "api/ApiSettings.h"
#include "curl/CurlRequest.h"

using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPluginUtils::Api::ApiFactory;
using UKControllerPluginUtils::Api::ApiSettings;

namespace UKControllerPluginUtilsTest::Api {
    class ApiFactoryTest : public testing::Test
    {
        public:
        ApiFactoryTest()
            : settings("https://ukcp.vatsim.uk", "key"),
              requestFactory(std::make_shared<testing::NiceMock<MockApiRequestPerformerFactory>>()),
              settingsProvider(std::make_shared<testing::NiceMock<MockApiSettingsProvider>>()),
              factory(settingsProvider, requestFactory)
        {
            ON_CALL(*settingsProvider, Get).WillByDefault(testing::ReturnRef(settings));
            ON_CALL(*requestFactory, Make(testing::Ref(settings))).WillByDefault(testing::ReturnRef(requestPerformer));
        }

        ApiSettings settings;
        std::shared_ptr<testing::NiceMock<MockApiRequestPerformerFactory>> requestFactory;
        testing::NiceMock<MockApiRequestPerformer> requestPerformer;
        std::shared_ptr<testing::NiceMock<MockApiSettingsProvider>> settingsProvider;
        ApiFactory factory;
    };

    TEST_F(ApiFactoryTest, ItReturnsSettingsProvider)
    {
        EXPECT_EQ(settingsProvider, factory.SettingsProvider());
    }

    TEST_F(ApiFactoryTest, ItReturnsARequestFactory)
    {
        auto& requestFactory = factory.RequestFactory();
        EXPECT_CALL(requestPerformer, Perform(ApiRequestData("test", UKControllerPluginUtils::Http::HttpMethod::Get())))
            .Times(1)
            .WillOnce(testing::Return(Response(UKControllerPluginUtils::Http::HttpStatusCode::Ok, {})));

        static_cast<void>(requestFactory.Get("test"));
        requestFactory.AwaitRequestCompletion();
    }

    TEST_F(ApiFactoryTest, ItReturnsARequestFactoryAsASingleton)
    {
        auto& requestFactory1 = factory.RequestFactory();
        auto& requestFactory2 = factory.RequestFactory();

        EXPECT_EQ(&requestFactory1, &requestFactory2);
    }

    TEST_F(ApiFactoryTest, ItReturnsTheLegacyRequestBuilder)
    {
        auto& builder = factory.LegacyRequestBuilder();

        CurlRequest expectedRequest("https://ukcp.vatsim.uk/authorise", CurlRequest::METHOD_GET);
        expectedRequest.AddHeader("Authorization", "Bearer key");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        EXPECT_EQ(expectedRequest, builder.BuildAuthCheckRequest());
    }

    TEST_F(ApiFactoryTest, ItReturnsTheLegacyRequestBuilderAsASingleton)
    {
        auto& builder1 = factory.LegacyRequestBuilder();
        auto& builder2 = factory.LegacyRequestBuilder();

        EXPECT_EQ(&builder1, &builder2);
    }
} // namespace UKControllerPluginUtilsTest::Api
