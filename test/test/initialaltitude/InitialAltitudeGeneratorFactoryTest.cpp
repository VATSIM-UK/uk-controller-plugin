#include "pch/pch.h"
#include "initialaltitude/InitialAltitudeGeneratorFactory.h"
#include "initialaltitude/InitialAltitudeGenerator.h"
#include "dependency/DependencyCache.h"

using UKControllerPlugin::InitialAltitude::InitialAltitudeGenerator;
using UKControllerPlugin::InitialAltitude::InitialAltitudeGeneratorFactory;
using UKControllerPlugin::Dependency::DependencyCache;

namespace UKControllerPluginTest {
    namespace InitialAltitude {
        TEST(InitialAltitudeGeneratorFactory, CreateIgnoresNonIntegerInitialAltitudes)
        {
            DependencyCache dependency;
            dependency.AddDependency(
                "initial-altitudes.json",
                "{\"EGKK\": {\"ADMAG2X\":6000, \"DVR8M\":\"abc\"}}"
            );
            EXPECT_FALSE(InitialAltitudeGeneratorFactory::Create(dependency)->HasSid("EGKK", "DVR8M"));
        }

        TEST(InitialAltitudeGeneratorFactory, CreateIgnoresInvalidJson)
        {
            DependencyCache dependency;
            dependency.AddDependency("initial-altitudes.json", "{\"EGKK\": {\"ADMAG2X\":6000,}}");
            EXPECT_FALSE(InitialAltitudeGeneratorFactory::Create(dependency)->HasSid("EGKK", "ADMAG2X"));
        }

        TEST(InitialAltitudeGeneratorFactory, CreateStopsIfFileMissing)
        {
            DependencyCache dependency;
            EXPECT_FALSE(InitialAltitudeGeneratorFactory::Create(dependency)->HasSid("EGKK", "ADMAG2X"));
        }

        TEST(InitialAltitudeGeneratorFactory, CreateIncludesIntegerInitialAltitudes)
        {
            DependencyCache dependency;
            dependency.AddDependency(
                "initial-altitudes.json", "{\"EGKK\": {\"ADMAG2X\":6000, \"DVR8M\":\"abc\"}}"
            );
            EXPECT_EQ(
                6000,
                InitialAltitudeGeneratorFactory::Create(dependency)->GetInitialAltitudeForDeparture("EGKK", "ADMAG2X")
            );
        }

        TEST(InitialAltitudeGeneratorFactory, CreateRemovesDeprecationMarkings)
        {
            DependencyCache dependency;
            dependency.AddDependency("initial-altitudes.json", "{\"EGKK\": {\"#ADMAG2X\":6000, \"DVR8M\":\"abc\"}}");
            EXPECT_EQ(
                6000,
                InitialAltitudeGeneratorFactory::Create(dependency)->GetInitialAltitudeForDeparture("EGKK", "ADMAG2X")
            );
        }

        TEST(InitialAltitudeGeneratorFactory, CreateProcessesMultipleAirfields)
        {
            DependencyCache dependency;
            dependency.AddDependency(
                "initial-altitudes.json",
                "{\"EGKK\": {\"ADMAG2X\":6000, \"SAM1X\":5000}, \"EGLC\": {\"BPK1A\" : 3000}}"
            );
            std::unique_ptr<InitialAltitudeGenerator> initial = InitialAltitudeGeneratorFactory::Create(dependency);
            EXPECT_EQ(6000, initial->GetInitialAltitudeForDeparture("EGKK", "ADMAG2X"));
            EXPECT_EQ(5000, initial->GetInitialAltitudeForDeparture("EGKK", "SAM1X"));
            EXPECT_EQ(3000, initial->GetInitialAltitudeForDeparture("EGLC", "BPK1A"));
        }

        TEST(InitialAltitudeGeneratorFactory, KeepsGoingIfDuplicateDeparture)
        {
            DependencyCache dependency;
            dependency.AddDependency(
                "initial-altitudes.json",
                "{\"EGKK\": {\"ADMAG2X\":6000, \"ADMAG2X\":6000}, \"EGLC\": {\"BPK1A\" : 3000}}"
            );
            std::unique_ptr<InitialAltitudeGenerator> initial = InitialAltitudeGeneratorFactory::Create(dependency);
            EXPECT_EQ(6000, initial->GetInitialAltitudeForDeparture("EGKK", "ADMAG2X"));
            EXPECT_EQ(3000, initial->GetInitialAltitudeForDeparture("EGLC", "BPK1A"));
        }
    }  // namespace InitialAltitude
}  // namespace UKControllerPluginTest
