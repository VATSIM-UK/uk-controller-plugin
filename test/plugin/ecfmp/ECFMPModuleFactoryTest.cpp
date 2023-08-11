#include "ecfmp/ECFMPModuleFactory.h"
#include "mock/MockCurlApi.h"

namespace UKControllerPluginTest::ECFMP {
    class ECFMPModuleFactoryTest : public testing::Test
    {
        public:
        ECFMPModuleFactoryTest()
        {
        }

        testing::NiceMock<Curl::MockCurlApi> mockCurl;
        UKControllerPlugin::ECFMP::ECFMPModuleFactory factory;
    };

    TEST_F(ECFMPModuleFactoryTest, ItCreatesTheECFMPSDK)
    {
        const auto sdk = factory.Sdk(mockCurl);
        EXPECT_EQ(0, sdk->FlowMeasures()->Count());
    }

    TEST_F(ECFMPModuleFactoryTest, ItReturnsTheSdkAsASingleton)
    {
        const auto sdk1 = factory.Sdk(mockCurl);
        const auto sdk2 = factory.Sdk(mockCurl);
        EXPECT_EQ(sdk1, sdk2);
    }
} // namespace UKControllerPluginTest::ECFMP
