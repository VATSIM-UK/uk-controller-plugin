#include "update/CheckDevelopmentVersion.h"

using testing::Test;
using UKControllerPluginUtils::Update::IsDevelopmentVersion;

namespace UKControllerPluginUtilsTest::Update {
    class CheckDevelopmentVersionTest : public Test
    {
    };

    TEST_F(CheckDevelopmentVersionTest, IsDevelopmentIfVersionUnspecified)
    {
        EXPECT_TRUE(IsDevelopmentVersion("#VERSION_STRING#"));
    }

    TEST_F(CheckDevelopmentVersionTest, IsDevelopmentIfVersionNonRelease)
    {
        EXPECT_TRUE(IsDevelopmentVersion("non-release-build"));
    }

    TEST_F(CheckDevelopmentVersionTest, IsNotDevelopmentOnRelease)
    {
        EXPECT_FALSE(IsDevelopmentVersion("1.5.0-"));
    }

    TEST_F(CheckDevelopmentVersionTest, IsNotDevelopmentOnBetaRelease)
    {
        EXPECT_FALSE(IsDevelopmentVersion("1.6.0-beta1"));
    }

    TEST_F(CheckDevelopmentVersionTest, DevelopmentVersionsAreCheckedForStrings)
    {
        EXPECT_TRUE(IsDevelopmentVersion(std::string("#VERSION_STRING#")));
    }
} // namespace UKControllerPluginUtilsTest::Update
