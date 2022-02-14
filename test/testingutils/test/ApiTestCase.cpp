#include "ApiTestCase.h"
#include "api/ApiFactory.h"
#include "api/ApiRequestData.h"
#include "api/ApiRequestException.h"
#include "api/ApiRequestFacade.h"
#include "api/ApiRequestFactory.h"
#include "api/ApiSettings.h"
#include "api/Response.h"

using UKControllerPluginUtils::Api::ApiFactory;
using UKControllerPluginUtils::Api::ApiRequestFactory;
using UKControllerPluginUtils::Api::ApiSettings;
using UKControllerPluginUtilsTest::Api::MockApiRequestPerformer;
using UKControllerPluginUtilsTest::Api::MockApiRequestPerformerFactory;
using UKControllerPluginUtilsTest::Api::MockApiSettingsProvider;

namespace UKControllerPluginTest {
    ApiTestCase::ApiTestCase()
        : settings(std::make_shared<ApiSettings>("https://ukcp.vatsim.uk", "key")),
          requestPerformer(
              std::make_shared<testing::NiceMock<UKControllerPluginUtilsTest::Api ::MockApiRequestPerformer>>()),
          requestPerformerFactory(std::make_shared<testing::NiceMock<MockApiRequestPerformerFactory>>()),
          settingsProvider(std::make_shared<testing::NiceMock<MockApiSettingsProvider>>()),
          factory(std::make_shared<ApiFactory>(settingsProvider, requestPerformerFactory))
    {
        ON_CALL(*settingsProvider, Get).WillByDefault(testing::ReturnRef(*settings));

        ON_CALL(*requestPerformerFactory, Make(testing::Ref(*settings)))
            .WillByDefault(testing::ReturnRef(*requestPerformer));

        SetApiRequestFactory(factory);
    }

    void ApiTestCase::ExpectApiRequestWithResponse(
        const UKControllerPluginUtils::Api::ApiRequestData& request,
        const UKControllerPluginUtils::Api::Response& response)
    {
        EXPECT_CALL(*requestPerformer, Perform(request)).Times(1).WillOnce(testing::Return(response));
    }

    void ApiTestCase::ExpectApiRequestWithError(
        const UKControllerPluginUtils::Api::ApiRequestData& request,
        const UKControllerPluginUtils::Api::ApiRequestException& exception)
    {
        EXPECT_CALL(*requestPerformer, Perform(request)).Times(1).WillOnce(testing::Throw(exception));
    }

    void ApiTestCase::ExpectNoRequests()
    {
        EXPECT_CALL(*requestPerformer, Perform(testing::_)).Times(0);
    }

    void ApiTestCase::DontExpectRequest(const UKControllerPluginUtils::Api::ApiRequestData& request)
    {
        EXPECT_CALL(*requestPerformer, Perform(request)).Times(0);
    }

    void ApiTestCase::AwaitApiCallCompletion()
    {
        factory->RequestFactory().AwaitRequestCompletion();
    }

    auto ApiTestCase::SettingsProvider()
        -> testing::NiceMock<UKControllerPluginUtilsTest::Api::MockApiSettingsProvider>&
    {
        return *settingsProvider;
    }
} // namespace UKControllerPluginTest
