#include "pch/pch.h"
#include "hold/PublishedHoldCollectionFactory.h"
#include "hold/PublishedHoldCollection.h"
#include "hold/HoldingData.h"
#include "bootstrap/PersistenceContainer.h"

using testing::Test;
using UKControllerPlugin::Hold::CreatePublishedHoldCollection;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::PublishedHoldCollection;
using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPluginTest {
    namespace Hold {

        class PublishedHoldCollectionFactoryTest : public Test
        {
            public:
                PersistenceContainer container;
        };

        TEST_F(PublishedHoldCollectionFactoryTest, ItReturnsEmptyIfDataEmpty)
        {
            nlohmann::json data = "[]"_json;
            EXPECT_EQ(0, CreatePublishedHoldCollection(data, container)->Count());
        }

        TEST_F(PublishedHoldCollectionFactoryTest, ItReturnsEmptyIfDataNotArray)
        {
            nlohmann::json data = nlohmann::json::object();
            EXPECT_EQ(0, CreatePublishedHoldCollection(data, container)->Count());
        }

        TEST_F(PublishedHoldCollectionFactoryTest, ItAddsHoldsWithData)
        {
            nlohmann::json data;
            nlohmann::json hold1;
            hold1["id"] = 1;
            hold1["description"] = "TIMBA";
            hold1["fix"] = "TIMBA";
            hold1["minimum_altitude"] = 7000;
            hold1["maximum_altitude"] = 15000;
            hold1["inbound_heading"] = 309;
            hold1["turn_direction"] = "right";
            hold1["restrictions"] = nlohmann::json::array();
            nlohmann::json hold2;
            hold2["id"] = 2;
            hold2["description"] = "WILLO";
            hold2["fix"] = "WILLO";
            hold2["minimum_altitude"] = 7000;
            hold2["maximum_altitude"] = 15000;
            hold2["inbound_heading"] = 285;
            hold2["turn_direction"] = "left";
            hold2["restrictions"] = nlohmann::json::array();

            data = { hold1, hold2 };
            EXPECT_EQ(2, CreatePublishedHoldCollection(data, container)->Count());
        }

        TEST_F(PublishedHoldCollectionFactoryTest, ItDoesntAddNonObjects)
        {
            nlohmann::json data;
            data = { "Test" };

            EXPECT_EQ(0, CreatePublishedHoldCollection(data, container)->Count());
        }

        TEST_F(PublishedHoldCollectionFactoryTest, ItDoesntAddInvalidData)
        {
            nlohmann::json data;
            nlohmann::json hold;
            hold["id"] = 1;
            hold["description"] = "TIMBA";
            hold["fix"] = "TIMBA Description";
            hold["maximum_altitude"] = 15000;
            hold["inbound_heading"] = 309;
            hold["turn_direction"] = "left";

            data = { hold };

            EXPECT_EQ(0, CreatePublishedHoldCollection(data, container)->Count());
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
