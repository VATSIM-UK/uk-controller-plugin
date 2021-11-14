#include "controller/ControllerPositionHierarchyFactory.h"
#include "controller/ControllerPositionHierarchy.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPosition.h"

using ::testing::Test;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Controller::ControllerPositionHierarchyFactory;

namespace UKControllerPluginTest::Controller {

    class ControllerPositionHierarchyFactoryTest : public Test
    {
        public:
        void SetUp() override
        {
            this->collection = std::make_unique<ControllerPositionCollection>();
            this->collection->AddPosition(std::make_shared<ControllerPosition>(
                1, "EGKK_GND", 121.800, std::vector<std::string>{"EGKK"}, true, false));
            this->collection->AddPosition(std::make_shared<ControllerPosition>(
                2, "EGKK_TWR", 124.220, std::vector<std::string>{"EGKK"}, true, false));
            this->factory = std::make_unique<ControllerPositionHierarchyFactory>(*this->collection);
        };

        std::unique_ptr<ControllerPositionCollection> collection;
        std::unique_ptr<ControllerPositionHierarchyFactory> factory;
    };

    TEST_F(ControllerPositionHierarchyFactoryTest, CreateFromJsonByCallsignThrowsAnExceptionIfNotArray)
    {
        nlohmann::json json = "{}";
        EXPECT_THROW(static_cast<void>(this->factory->CreateFromJsonByCallsign(json)), std::invalid_argument);
    }

    TEST_F(ControllerPositionHierarchyFactoryTest, CreateFromJsonByCallsignThrowsAnExceptionIfEmpty)
    {
        nlohmann::json json = "[]";
        EXPECT_THROW(static_cast<void>(this->factory->CreateFromJsonByCallsign(json)), std::invalid_argument);
    }

    TEST_F(
        ControllerPositionHierarchyFactoryTest, CreateFromJsonByCallsignThrowsAnExceptionIfHierarchyContainsNonString)
    {
        nlohmann::json json = {"EGKK_GND", 123, "EGKK_TWR"};
        EXPECT_THROW(static_cast<void>(this->factory->CreateFromJsonByCallsign(json)), std::invalid_argument);
    }

    TEST_F(ControllerPositionHierarchyFactoryTest, CreateFromJsonByCallsignThrowsAnExceptionIfControllerDoesNotExist)
    {
        nlohmann::json json = {"EGKK_GND", "EGKK_TWR", "EGKK_APP"};
        EXPECT_THROW(static_cast<void>(this->factory->CreateFromJsonByCallsign(json)), std::invalid_argument);
    }

    TEST_F(ControllerPositionHierarchyFactoryTest, ReturnsHierarchyWithCorrectNumberOfPositions)
    {
        nlohmann::json json = {"EGKK_GND", "EGKK_TWR"};
        EXPECT_EQ(2, this->factory->CreateFromJsonByCallsign(json)->CountPositions());
    }

    TEST_F(ControllerPositionHierarchyFactoryTest, ReturnsHierarchyWithCorrectOrderByCallsign)
    {
        nlohmann::json json = {"EGKK_GND", "EGKK_TWR"};
        std::unique_ptr<ControllerPositionHierarchy> hierarchy = this->factory->CreateFromJsonByCallsign(json);
        auto it = hierarchy->cbegin();

        EXPECT_EQ(this->collection->FetchPositionByCallsign("EGKK_GND"), *it++);
        EXPECT_EQ(this->collection->FetchPositionByCallsign("EGKK_TWR"), *it++);
    }

    TEST_F(ControllerPositionHierarchyFactoryTest, ReturnsNullptrIfJsonNotArrayById)
    {
        EXPECT_EQ(nullptr, this->factory->CreateFromJsonById(nlohmann::json::object()));
    }

    TEST_F(ControllerPositionHierarchyFactoryTest, ReturnsNullptrIfControllerInHierarchyIsInvalid)
    {
        auto controllers = nlohmann::json::array({1, 3, 2});
        EXPECT_EQ(nullptr, this->factory->CreateFromJsonById(controllers));
    }

    TEST_F(ControllerPositionHierarchyFactoryTest, ReturnsHierarchyById)
    {
        auto controllers = nlohmann::json::array({2, 1});
        auto hierarchy = this->factory->CreateFromJsonById(controllers);
        EXPECT_NE(nullptr, hierarchy);
        EXPECT_EQ(2, hierarchy->CountPositions());
        EXPECT_EQ(this->collection->FetchPositionById(2), *hierarchy->cbegin());
        EXPECT_EQ(this->collection->FetchPositionById(1), *(++hierarchy->cbegin()));
    }
} // namespace UKControllerPluginTest::Controller
