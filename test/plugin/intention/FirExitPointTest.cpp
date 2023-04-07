#include "intention/ExitDetermination.h"
#include "intention/FirExitPoint.h"

using UKControllerPlugin::IntentionCode::FirExitPoint;

namespace UKControllerPluginTest::IntentionCode {
    class FirExitPointTest : public testing::Test
    {
        public:
        FirExitPointTest()
            : mockExitDetermination(std::make_shared<testing::NiceMock<MockExitDetermination>>()),
              exitPoint(1, "TEST", true, mockExitDetermination)
        {
        }

        std::shared_ptr<testing::NiceMock<MockExitDetermination>> mockExitDetermination;
        FirExitPoint exitPoint;
    };

    TEST_F(FirExitPointTest, ItHasAnId)
    {
        EXPECT_EQ(1, exitPoint.Id());
    }

    TEST_F(FirExitPointTest, ItHasAnIdentifier)
    {
        EXPECT_EQ("TEST", exitPoint.Identifier());
    }

    TEST_F(FirExitPointTest, ItIsInternal)
    {
        EXPECT_TRUE(exitPoint.IsInternal());
    }

    TEST_F(FirExitPointTest, ItHasAnExitDetermination)
    {
        EXPECT_EQ(mockExitDetermination.get(), &exitPoint.FirExitDetermination());
    }
} // namespace UKControllerPluginTest::IntentionCode
