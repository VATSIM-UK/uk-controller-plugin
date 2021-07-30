#include "pch/pch.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;

namespace UKControllerPluginTest {
    namespace Controller {

        class ControllerPositionHierarchyTest : public testing::Test
        {
            public:
                ControllerPositionHierarchyTest()
                    : position1(1, "EGKK_DEL", 121.950, {"EGKK"}, true, false),
                      position2(2, "EGKK_GND", 121.800, {"EGKK"}, true, false),
                      position3(3, "EGKK_TWR", 124.220, {"EGKK"}, true, false)
                { }

                ControllerPosition position1;
                ControllerPosition position2;
                ControllerPosition position3;
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
            EXPECT_TRUE(hierarchy.PositionInHierarchy(position1));
        }

        TEST_F(ControllerPositionHierarchyTest, PositionInHierarchyReturnsFalseIfNotInHierarchy)
        {
            ControllerPositionHierarchy hierarchy;

            hierarchy.AddPosition(position1);
            EXPECT_FALSE(hierarchy.PositionInHierarchy(position2));
        }

        TEST_F(ControllerPositionHierarchyTest, OrderIsDeterminedByFIFO)
        {
            ControllerPositionHierarchy hierarchy;

            hierarchy.AddPosition(position1);
            hierarchy.AddPosition(position2);
            hierarchy.AddPosition(position3);

            ControllerPositionHierarchy::const_iterator it = hierarchy.cbegin();
            EXPECT_EQ(it++->get(), position1);
            EXPECT_EQ(it++->get(), position2);
            EXPECT_EQ(it++->get(), position3);
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
    }  // namespace Controller
}  // namespace UKControllerPluginTest
