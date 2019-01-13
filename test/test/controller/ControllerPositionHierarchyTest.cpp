#include "pch/pch.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;

namespace UKControllerPluginTest {
    namespace Controller {

        TEST(ControllerPositionHierarchy, AddPositionAddsToHierarchy)
        {
            ControllerPosition position("EGKK_DEL", 121.950, "DEL", { "EGKK" });
            ControllerPositionHierarchy hierarchy;

            hierarchy.AddPosition(position);
            EXPECT_EQ(1, hierarchy.CountPositions());
        }

        TEST(ControllerPositionHierarchy, PositionInHierarchyReturnsTrueIfInHierarchy)
        {
            ControllerPosition position("EGKK_DEL", 121.950, "DEL", { "EGKK" });
            ControllerPositionHierarchy hierarchy;

            hierarchy.AddPosition(position);
            EXPECT_TRUE(hierarchy.PositionInHierarchy(position));
        }

        TEST(ControllerPositionHierarchy, PositionInHierarchyReturnsFalseIfNotInHierarchy)
        {
            ControllerPosition position("EGKK_DEL", 121.950, "DEL", { "EGKK" });
            ControllerPosition position2("EGKK_GND", 121.800, "GND", { "EGKK" });
            ControllerPositionHierarchy hierarchy;

            hierarchy.AddPosition(position);
            EXPECT_FALSE(hierarchy.PositionInHierarchy(position2));
        }

        TEST(ControllerPositionHierarchy, OrderIsDeterminedByFIFO)
        {
            ControllerPosition position1("EGKK_DEL", 121.950, "DEL", { "EGKK" });
            ControllerPosition position2("EGKK_GND", 121.800, "GND", { "EGKK" });
            ControllerPosition position3("EGKK_TWR", 124.220, "TWR", { "EGKK" });
            ControllerPositionHierarchy hierarchy;

            hierarchy.AddPosition(position1);
            hierarchy.AddPosition(position2);
            hierarchy.AddPosition(position3);

            ControllerPositionHierarchy::const_iterator it = hierarchy.cbegin();
            EXPECT_EQ(it++->get(), position1);
            EXPECT_EQ(it++->get(), position2);
            EXPECT_EQ(it++->get(), position3);
        }
    }  // namespace Controller
}  // namespace UKControllerPluginTest
