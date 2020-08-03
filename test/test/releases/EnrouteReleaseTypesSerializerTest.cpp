#include "pch/pch.h"
#include "releases/EnrouteReleaseTypesSerializer.h"
#include "releases/EnrouteReleaseType.h"

using ::testing::Test;
using UKControllerPlugin::Releases::EnrouteReleaseType;

namespace UKControllerPluginTest {
    namespace Releases {

        class EnrouteReleaseTypesSerializerTest : public Test
        {

        };

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

        TEST_F(EnrouteReleaseTypesSerializerTest, ItReturnsModelOnValidJson)
        {
            nlohmann::json data{
                {"id", 1},
                {"tag_string", "RFC"},
                {"description", "Released For Climb"}
            };
            
            EnrouteReleaseType releaseType;
            UKControllerPlugin::Releases::from_json(data, releaseType);

            EXPECT_EQ(1, releaseType.id);
            EXPECT_EQ("RFC", releaseType.tagString);
            EXPECT_EQ("Released For Climb", releaseType.description);
        }

        TEST_F(EnrouteReleaseTypesSerializerTest, ItReturnsInvalidOnBadJson)
        {
            nlohmann::json data{
                {"tag_string", "RFC"},
                {"description", "Released For Climb"}
            };

            EnrouteReleaseType releaseType;
            UKControllerPlugin::Releases::from_json(data, releaseType);

            EXPECT_EQ(UKControllerPlugin::Releases::releaseTypeInvalid, releaseType);
        }
    }  // namespace Releases
}  // namespace UKControllerPluginTest
