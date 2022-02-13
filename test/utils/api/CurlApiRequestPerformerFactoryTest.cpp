#include "api/ApiSettings.h"
#include "api/CurlApiRequestPerformerFactory.h"

using UKControllerPluginTest::Curl::MockCurlApi;
using UKControllerPluginUtils::Api::ApiSettings;
using UKControllerPluginUtils::Api::CurlApiRequestPerformerFactory;

namespace UKControllerPluginUtilsTest::Api {
    class CurlApiRequestPerformerFactoryTest : public testing::Test
    {
        public:
        CurlApiRequestPerformerFactoryTest()
            : settings("url", "key"), factory(std::make_unique<testing::NiceMock<MockCurlApi>>())
        {
        }

        ApiSettings settings;
        CurlApiRequestPerformerFactory factory;
    };

    TEST_F(CurlApiRequestPerformerFactoryTest, ItReturnsPerformer)
    {
        EXPECT_NE(nullptr, &factory.Make(settings));
    }

    TEST_F(CurlApiRequestPerformerFactoryTest, ItReturnsTheSamePerformer)
    {
        EXPECT_EQ(&factory.Make(settings), &factory.Make(settings));
    }
} // namespace UKControllerPluginUtilsTest::Api
