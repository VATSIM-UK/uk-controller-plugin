#include "pch/pch.h"
#include "handoff/HandoffCollectionFactory.h"
#include "handoff/HandoffCollection.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPosition.h"

using UKControllerPlugin::Handoff::HandoffCollection;
using UKControllerPlugin::Handoff::SidMappingValid;
using UKControllerPlugin::Handoff::HandoffOrderValid;
using UKControllerPlugin::Handoff::Create;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using testing::Test;

namespace UKControllerPluginTest {
    namespace Handoff {

        class HandoffCollectionFactoryTest : public Test
        {
            public:
                HandoffCollectionFactoryTest()
                {
                    std::vector<std::string> handoffs = { "EGKK" };
                    controllers.AddPosition(
                        std::make_unique<ControllerPosition>(1, "EGKK_APP", 126.820, handoffs, true, false)
                    );
                    controllers.AddPosition(
                        std::make_unique<ControllerPosition>(2, "LON_S_CTR", 129.420, handoffs, true, false)
                    );
                }

                ControllerPositionCollection controllers;
        };

        TEST_F(HandoffCollectionFactoryTest, SidMappingIsValidIfString)
        {
            EXPECT_TRUE(SidMappingValid("\"hi\""_json));
        }

        TEST_F(HandoffCollectionFactoryTest, SidMappingIsNotValidIfNotString)
        {
            EXPECT_FALSE(SidMappingValid("{}"_json));
        }

        TEST_F(HandoffCollectionFactoryTest, HandoffOrderIsNotValidIfNotArray)
        {
            EXPECT_FALSE(HandoffOrderValid("{}"_json, this->controllers));
        }

        TEST_F(HandoffCollectionFactoryTest, HandoffOrderIsNotValidIfPositionNotString)
        {
            EXPECT_FALSE(HandoffOrderValid("[1]"_json, this->controllers));
        }

        TEST_F(HandoffCollectionFactoryTest, HandoffOrderIsNotValidIfControllerNotInCollection)
        {
            EXPECT_FALSE(HandoffOrderValid("[\"EGKK_TWR\"]"_json, this->controllers));
        }

        TEST_F(HandoffCollectionFactoryTest, HandoffOrderIsNotValidIfNotAllPass)
        {
            EXPECT_FALSE(HandoffOrderValid("[\"EGKK_APP\", \"LON_C_CTR\"]"_json, this->controllers));
        }

        TEST_F(HandoffCollectionFactoryTest, HandoffOrderIsValidIfAllPass)
        {
            EXPECT_TRUE(HandoffOrderValid("[\"EGKK_APP\", \"LON_S_CTR\"]"_json, this->controllers));
        }

        TEST_F(HandoffCollectionFactoryTest, ItReturnsACollectionOfHandoffsAndMappings)
        {
            nlohmann::json handoffs;
            handoffs["EGKK_ADMAG2X"] = nlohmann::json::array({ "EGKK_APP", "LON_S_CTR" });
            nlohmann::json sidMappings;
            sidMappings["EGKK"]["ADMAG2X"] = "EGKK_ADMAG2X";

            std::unique_ptr<HandoffCollection> collection = Create(this->controllers, handoffs, sidMappings);
            EXPECT_EQ(1, collection->CountHandoffs());
            EXPECT_EQ(1, collection->CountSidMappings());
            ASSERT_EQ(2, collection->GetSidHandoffOrder("EGKK", "ADMAG2X").CountPositions());
            ASSERT_EQ("EGKK_APP", collection->GetSidHandoffOrder("EGKK", "ADMAG2X").cbegin()->get().GetCallsign());
            ASSERT_EQ("LON_S_CTR", (++collection->GetSidHandoffOrder("EGKK", "ADMAG2X").cbegin())->get().GetCallsign());
        }

        TEST_F(HandoffCollectionFactoryTest, ItReturnsAnEmptyCollectionIfHandoffsNotValid)
        {
            nlohmann::json handoffs = nlohmann::json::array();
            nlohmann::json sidMappings;
            sidMappings["EGKK"]["ADMAG2X"] = "EGKK_ADMAG2X";

            std::unique_ptr<HandoffCollection> collection = Create(this->controllers, handoffs, sidMappings);
            EXPECT_EQ(0, collection->CountHandoffs());
            EXPECT_EQ(0, collection->CountSidMappings());
        }

        TEST_F(HandoffCollectionFactoryTest, ItIgnoresBadHandoffOrders)
        {
            nlohmann::json handoffs;
            handoffs["EGKK_ADMAG2X"] = nlohmann::json::array({ "EGKK_APP", "LON_C_CTR" });
            nlohmann::json sidMappings;
            sidMappings["EGKK"]["ADMAG2X"] = "EGKK_ADMAG2X";

            std::unique_ptr<HandoffCollection> collection = Create(this->controllers, handoffs, sidMappings);
            EXPECT_EQ(0, collection->CountHandoffs());
            EXPECT_EQ(1, collection->CountSidMappings());
        }

        TEST_F(HandoffCollectionFactoryTest, ItReturnsAnEmptyCollectionIfMappingsNotValid)
        {
            nlohmann::json handoffs;
            handoffs["EGKK_ADMAG2X"] = nlohmann::json::array({ "EGKK_APP", "LON_S_CTR" });
            nlohmann::json sidMappings = nlohmann::json::array();

            std::unique_ptr<HandoffCollection> collection = Create(this->controllers, handoffs, sidMappings);
            EXPECT_EQ(1, collection->CountHandoffs());
            EXPECT_EQ(0, collection->CountSidMappings());
        }

        TEST_F(HandoffCollectionFactoryTest, ItIgnoresBadMappings)
        {
            nlohmann::json handoffs;
            handoffs["EGKK_ADMAG2X"] = nlohmann::json::array({ "EGKK_APP", "LON_S_CTR" });
            nlohmann::json sidMappings;
            sidMappings["EGKK"]["ADMAG2X"] = 123;

            std::unique_ptr<HandoffCollection> collection = Create(this->controllers, handoffs, sidMappings);
            EXPECT_EQ(1, collection->CountHandoffs());
            EXPECT_EQ(0, collection->CountSidMappings());
        }
    }  // namespace Handoff
}  // namespace UKControllerPluginTest
