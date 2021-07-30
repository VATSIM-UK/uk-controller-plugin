#include "pch/pch.h"
#include "releases/EnrouteReleaseTypesSerializer.h"
#include "releases/EnrouteReleaseType.h"
#include "releases/CompareEnrouteReleaseTypes.h"

using ::testing::Test;
using UKControllerPlugin::Releases::EnrouteReleaseType;
using UKControllerPlugin::Releases::CompareEnrouteReleaseTypes;

namespace UKControllerPluginTest {
    namespace Releases {

        class EnrouteReleaseTypesSerializerTest : public Test
        {

        };

        TEST_F(EnrouteReleaseTypesSerializerTest, DependencyValidReturnsTrueIfValid)
        {
            EXPECT_TRUE(UKControllerPlugin::Releases::DependencyValid(nlohmann::json::array()));
        }

        TEST_F(EnrouteReleaseTypesSerializerTest, DependencyValidReturnsFalseIfNotArray)
        {
            EXPECT_FALSE(UKControllerPlugin::Releases::DependencyValid(nlohmann::json::object()));
        }

        TEST_F(EnrouteReleaseTypesSerializerTest, JsonValidReturnsTrueIfValid)
        {
            nlohmann::json data{
                {"id", 1},
                {"tag_string", "RFC"},
                {"description", "Released For Climb"}
            };

            EXPECT_TRUE(UKControllerPlugin::Releases::JsonValid(data));
        }

        TEST_F(EnrouteReleaseTypesSerializerTest, JsonValidReturnsFalseIfIdMissing)
        {
            nlohmann::json data{
                {"tag_string", "RFC"},
                {"description", "Released For Climb"}
            };

            EXPECT_FALSE(UKControllerPlugin::Releases::JsonValid(data));
        }

        TEST_F(EnrouteReleaseTypesSerializerTest, JsonValidReturnsFalseIfIdNotInteger)
        {
            nlohmann::json data{
                {"id", "abc"},
                {"tag_string", "RFC"},
                {"description", "Released For Climb"}
            };

            EXPECT_FALSE(UKControllerPlugin::Releases::JsonValid(data));
        }

        TEST_F(EnrouteReleaseTypesSerializerTest, JsonValidReturnsFalseIfTagStringMissing)
        {
            nlohmann::json data{
                {"id", 1},
                {"description", "Released For Climb"}
            };

            EXPECT_FALSE(UKControllerPlugin::Releases::JsonValid(data));
        }

        TEST_F(EnrouteReleaseTypesSerializerTest, JsonValidReturnsFalseIfTagStringNotString)
        {
            nlohmann::json data{
                {"id", 1},
                {"tag_string", 123},
                {"description", "Released For Climb"}
            };

            EXPECT_FALSE(UKControllerPlugin::Releases::JsonValid(data));
        }

        TEST_F(EnrouteReleaseTypesSerializerTest, JsonValidReturnsFalseIfDescriptionMissing)
        {
            nlohmann::json data{
                {"id", 1},
                {"tag_string", "RFC"},
            };

            EXPECT_FALSE(UKControllerPlugin::Releases::JsonValid(data));
        }

        TEST_F(EnrouteReleaseTypesSerializerTest, JsonValidReturnsFalseIfDescriptionNotString)
        {
            nlohmann::json data{
                {"id", 1},
                {"tag_string", "RFC"},
                {"description", 123}
            };

            EXPECT_FALSE(UKControllerPlugin::Releases::JsonValid(data));
        }

        TEST_F(EnrouteReleaseTypesSerializerTest, ItReturnsCollectionOfReleaseTypes)
        {
            nlohmann::json dependency = nlohmann::json::array();
            dependency.push_back(
                {
                    {"id", 1},
                    {"tag_string", "RFC"},
                    {"description", "Released For Climb"}
                }
            );
            dependency.push_back(
                {
                    {"id", 2},
                    {"tag_string", "RFD"},
                    {"description", "Released For Descent"}
                }
            );


            std::set<EnrouteReleaseType, CompareEnrouteReleaseTypes> releases;
            UKControllerPlugin::Releases::from_json(dependency, releases);

            EXPECT_EQ(2, releases.size());
            EXPECT_TRUE(releases.find(1) != releases.cend());
            EXPECT_EQ(1, releases.find(1)->id);
            EXPECT_EQ("RFC", releases.find(1)->tagString);
            EXPECT_EQ("Released For Climb", releases.find(1)->description);
            EXPECT_TRUE(releases.find(2) != releases.cend());
            EXPECT_EQ(2, releases.find(2)->id);
            EXPECT_EQ("RFD", releases.find(2)->tagString);
            EXPECT_EQ("Released For Descent", releases.find(2)->description);
        }

        TEST_F(EnrouteReleaseTypesSerializerTest, ItHandlesInvalidDependency)
        {
            nlohmann::json dependency = {
                {"id", 1},
                {"tag_string", "RFC"},
                {"description", "Released For Climb"}
            };


            std::set<EnrouteReleaseType, CompareEnrouteReleaseTypes> releases;
            UKControllerPlugin::Releases::from_json(dependency, releases);

            EXPECT_EQ(0, releases.size());
        }

        TEST_F(EnrouteReleaseTypesSerializerTest, ItHandlesInvalidTypes)
        {
            nlohmann::json dependency = nlohmann::json::array();
            dependency.push_back(
                {
                    {"id", 1},
                    {"tag_string", 123},  // Invalid
                    {"description", "Released For Climb"}
                }
            );
            dependency.push_back(
                {
                    {"id", 2},
                    {"tag_string", "RFD"},  // Missing description
                }
            );


            std::set<EnrouteReleaseType, CompareEnrouteReleaseTypes> releases;
            UKControllerPlugin::Releases::from_json(dependency, releases);

            EXPECT_EQ(0, releases.size());
        }
    }  // namespace Releases
}  // namespace UKControllerPluginTest
