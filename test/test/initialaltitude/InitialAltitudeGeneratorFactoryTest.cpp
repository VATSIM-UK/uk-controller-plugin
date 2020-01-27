#include "pch/pch.h"
#include "initialaltitude/InitialAltitudeGeneratorFactory.h"
#include "initialaltitude/InitialAltitudeGenerator.h"
#include "dependency/DependencyCache.h"
#include "mock/MockDependencyLoader.h"

using UKControllerPlugin::InitialAltitude::InitialAltitudeGenerator;
using UKControllerPlugin::InitialAltitude::InitialAltitudeGeneratorFactory;
using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPluginTest::Dependency::MockDependencyLoader;
using testing::Test;
using testing::NiceMock;
using testing::Return;

namespace UKControllerPluginTest {
    namespace InitialAltitude {

        class InitialAltitudeGeneratorFactoryTest : public Test
        {
            public:
                NiceMock<MockDependencyLoader> mockLoader;
        };

        TEST_F(InitialAltitudeGeneratorFactoryTest, CreateIgnoresNonIntegerInitialAltitudes)
        {
            ON_CALL(this->mockLoader, LoadDependency("DEPENDENCY_INITIAL_ALTITUDES", nlohmann::json::array()))
                .WillByDefault(Return("{\"EGKK\": {\"ADMAG2X\":6000, \"DVR8M\":\"abc\"}}"_json));

            EXPECT_FALSE(InitialAltitudeGeneratorFactory::Create(this->mockLoader)->HasSid("EGKK", "DVR8M"));
        }

        TEST_F(InitialAltitudeGeneratorFactoryTest, CreateIgnoresInvalidJson)
        {
            ON_CALL(this->mockLoader, LoadDependency("DEPENDENCY_INITIAL_ALTITUDES", nlohmann::json::array()))
                .WillByDefault(Return(nlohmann::json::array()));
            EXPECT_FALSE(InitialAltitudeGeneratorFactory::Create(this->mockLoader)->HasSid("EGKK", "ADMAG2X"));
        }

        TEST_F(InitialAltitudeGeneratorFactoryTest, CreateStopsIfFileMissing)
        {
            ON_CALL(this->mockLoader, LoadDependency("DEPENDENCY_INITIAL_ALTITUDES", nlohmann::json::array()))
                .WillByDefault(Return(nlohmann::json::array()));
            EXPECT_FALSE(InitialAltitudeGeneratorFactory::Create(this->mockLoader)->HasSid("EGKK", "ADMAG2X"));
        }

        TEST_F(InitialAltitudeGeneratorFactoryTest, CreateIncludesIntegerInitialAltitudes)
        {
            ON_CALL(this->mockLoader, LoadDependency("DEPENDENCY_INITIAL_ALTITUDES", nlohmann::json::array()))
                .WillByDefault(Return("{\"EGKK\": {\"ADMAG2X\":6000, \"DVR8M\":\"abc\"}}"_json));
            EXPECT_EQ(
                6000,
                InitialAltitudeGeneratorFactory::Create(this->mockLoader)
                    ->GetInitialAltitudeForDeparture("EGKK", "ADMAG2X")
            );
        }

        TEST_F(InitialAltitudeGeneratorFactoryTest, CreateRemovesDeprecationMarkings)
        {
            ON_CALL(this->mockLoader, LoadDependency("DEPENDENCY_INITIAL_ALTITUDES", nlohmann::json::array()))
                .WillByDefault(Return("{\"EGKK\": {\"#ADMAG2X\":6000, \"DVR8M\":\"abc\"}}"_json));
            EXPECT_EQ(
                6000,
                InitialAltitudeGeneratorFactory::Create(this->mockLoader)
                    ->GetInitialAltitudeForDeparture("EGKK", "ADMAG2X")
            );
        }

        TEST_F(InitialAltitudeGeneratorFactoryTest, CreateProcessesMultipleAirfields)
        {
            ON_CALL(
                this->mockLoader,
                LoadDependency("DEPENDENCY_INITIAL_ALTITUDES", nlohmann::json::array())
            )
                .WillByDefault(
                    Return("{\"EGKK\": {\"ADMAG2X\":6000, \"SAM1X\":5000}, \"EGLC\": {\"BPK1A\" : 3000}}"_json)
                );
            std::unique_ptr<InitialAltitudeGenerator> initial =
                InitialAltitudeGeneratorFactory::Create(this->mockLoader);
            EXPECT_EQ(6000, initial->GetInitialAltitudeForDeparture("EGKK", "ADMAG2X"));
            EXPECT_EQ(5000, initial->GetInitialAltitudeForDeparture("EGKK", "SAM1X"));
            EXPECT_EQ(3000, initial->GetInitialAltitudeForDeparture("EGLC", "BPK1A"));
        }

        TEST_F(InitialAltitudeGeneratorFactoryTest, KeepsGoingIfDuplicateDeparture)
        {
            ON_CALL(
                this->mockLoader,
                LoadDependency("DEPENDENCY_INITIAL_ALTITUDES", nlohmann::json::array())
            )
                .WillByDefault(
                    Return("{\"EGKK\": {\"ADMAG2X\":6000, \"ADMAG2X\":6000}, \"EGLC\": {\"BPK1A\" : 3000}}"_json)
                );
            std::unique_ptr<InitialAltitudeGenerator> initial =
                InitialAltitudeGeneratorFactory::Create(this->mockLoader);
            EXPECT_EQ(6000, initial->GetInitialAltitudeForDeparture("EGKK", "ADMAG2X"));
            EXPECT_EQ(3000, initial->GetInitialAltitudeForDeparture("EGLC", "BPK1A"));
        }
    }  // namespace InitialAltitude
}  // namespace UKControllerPluginTest
