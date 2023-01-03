#include "intention/ExitCone.h"
#include "intention/FirExitPointCollectionFactory.h"
#include "intention/FirExitPointCollection.h"
#include "intention/FirExitPoint.h"

using UKControllerPlugin::IntentionCode::ExitCone;
using UKControllerPlugin::IntentionCode::ExitPointValid;
using UKControllerPlugin::IntentionCode::MakeFirExitPointCollection;

namespace UKControllerPluginTest::IntentionCode {
    class FirExitPointCollectionFactoryTest : public testing::Test
    {
        public:
        static auto MakeExitPoint(
            const nlohmann::json& overridingData = nlohmann::json::object(), const std::string& keyToRemove = "")
            -> nlohmann::json
        {
            nlohmann::json exitPoint{
                {"id", 1},
                {"exit_point", "INKOB"},
                {"internal", false},
                {"exit_direction_start", 123},
                {"exit_direction_end", 234}};
            exitPoint.update(overridingData);

            if (!keyToRemove.empty()) {
                exitPoint.erase(exitPoint.find(keyToRemove));
            }

            return exitPoint;
        };
    };

    TEST_F(FirExitPointCollectionFactoryTest, ItReturnsEmptyCollectionIfDataIsNotArray)
    {
        EXPECT_EQ(0, MakeFirExitPointCollection(nlohmann::json::object())->CountPoints());
    }

    TEST_F(FirExitPointCollectionFactoryTest, ItSkipsInvalidPoints)
    {
        const auto collection = MakeFirExitPointCollection(nlohmann::json::array(
            {MakeExitPoint(nlohmann::json::object(), "id"), MakeExitPoint(nlohmann::json::object(), "exit_point")}));
        EXPECT_EQ(0, collection->CountPoints());
    }

    TEST_F(FirExitPointCollectionFactoryTest, ItBuildsCollection)
    {
        const auto collection = MakeFirExitPointCollection(nlohmann::json::array(
            {MakeExitPoint(), MakeExitPoint(nlohmann::json{{"id", 2}, {"exit_point", "SOSIM"}, {"internal", true}})}));
        EXPECT_EQ(2, collection->CountPoints());

        // Check the first point
        const auto point1 = collection->PointByIdentifier("INKOB");
        EXPECT_EQ(1, point1->Id());
        EXPECT_EQ("INKOB", point1->Identifier());
        EXPECT_FALSE(point1->IsInternal());

        const auto& exitCone1 = static_cast<const ExitCone&>(point1->FirExitDetermination());
        EXPECT_EQ(123, exitCone1.StartDirection());
        EXPECT_EQ(234, exitCone1.EndDirection());

        // Check the second point
        const auto point2 = collection->PointByIdentifier("SOSIM");
        EXPECT_EQ(2, point2->Id());
        EXPECT_EQ("SOSIM", point2->Identifier());
        EXPECT_TRUE(point2->IsInternal());

        const auto& exitCone2 = static_cast<const ExitCone&>(point2->FirExitDetermination());
        EXPECT_EQ(123, exitCone2.StartDirection());
        EXPECT_EQ(234, exitCone2.EndDirection());
    }

    TEST_F(FirExitPointCollectionFactoryTest, ExitPointIsValid)
    {
        EXPECT_TRUE(ExitPointValid(MakeExitPoint()));
    }

    TEST_F(FirExitPointCollectionFactoryTest, ExitPointIsValidWhenInternal)
    {
        EXPECT_TRUE(ExitPointValid(MakeExitPoint(nlohmann::json{{"internal", true}})));
    }

    TEST_F(FirExitPointCollectionFactoryTest, ExitPointIsInvalidNotObject)
    {
        EXPECT_FALSE(ExitPointValid(nlohmann::json::array()));
    }

    TEST_F(FirExitPointCollectionFactoryTest, ExitPointIsInvalidIdMissing)
    {
        EXPECT_FALSE(ExitPointValid(MakeExitPoint(nlohmann::json::object(), "id")));
    }

    TEST_F(FirExitPointCollectionFactoryTest, ExitPointIsInvalidIdNotAnInteger)
    {
        EXPECT_FALSE(ExitPointValid(MakeExitPoint(nlohmann::json{{"id", "abc"}})));
    }

    TEST_F(FirExitPointCollectionFactoryTest, ExitPointIsInvalidExitPointMissing)
    {
        EXPECT_FALSE(ExitPointValid(MakeExitPoint(nlohmann::json::object(), "exit_point")));
    }

    TEST_F(FirExitPointCollectionFactoryTest, ExitPointIsInvalidExitPointNotAString)
    {
        EXPECT_FALSE(ExitPointValid(MakeExitPoint(nlohmann::json{{"exit_point", 123}})));
    }

    TEST_F(FirExitPointCollectionFactoryTest, ExitPointIsInvalidInternalMissing)
    {
        EXPECT_FALSE(ExitPointValid(MakeExitPoint(nlohmann::json::object(), "internal")));
    }

    TEST_F(FirExitPointCollectionFactoryTest, ExitPointIsInternalNotABoolean)
    {
        EXPECT_FALSE(ExitPointValid(MakeExitPoint(nlohmann::json{{"internal", "abc"}})));
    }

    TEST_F(FirExitPointCollectionFactoryTest, ExitPointIsInvalidStartDirectionMissing)
    {
        EXPECT_FALSE(ExitPointValid(MakeExitPoint(nlohmann::json::object(), "exit_direction_start")));
    }

    TEST_F(FirExitPointCollectionFactoryTest, ExitPointIsInvalidStartDirectionNotAnInteger)
    {
        EXPECT_FALSE(ExitPointValid(MakeExitPoint(nlohmann::json{{"exit_direction_start", "abc"}})));
    }

    TEST_F(FirExitPointCollectionFactoryTest, ExitPointIsInvalidStartDirectionTooSmall)
    {
        EXPECT_FALSE(ExitPointValid(MakeExitPoint(nlohmann::json{{"exit_direction_start", -1}})));
    }

    TEST_F(FirExitPointCollectionFactoryTest, ExitPointIsInvalidStartDirectionTooLarge)
    {
        EXPECT_FALSE(ExitPointValid(MakeExitPoint(nlohmann::json{{"exit_direction_start", 361}})));
    }

    TEST_F(FirExitPointCollectionFactoryTest, ExitPointIsInvalidEndDirectionMissing)
    {
        EXPECT_FALSE(ExitPointValid(MakeExitPoint(nlohmann::json::object(), "exit_direction_end")));
    }

    TEST_F(FirExitPointCollectionFactoryTest, ExitPointIsInvalidEndDirectionNotAnInteger)
    {
        EXPECT_FALSE(ExitPointValid(MakeExitPoint(nlohmann::json{{"exit_direction_end", "abc"}})));
    }

    TEST_F(FirExitPointCollectionFactoryTest, ExitPointIsInvalidEndDirectionTooSmall)
    {
        EXPECT_FALSE(ExitPointValid(MakeExitPoint(nlohmann::json{{"exit_direction_end", -1}})));
    }

    TEST_F(FirExitPointCollectionFactoryTest, ExitPointIsInvalidEndDirectionTooLarge)
    {
        EXPECT_FALSE(ExitPointValid(MakeExitPoint(nlohmann::json{{"exit_direction_end", 361}})));
    }
} // namespace UKControllerPluginTest::IntentionCode
