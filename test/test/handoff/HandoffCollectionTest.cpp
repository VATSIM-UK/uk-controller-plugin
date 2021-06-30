#include "pch/pch.h"
#include "handoff/HandoffCollection.h"
#include "controller/ControllerPositionHierarchy.h"
#include "controller/ControllerPosition.h"

using UKControllerPlugin::Handoff::HandoffCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;

using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Handoff {

        class HandoffCollectionTest : public Test
        {
            public:

                HandoffCollectionTest()
                    : position(new ControllerPosition(1, "EGKK_GND", 122.8, {"EGKK"}, true, false))
                {
                    this->hierarchy1.reset(new ControllerPositionHierarchy);
                    this->hierarchy1->AddPosition(*this->position);
                    this->hierarchy2.reset(new ControllerPositionHierarchy);
                    this->hierarchy2->AddPosition(*this->position);
                }

                std::unique_ptr<ControllerPosition> position;
                HandoffCollection collection;
                std::shared_ptr<ControllerPositionHierarchy> hierarchy1;
                std::shared_ptr<ControllerPositionHierarchy> hierarchy2;
        };

        TEST_F(HandoffCollectionTest, ItStartsWithNoSids)
        {
            EXPECT_EQ(0, this->collection.CountSidMappings());
        }

        TEST_F(HandoffCollectionTest, ItStartsWithNoHandoffs)
        {
            EXPECT_EQ(0, this->collection.CountHandoffs());
        }

        TEST_F(HandoffCollectionTest, ItAddsHandoffOrders)
        {
            this->collection.AddHandoffOrder("TEST_1", this->hierarchy1);
            this->collection.AddHandoffOrder("TEST_2", this->hierarchy2);

            EXPECT_EQ(2, this->collection.CountHandoffs());
        }

        TEST_F(HandoffCollectionTest, ItDoesntAddDuplicateHandoffs)
        {
            this->collection.AddHandoffOrder("TEST_1", this->hierarchy1);
            this->collection.AddHandoffOrder("TEST_1", this->hierarchy2);

            EXPECT_EQ(1, this->collection.CountHandoffs());
        }

        TEST_F(HandoffCollectionTest, ItAddsSidMappings)
        {
            this->collection.AddSidMapping("EGKK", "ADMAG2X", "TEST_1");
            this->collection.AddSidMapping("EGKK", "SFD4Z", "TEST_2");

            EXPECT_EQ(2, this->collection.CountSidMappings());
        }

        TEST_F(HandoffCollectionTest, ItDoesntAddDuplicateSidMappings)
        {
            this->collection.AddSidMapping("EGKK", "ADMAG2X", "TEST_1");
            this->collection.AddSidMapping("EGKK", "ADMAG2X", "TEST_2");

            EXPECT_EQ(1, this->collection.CountSidMappings());
        }

        TEST_F(HandoffCollectionTest, ItReturnsControllerHierarchiesIfSidMappingsValid)
        {
            this->collection.AddHandoffOrder("TEST_1", this->hierarchy1);
            this->collection.AddHandoffOrder("TEST_2", this->hierarchy2);
            this->collection.AddSidMapping("EGKK", "ADMAG2X", "TEST_1");
            this->collection.AddSidMapping("EGKK", "SFD4Z", "TEST_2");

            EXPECT_EQ(*this->hierarchy1, this->collection.GetSidHandoffOrder("EGKK", "ADMAG2X"));
            EXPECT_EQ(*this->hierarchy2, this->collection.GetSidHandoffOrder("EGKK", "SFD4Z"));
        }

        TEST_F(HandoffCollectionTest, ItReturnsInvalidMappingIfSidNotFound)
        {
            this->collection.AddHandoffOrder("TEST_1", this->hierarchy1);
            this->collection.AddHandoffOrder("TEST_2", this->hierarchy2);
            this->collection.AddSidMapping("EGKK", "ADMAG2X", "TEST_1");
            this->collection.AddSidMapping("EGKK", "SFD4Z", "TEST_2");

            EXPECT_EQ(this->collection.invalidHierarchy, this->collection.GetSidHandoffOrder("EGKK", "BIG1X"));
        }

        TEST_F(HandoffCollectionTest, ItReturnsInvalidMappingIfSidFoundButNoHierarchy)
        {
            this->collection.AddHandoffOrder("TEST_1", this->hierarchy1);
            this->collection.AddHandoffOrder("TEST_2", this->hierarchy2);
            this->collection.AddSidMapping("EGKK", "ADMAG2X", "TEST_1");
            this->collection.AddSidMapping("EGKK", "SFD4Z", "TEST_3");

            EXPECT_EQ(this->collection.invalidHierarchy, this->collection.GetSidHandoffOrder("EGKK", "SFD4Z"));
        }
    }  // namespace Handoff
}  // namespace UKControllerPluginTest
