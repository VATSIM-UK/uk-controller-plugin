#include "controller/ControllerPositionHierarchyFactory.h"
#include "controller/ControllerPositionHierarchy.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPosition.h"

using ::testing::Test;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Controller::ControllerPositionHierarchyFactory;

namespace UKControllerPluginTest {
    namespace Controller {

        class ControllerPositionHierarchyFactoryTest : public Test
        {
            public:
            void SetUp(void)
            {
                this->collection = std::make_unique<ControllerPositionCollection>();
                this->collection->AddPosition(std::unique_ptr<ControllerPosition>(
                    new ControllerPosition(1, "EGKK_GND", 121.800, {"EGKK"}, true, false)));
                this->collection->AddPosition(std::unique_ptr<ControllerPosition>(
                    new ControllerPosition(2, "EGKK_TWR", 124.220, {"EGKK"}, true, false)));
                this->factory = std::make_unique<ControllerPositionHierarchyFactory>(*this->collection);
            };

            std::unique_ptr<ControllerPositionCollection> collection;
            std::unique_ptr<ControllerPositionHierarchyFactory> factory;
        };

        TEST_F(ControllerPositionHierarchyFactoryTest, CreateFromJsonThrowsAnExceptionIfNotArray)
        {
            nlohmann::json json = "{}";
            EXPECT_THROW(this->factory->CreateFromJson(json), std::invalid_argument);
        }

        TEST_F(ControllerPositionHierarchyFactoryTest, CreateFromJsonThrowsAnExceptionIfEmpty)
        {
            nlohmann::json json = "[]";
            EXPECT_THROW(this->factory->CreateFromJson(json), std::invalid_argument);
        }

        TEST_F(ControllerPositionHierarchyFactoryTest, CreateFromJsonThrowsAnExceptionIfHierarchyContainsNonString)
        {
            nlohmann::json json = {"EGKK_GND", 123, "EGKK_TWR"};
            EXPECT_THROW(this->factory->CreateFromJson(json), std::invalid_argument);
        }

        TEST_F(ControllerPositionHierarchyFactoryTest, CreateFromJsonThrowsAnExceptionIfControllerDoesNotExist)
        {
            nlohmann::json json = {"EGKK_GND", "EGKK_TWR", "EGKK_APP"};
            EXPECT_THROW(this->factory->CreateFromJson(json), std::out_of_range);
        }

        TEST_F(ControllerPositionHierarchyFactoryTest, ReturnsHierarchyWithCorrectNumberOfPositions)
        {
            nlohmann::json json = {"EGKK_GND", "EGKK_TWR"};
            EXPECT_EQ(2, this->factory->CreateFromJson(json)->CountPositions());
        }

        TEST_F(ControllerPositionHierarchyFactoryTest, ReturnsHierarchyWithCorrectOrder)
        {
            nlohmann::json json = {"EGKK_GND", "EGKK_TWR"};
            std::unique_ptr<ControllerPositionHierarchy> hierarchy = this->factory->CreateFromJson(json);
            ControllerPositionHierarchy::const_iterator it = hierarchy->cbegin();

            EXPECT_EQ(this->collection->FetchPositionByCallsign("EGKK_GND"), *it++);
            EXPECT_EQ(this->collection->FetchPositionByCallsign("EGKK_TWR"), *it++);
        }
    } // namespace Controller
} // namespace UKControllerPluginTest
