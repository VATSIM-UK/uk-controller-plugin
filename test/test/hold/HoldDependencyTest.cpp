#include "pch/pch.h"
#include "mock/MockWinApi.h"
#include "mock/MockApiInterface.h"
#include "hold/HoldDependency.h"
#include "api/ApiException.h"

using UKControllerPlugin::Api::ApiException;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPluginTest::Api::MockApiInterface;
using testing::Test;
using testing::NiceMock;
using testing::Return;
using testing::Throw;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldDependencyTest : public Test
        {
            public:
                const std::string dependencyFile = "dependencies/holds.json";
                const std::string defaultWrite = "[]";
                NiceMock<MockWinApi> mockWindows;
                NiceMock<MockApiInterface> mockApi;
        };

        TEST_F(HoldDependencyTest, UpdateHoldDependencyCreatesPlaceholderFileIfNotExists)
        {
            ON_CALL(this->mockWindows, FileExists(this->dependencyFile))
                .WillByDefault(Return(false));

            EXPECT_CALL(this->mockWindows, WriteToFile(_, _, _));

            EXPECT_CALL(this->mockWindows, WriteToFile(this->dependencyFile, this->defaultWrite, true));
            UKControllerPlugin::Hold::UpdateHoldDependency(this->mockApi, this->mockWindows);
        }

        TEST_F(HoldDependencyTest, UpdateHoldDependencyGracefullyHandlesApiFailure)
        {
            ON_CALL(this->mockWindows, FileExists(this->dependencyFile))
                .WillByDefault(Return(true));

            ON_CALL(this->mockApi, GetHoldDependency())
                .WillByDefault(Throw(ApiException("Nah")));

            EXPECT_NO_THROW(UKControllerPlugin::Hold::UpdateHoldDependency(this->mockApi, this->mockWindows));
        }

        TEST_F(HoldDependencyTest, UpdateDependencyWritesDataToFile)
        {
            nlohmann::json returnData;
            returnData["foo"] = "bar";
            returnData["baz"] = "what";

            ON_CALL(this->mockWindows, FileExists(this->dependencyFile))
                .WillByDefault(Return(true));

            ON_CALL(this->mockApi, GetHoldDependency())
                .WillByDefault(Return(returnData));

            EXPECT_CALL(this->mockWindows, WriteToFile(this->dependencyFile, returnData.dump(), true));
            UKControllerPlugin::Hold::UpdateHoldDependency(this->mockApi, this->mockWindows);
        }

        TEST_F(HoldDependencyTest, GetLocalHoldDataReturnsEmptyIfFileDoesntExist)
        {
            ON_CALL(this->mockWindows, FileExists(this->dependencyFile))
                .WillByDefault(Return(false));

            EXPECT_EQ(
                nlohmann::json::parse(this->defaultWrite),
                UKControllerPlugin::Hold::GetLocalHoldData(this->mockWindows)
            );
        }

        TEST_F(HoldDependencyTest, GetLocalHoldDataReturnsEmptyOnInvalidJson)
        {
            ON_CALL(this->mockWindows, FileExists(this->dependencyFile))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(this->dependencyFile, true))
                .WillByDefault(Return("{]"));

            EXPECT_EQ(
                nlohmann::json::parse(this->defaultWrite),
                UKControllerPlugin::Hold::GetLocalHoldData(this->mockWindows)
            );
        }

        TEST_F(HoldDependencyTest, GetLocalHoldDataReturnsValidJsonOnSuccess)
        {
            nlohmann::json returnData;
            returnData["foo"] = "bar";
            returnData["baz"] = "what";

            ON_CALL(this->mockWindows, FileExists(this->dependencyFile))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(this->dependencyFile, true))
                .WillByDefault(Return(returnData.dump()));

            EXPECT_EQ(
                returnData, UKControllerPlugin::Hold::GetLocalHoldData(this->mockWindows)
            );
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest