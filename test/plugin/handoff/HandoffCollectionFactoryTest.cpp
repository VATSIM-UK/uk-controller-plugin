#include "handoff/HandoffCollectionFactory.h"
#include "handoff/HandoffCollection.h"
#include "handoff/HandoffOrder.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPositionHierarchy.h"

using testing::Test;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Handoff::Create;
using UKControllerPlugin::Handoff::HandoffCollection;
using UKControllerPlugin::Handoff::HandoffOrderValid;

namespace UKControllerPluginTest::Handoff {

    class HandoffCollectionFactoryTest : public Test
    {
        public:
        HandoffCollectionFactoryTest()
        {
            std::vector<std::string> handoffs = {"EGKK"};
            controllers.AddPosition(
                std::make_unique<ControllerPosition>(1, "EGKK_APP", 126.820, handoffs, true, false));
            controllers.AddPosition(
                std::make_unique<ControllerPosition>(2, "LON_S_CTR", 129.420, handoffs, true, false));
        }

        ControllerPositionCollection controllers;
    };

    TEST_F(HandoffCollectionFactoryTest, HandoffOrderIsValid)
    {
        const nlohmann::json data = {
            {"id", 1}, {"key", "handoff_key"}, {"controller_positions", nlohmann::json::array({1, 2})}};

        EXPECT_TRUE(HandoffOrderValid(data, this->controllers));
    }

    TEST_F(HandoffCollectionFactoryTest, HandoffOrderIsInvalidIfNoId)
    {
        const nlohmann::json data = {{"key", "handoff_key"}, {"controller_positions", nlohmann::json::array({1, 2})}};

        EXPECT_FALSE(HandoffOrderValid(data, this->controllers));
    }

    TEST_F(HandoffCollectionFactoryTest, HandoffOrderIsInvalidIfIdNotInteger)
    {
        const nlohmann::json data = {
            {"id", "abc"}, {"key", "handoff_key"}, {"controller_positions", nlohmann::json::array({1, 2})}};

        EXPECT_FALSE(HandoffOrderValid(data, this->controllers));
    }

    TEST_F(HandoffCollectionFactoryTest, HandoffOrderIsInvalidIfNoKey)
    {
        const nlohmann::json data = {{"id", 1}, {"controller_positions", nlohmann::json::array({1, 2})}};

        EXPECT_FALSE(HandoffOrderValid(data, this->controllers));
    }

    TEST_F(HandoffCollectionFactoryTest, HandoffOrderIsInvalidIfKeyNotString)
    {
        const nlohmann::json data = {{"id", 1}, {"key", 123}, {"controller_positions", nlohmann::json::array({1, 2})}};

        EXPECT_FALSE(HandoffOrderValid(data, this->controllers));
    }

    TEST_F(HandoffCollectionFactoryTest, HandoffOrderIsInvalidIfNoControllerPositions)
    {
        const nlohmann::json data = {
            {"id", 1},
            {"key", "handoff_key"},
        };

        EXPECT_FALSE(HandoffOrderValid(data, this->controllers));
    }

    TEST_F(HandoffCollectionFactoryTest, HandoffOrderIsInvalidIfControllerPositionsNotArray)
    {
        const nlohmann::json data = {
            {"id", 1}, {"key", "handoff_key"}, {"controller_positions", nlohmann::json::object()}};

        EXPECT_FALSE(HandoffOrderValid(data, this->controllers));
    }

    TEST_F(HandoffCollectionFactoryTest, HandoffOrderIsInvalidIfControllerPositionNotInteger)
    {
        const nlohmann::json data = {
            {"id", 1}, {"key", "handoff_key"}, {"controller_positions", nlohmann::json::array({"abc"})}};

        EXPECT_FALSE(HandoffOrderValid(data, this->controllers));
    }

    TEST_F(HandoffCollectionFactoryTest, HandoffOrderIsInvalidIfControllerPositionsEmpty)
    {
        const nlohmann::json data = {
            {"id", 1}, {"key", "handoff_key"}, {"controller_positions", nlohmann::json::array()}};

        EXPECT_FALSE(HandoffOrderValid(data, this->controllers));
    }

    TEST_F(HandoffCollectionFactoryTest, HandoffOrderIsInvalidIfControllerPositionIsNotValid)
    {
        const nlohmann::json data = {
            {"id", 1}, {"key", "handoff_key"}, {"controller_positions", nlohmann::json::array({1, 2, 55})}};

        EXPECT_FALSE(HandoffOrderValid(data, this->controllers));
    }

    TEST_F(HandoffCollectionFactoryTest, HandoffOrderIsNotValidIfNotObject)
    {
        EXPECT_FALSE(HandoffOrderValid(nlohmann::json::array(), this->controllers));
    }

    TEST_F(HandoffCollectionFactoryTest, ItReturnsACollectionOfHandoffs)
    {
        nlohmann::json handoffs = nlohmann::json::array();
        handoffs.push_back(
            nlohmann::json{{"id", 1}, {"key", "handoff_key"}, {"controller_positions", nlohmann::json::array({1, 2})}});
        handoffs.push_back(
            nlohmann::json{{"id", 2}, {"key", "handoff_key_2"}, {"controller_positions", nlohmann::json::array({1})}});

        std::unique_ptr<HandoffCollection> collection = Create(this->controllers, handoffs);
        EXPECT_EQ(2, collection->Count());
        auto handoff1 = collection->Get(1);
        EXPECT_EQ(1, handoff1->id);
        EXPECT_EQ(2, handoff1->order->CountPositions());
        EXPECT_TRUE(handoff1->order->PositionInHierarchy(*controllers.FetchPositionById(1)));
        EXPECT_TRUE(handoff1->order->PositionInHierarchy(*controllers.FetchPositionById(2)));

        auto handoff2 = collection->Get(2);
        EXPECT_EQ(2, handoff2->id);
        EXPECT_EQ(1, handoff2->order->CountPositions());
        EXPECT_TRUE(handoff2->order->PositionInHierarchy(*controllers.FetchPositionById(1)));
        EXPECT_FALSE(handoff2->order->PositionInHierarchy(*controllers.FetchPositionById(2)));
    }

    TEST_F(HandoffCollectionFactoryTest, ItReturnsAnEmptyCollectionIfHandoffsNotValid)
    {
        std::unique_ptr<HandoffCollection> collection = Create(this->controllers, nlohmann::json::object());
        EXPECT_EQ(0, collection->Count());
    }

    TEST_F(HandoffCollectionFactoryTest, ItIgnoresBadHandoffOrders)
    {
        nlohmann::json handoffs = nlohmann::json::array();
        handoffs.push_back(
            nlohmann::json{{"id", 1}, {"key", "handoff_key"}, {"controller_positions", nlohmann::json::array({1, 2})}});
        handoffs.push_back(
            nlohmann::json{{"id", 2}, {"key", "handoff_key_2"}, {"controller_positions", nlohmann::json::array({55})}});

        std::unique_ptr<HandoffCollection> collection = Create(this->controllers, handoffs);
        EXPECT_EQ(1, collection->Count());
        EXPECT_NE(nullptr, collection->Get(1));
    }
} // namespace UKControllerPluginTest::Handoff
