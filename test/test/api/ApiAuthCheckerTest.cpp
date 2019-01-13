#include "pch/pch.h"
#include "api/ApiAuthChecker.h"
#include "mock/MockWinApi.h"
#include "mock/MockApiInterface.h"
#include "api/ApiNotAuthorisedException.h"
#include "api/ApiException.h"

using UKControllerPlugin::Api::ApiAuthChecker;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPlugin::Api::ApiNotAuthorisedException;
using UKControllerPlugin::Api::ApiException;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Throw;
using ::testing::Return;

namespace UKControllerPluginTest {
    namespace Api {

        class ApiAuthCheckerTest : public Test
        {
            public:
                NiceMock<MockApiInterface> mockApi;
                NiceMock<MockWinApi> mockWindows;
        };

        TEST_F(ApiAuthCheckerTest, IsAuthorisedReturnsFalseIfNotAuthorised)
        {
            EXPECT_CALL(mockApi, CheckApiAuthorisation())
                .Times(1)
                .WillOnce(Throw(ApiNotAuthorisedException("not allowed")));

            EXPECT_FALSE(ApiAuthChecker::IsAuthorised(this->mockApi, this->mockWindows));
        }

        TEST_F(ApiAuthCheckerTest, IsAuthorisedReturnsFalseIfApiFails)
        {
            EXPECT_CALL(mockApi, CheckApiAuthorisation())
                .Times(1)
                .WillOnce(Throw(ApiException("something went wrong")));

            EXPECT_FALSE(ApiAuthChecker::IsAuthorised(this->mockApi, this->mockWindows));
        }

        TEST_F(ApiAuthCheckerTest, IsAuthorisedReturnsTrueIfCheckOk)
        {
            EXPECT_CALL(mockApi, CheckApiAuthorisation())
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_TRUE(ApiAuthChecker::IsAuthorised(this->mockApi, this->mockWindows));
        }
    }  // namespace Api
}  // namespace UKControllerPluginTest
