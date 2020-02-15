#include "pch/pch.h"
#include "regional/RegionalPressureManagerFactory.h"
#include "regional/RegionalPressureManager.h"
#include "mock/MockDependencyLoader.h"

using UKControllerPlugin::Regional::Create;
using UKControllerPlugin::Regional::ValidAsr;
using UKControllerPluginTest::Dependency::MockDependencyLoader;
using UKControllerPlugin::Regional::RegionalPressureManager;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;

namespace UKControllerPluginTest {
    namespace Regional {

        class RegionalPressureManagerFactoryTest : public Test
        {
            public:
                NiceMock<MockDependencyLoader> mockDependency;
        };

        TEST_F(RegionalPressureManagerFactoryTest, ValidAsrReturnsTrueAllValid)
        {
            nlohmann::json asrData = {
                {"key", "ASR_LONDON"},
                {"name", "London"}
            };
            EXPECT_TRUE(ValidAsr(asrData));
        }

        TEST_F(RegionalPressureManagerFactoryTest, ValidAsrReturnsFalseIfKeyNotString)
        {
            nlohmann::json asrData = {
                {"key", 123},
                {"name", "London"}
            };
            EXPECT_FALSE(ValidAsr(asrData));
        }

        TEST_F(RegionalPressureManagerFactoryTest, ValidAsrReturnsFalseIfNoKey)
        {
            nlohmann::json asrData = {
                {"name", "London"}
            };
            EXPECT_FALSE(ValidAsr(asrData));
        }

        TEST_F(RegionalPressureManagerFactoryTest, ValidAsrReturnsFalseIfNameNotString)
        {
            nlohmann::json asrData = {
                {"key", "ASR_LONDON"},
                {"name", 123}
            };
            EXPECT_FALSE(ValidAsr(asrData));
        }

        TEST_F(RegionalPressureManagerFactoryTest, ValidAsrReturnsFalseIfNoName)
        {
            nlohmann::json asrData = {
                {"key", "ASR_LONDON"}
            };
            EXPECT_FALSE(ValidAsr(asrData));
        }

        TEST_F(RegionalPressureManagerFactoryTest, ValidAsrReturnsFalseIfNotObject)
        {
            nlohmann::json asrData = nlohmann::json::array();
            EXPECT_FALSE(ValidAsr(asrData));
        }

        TEST_F(RegionalPressureManagerFactoryTest, CreateLoadsNoAsrsIfDataIsNotArray)
        {
            nlohmann::json asrData = nlohmann::json::object();
            asrData["foo"] = {
                {"key", "ASR_LONDON"},
                {"name", "London"}
            };
            EXPECT_EQ(0, Create(this->mockDependency)->CountAltimeterSettingRegions());
        }

        TEST_F(RegionalPressureManagerFactoryTest, CreateSkipsBadAsrs)
        {
            nlohmann::json asrData = nlohmann::json::array();
            asrData.push_back({
                {"key", 123},
                {"name", "London"}
            });
            EXPECT_EQ(0, Create(this->mockDependency)->CountAltimeterSettingRegions());
        }

        TEST_F(RegionalPressureManagerFactoryTest, CreateLoadsAsrs)
        {
            nlohmann::json asrData = nlohmann::json::array();
            asrData.push_back({
                {"key", "ASR_LONDON"},
                {"name", "London"}
            });
            asrData.push_back({
                {"key", "ASR_SCOTTISH"},
                {"name", "Scottish"}
            });

            ON_CALL(this->mockDependency, LoadDependency("DEPENDENCY_ASR", nlohmann::json::array()))
                .WillByDefault(Return(asrData));

            std::shared_ptr<RegionalPressureManager> manager = Create(this->mockDependency);
            EXPECT_EQ(2, manager->CountAltimeterSettingRegions());
            EXPECT_EQ("London", manager->GetNameFromKey("ASR_LONDON"));
            EXPECT_EQ("Scottish", manager->GetNameFromKey("ASR_SCOTTISH"));
        }
    }  // namespace Regional
}  // namespace UKControllerPluginTest
