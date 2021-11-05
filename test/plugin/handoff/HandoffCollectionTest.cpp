#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"
#include "handoff/HandoffCollection.h"
#include "handoff/HandoffOrder.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Handoff::HandoffCollection;
using UKControllerPlugin::Handoff::HandoffOrder;

using ::testing::Test;

namespace UKControllerPluginTest::Handoff {

    class HandoffCollectionTest : public Test
    {
        public:
        HandoffCollectionTest() : position(new ControllerPosition(1, "EGKK_GND", 122.8, {"EGKK"}, true, false))
        {

            this->hierarchy = std::make_shared<ControllerPositionHierarchy>();
            this->hierarchy->AddPosition(*this->position);
            this->handoff1 = std::make_shared<HandoffOrder>(1, this->hierarchy);
            this->handoff2 = std::make_shared<HandoffOrder>(2, this->hierarchy);
        }

        std::unique_ptr<ControllerPosition> position;
        HandoffCollection collection;
        std::shared_ptr<ControllerPositionHierarchy> hierarchy;
        std::shared_ptr<HandoffOrder> handoff1;
        std::shared_ptr<HandoffOrder> handoff2;
    };

    TEST_F(HandoffCollectionTest, ItStartsEmpty)
    {
        EXPECT_EQ(0, this->collection.Count());
    }

    TEST_F(HandoffCollectionTest, ItAddsOrders)
    {
        this->collection.Add(this->handoff1);
        this->collection.Add(this->handoff2);
        EXPECT_EQ(2, this->collection.Count());
        EXPECT_EQ(this->handoff1, this->collection.Get(1));
        EXPECT_EQ(this->handoff2, this->collection.Get(2));
    }

    TEST_F(HandoffCollectionTest, ItDoesntAddDuplidateOrders)
    {
        this->collection.Add(this->handoff1);
        this->collection.Add(this->handoff1);
        this->collection.Add(this->handoff1);
        this->collection.Add(this->handoff2);
        this->collection.Add(this->handoff2);
        this->collection.Add(this->handoff2);
        EXPECT_EQ(2, this->collection.Count());
    }

    TEST_F(HandoffCollectionTest, ItReturnsNullPointerIfNoHandoffOrderFound)
    {
        this->collection.Add(this->handoff1);
        EXPECT_EQ(nullptr, this->collection.Get(2));
    }
} // namespace UKControllerPluginTest::Handoff
