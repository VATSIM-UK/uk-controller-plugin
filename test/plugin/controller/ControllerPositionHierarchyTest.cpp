#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;

namespace UKControllerPluginTest::Controller {

    class ControllerPositionHierarchyTest : public testing::Test
    {
        public:
        ControllerPositionHierarchyTest()
            : position1(std::make_shared<ControllerPosition>(
                  1, "EGKK_DEL", 121.950, std::vector<std::string>{"EGKK"}, true, false)),
              position2(std::make_shared<ControllerPosition>(
                  2, "EGKK_GND", 121.800, std::vector<std::string>{"EGKK"}, true, false)),
              position3(std::make_shared<ControllerPosition>(
                  3, "EGKK_TWR", 124.220, std::vector<std::string>{"EGKK"}, true, false))
        {
        }

        std::shared_ptr<ControllerPosition> position1;
        std::shared_ptr<ControllerPosition> position2;
        std::shared_ptr<ControllerPosition> position3;
    };

    TEST_F(ControllerPositionHierarchyTest, AddPositionAddsToHierarchy)
    {
        ControllerPositionHierarchy hierarchy;

        hierarchy.AddPosition(position1);
        EXPECT_EQ(1, hierarchy.CountPositions());
    }

    TEST_F(ControllerPositionHierarchyTest, PositionInHierarchyReturnsTrueIfInHierarchy)
    {
        ControllerPositionHierarchy hierarchy;

        hierarchy.AddPosition(position1);
        EXPECT_TRUE(hierarchy.PositionInHierarchy(*position1));
    }

    TEST_F(ControllerPositionHierarchyTest, PositionInHierarchyReturnsFalseIfNotInHierarchy)
    {
        ControllerPositionHierarchy hierarchy;

        hierarchy.AddPosition(position1);
        EXPECT_FALSE(hierarchy.PositionInHierarchy(*position2));
    }

    TEST_F(ControllerPositionHierarchyTest, OrderIsDeterminedByFIFO)
    {
        ControllerPositionHierarchy hierarchy;

        hierarchy.AddPosition(position1);
        hierarchy.AddPosition(position2);
        hierarchy.AddPosition(position3);

        auto it = hierarchy.cbegin();
        EXPECT_EQ(*it++, position1);
        EXPECT_EQ(*it++, position2);
        EXPECT_EQ(*it++, position3);
    }

    TEST_F(ControllerPositionHierarchyTest, EqualityReturnsFalseIfDifferentSizes)
    {
        ControllerPositionHierarchy hierarchy1;
        ControllerPositionHierarchy hierarchy2;

        hierarchy1.AddPosition(position1);
        EXPECT_FALSE(hierarchy1 == hierarchy2);
    }

    TEST_F(ControllerPositionHierarchyTest, EqualityReturnsFalseIfPositions)
    {
        ControllerPositionHierarchy hierarchy1;
        ControllerPositionHierarchy hierarchy2;

        hierarchy1.AddPosition(position1);
        hierarchy2.AddPosition(position2);
        EXPECT_FALSE(hierarchy1 == hierarchy2);
    }

    TEST_F(ControllerPositionHierarchyTest, EqualityReturnsFalseIfWrongOrder)
    {
        ControllerPositionHierarchy hierarchy1;
        ControllerPositionHierarchy hierarchy2;

        hierarchy1.AddPosition(position1);
        hierarchy1.AddPosition(position2);
        hierarchy2.AddPosition(position2);
        hierarchy2.AddPosition(position1);
        EXPECT_FALSE(hierarchy1 == hierarchy2);
    }

    TEST_F(ControllerPositionHierarchyTest, EqualityReturnsTrueIfEqual)
    {
        ControllerPositionHierarchy hierarchy1;
        ControllerPositionHierarchy hierarchy2;

        hierarchy1.AddPosition(position1);
        hierarchy1.AddPosition(position2);
        hierarchy2.AddPosition(position1);
        hierarchy2.AddPosition(position2);
        EXPECT_TRUE(hierarchy1 == hierarchy2);
    }

    TEST_F(ControllerPositionHierarchyTest, PositionPreceedsReturnsTrueIfPositionComesBefore)
    {
        ControllerPositionHierarchy hierarchy;
        hierarchy.AddPosition(position1);
        hierarchy.AddPosition(position2);

        EXPECT_TRUE(hierarchy.PositionPreceeds(*position1, *position2));
    }

    TEST_F(ControllerPositionHierarchyTest, PositionPreceedsReturnsTrueIfSamePositionAndInHierarchy)
    {
        ControllerPositionHierarchy hierarchy;
        hierarchy.AddPosition(position1);
        hierarchy.AddPosition(position2);

        EXPECT_TRUE(hierarchy.PositionPreceeds(*position1, *position1));
    }

    TEST_F(ControllerPositionHierarchyTest, PositionPreceedsReturnsFalseIfPositionComesAfter)
    {
        ControllerPositionHierarchy hierarchy;
        hierarchy.AddPosition(position2);
        hierarchy.AddPosition(position1);

        EXPECT_FALSE(hierarchy.PositionPreceeds(*position1, *position2));
    }

    TEST_F(ControllerPositionHierarchyTest, PositionPreceedsReturnsFalseIfPositionNotInHierarchy)
    {
        ControllerPositionHierarchy hierarchy;
        EXPECT_FALSE(hierarchy.PositionPreceeds(*position1, *position2));
    }
} // namespace UKControllerPluginTest::Controller
