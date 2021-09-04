#include "controller/ControllerPositionCollectionFactory.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPosition.h"
#include "mock/MockDependencyLoader.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPositionCollectionFactory;
using UKControllerPluginTest::Dependency::MockDependencyLoader;

using ::testing::ElementsAre;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

namespace UKControllerPluginTest::Controller {

    class ControllerPositionCollectionFactoryTest : public Test
    {
        public:
        NiceMock<MockDependencyLoader> dependency;
    };

    TEST_F(ControllerPositionCollectionFactoryTest, ReturnsEmptyCollectionIfNoDependencyFound)
    {
        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS_V2", nlohmann::json::array()))
            .WillByDefault(Return(nlohmann::json::array()));

        auto collection = ControllerPositionCollectionFactory::Create(dependency);
        EXPECT_EQ(0, collection->GetSize());
    }

    TEST_F(ControllerPositionCollectionFactoryTest, ReturnsEmptyCollectionIfJsonNotArray)
    {
        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS_V2", nlohmann::json::array()))
            .WillByDefault(Return(nlohmann::json::object()));
        auto collection = ControllerPositionCollectionFactory::Create(dependency);
        EXPECT_EQ(0, collection->GetSize());
    }

    TEST_F(ControllerPositionCollectionFactoryTest, ItAddsControllers)
    {
        nlohmann::json data = nlohmann::json::array({
            {
                {"id", 1},
                {"callsign", "EGAA_R_APP"},
                {"frequency", 121.750},
                {"top_down", nlohmann::json::array({"EGAA", "EGAC"})},
                {"requests_departure_releases", true},
                {"receives_departure_releases", true},
                {"sends_prenotes", true},
                {"receives_prenotes", true},
            }, // Normal frequency
            {
                {"id", 2},
                {"callsign", "EGAA_TWR"},
                {"frequency", 123},
                {"top_down", nlohmann::json::array({"EGAA"})},
                {"requests_departure_releases", true},
                {"receives_departure_releases", false},
                {"sends_prenotes", true},
                {"receives_prenotes", false},
            }, // Frequency ending with all zeros - comes out as integer in dependency
            {
                {"id", 3},
                {"callsign", "EGAA_GND"},
                {"frequency", 123.450},
                {"top_down", nlohmann::json::array()},
                {"requests_departure_releases", false},
                {"receives_departure_releases", false},
                {"sends_prenotes", false},
                {"receives_prenotes", false},
            } // No top-down
        });

        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS_V2", nlohmann::json::array()))
            .WillByDefault(Return(data));

        auto collection = ControllerPositionCollectionFactory::Create(dependency);
        EXPECT_EQ(3, collection->GetSize());

        ControllerPosition positionNormal = collection->FetchPositionByCallsign("EGAA_R_APP");
        EXPECT_EQ(1, positionNormal.GetId());
        EXPECT_EQ("EGAA_R_APP", positionNormal.GetCallsign());
        EXPECT_EQ(121.750, positionNormal.GetFrequency());
        EXPECT_EQ(std::vector<std::string>({"EGAA", "EGAC"}), positionNormal.GetTopdown());
        EXPECT_TRUE(positionNormal.RequestsDepartureReleases());
        EXPECT_TRUE(positionNormal.ReceivesDepartureReleases());
        EXPECT_TRUE(positionNormal.SendsPrenoteMessages());
        EXPECT_TRUE(positionNormal.ReceivesPrenoteMessages());

        ControllerPosition positionAllZeroFrequency = collection->FetchPositionByCallsign("EGAA_TWR");
        EXPECT_EQ(2, positionAllZeroFrequency.GetId());
        EXPECT_EQ("EGAA_TWR", positionAllZeroFrequency.GetCallsign());
        EXPECT_EQ(123.000, positionAllZeroFrequency.GetFrequency());
        EXPECT_EQ(std::vector<std::string>({"EGAA"}), positionAllZeroFrequency.GetTopdown());
        EXPECT_TRUE(positionAllZeroFrequency.RequestsDepartureReleases());
        EXPECT_FALSE(positionAllZeroFrequency.ReceivesDepartureReleases());
        EXPECT_TRUE(positionAllZeroFrequency.SendsPrenoteMessages());
        EXPECT_FALSE(positionAllZeroFrequency.ReceivesPrenoteMessages());

        ControllerPosition positionNoTopDown = collection->FetchPositionByCallsign("EGAA_GND");
        EXPECT_EQ(3, positionNoTopDown.GetId());
        EXPECT_EQ("EGAA_GND", positionNoTopDown.GetCallsign());
        EXPECT_EQ(123.450, positionNoTopDown.GetFrequency());
        EXPECT_EQ(std::vector<std::string>(), positionNoTopDown.GetTopdown());
        EXPECT_FALSE(positionNoTopDown.RequestsDepartureReleases());
        EXPECT_FALSE(positionNoTopDown.ReceivesDepartureReleases());
        EXPECT_FALSE(positionNoTopDown.SendsPrenoteMessages());
        EXPECT_FALSE(positionNoTopDown.ReceivesPrenoteMessages());
    }

    TEST_F(ControllerPositionCollectionFactoryTest, ItDoesntAddControllerWithMissingId)
    {
        nlohmann::json data = nlohmann::json::array({
            {
                {"callsign", "EGAA_R_APP"},
                {"frequency", 121.750},
                {"top_down", nlohmann::json::array({"EGAA", "EGAC"})},
                {"requests_departure_releases", true},
                {"receives_departure_releases", true},
                {"sends_prenotes", false},
                {"receives_prenotes", false},
            },
        });

        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS_V2", nlohmann::json::array()))
            .WillByDefault(Return(data));

        auto collection = ControllerPositionCollectionFactory::Create(dependency);
        EXPECT_EQ(0, collection->GetSize());
    }

    TEST_F(ControllerPositionCollectionFactoryTest, ItDoesntAddControllerWithIdNotInteger)
    {
        nlohmann::json data = nlohmann::json::array({
            {
                {"id", "abc"},
                {"callsign", "EGAA_R_APP"},
                {"frequency", 121.750},
                {"top_down", nlohmann::json::array({"EGAA", "EGAC"})},
                {"requests_departure_releases", true},
                {"receives_departure_releases", true},
                {"sends_prenotes", false},
                {"receives_prenotes", false},
            },
        });

        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS_V2", nlohmann::json::array()))
            .WillByDefault(Return(data));

        auto collection = ControllerPositionCollectionFactory::Create(dependency);
        EXPECT_EQ(0, collection->GetSize());
    }

    TEST_F(ControllerPositionCollectionFactoryTest, ItDoesntAddControllerWithMissingCallsign)
    {
        nlohmann::json data = nlohmann::json::array({
            {
                {"id", 1},
                {"frequency", 121.750},
                {"top_down", nlohmann::json::array({"EGAA", "EGAC"})},
                {"requests_departure_releases", true},
                {"receives_departure_releases", true},
                {"sends_prenotes", false},
                {"receives_prenotes", false},
            },
        });

        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS_V2", nlohmann::json::array()))
            .WillByDefault(Return(data));

        auto collection = ControllerPositionCollectionFactory::Create(dependency);
        EXPECT_EQ(0, collection->GetSize());
    }

    TEST_F(ControllerPositionCollectionFactoryTest, ItDoesntAddControllerWithCallsignNotString)
    {
        nlohmann::json data = nlohmann::json::array({
            {
                {"id", 1},
                {"callsign", 123},
                {"frequency", 121.750},
                {"top_down", nlohmann::json::array({"EGAA", "EGAC"})},
                {"requests_departure_releases", true},
                {"receives_departure_releases", true},
                {"sends_prenotes", false},
                {"receives_prenotes", false},
            },
        });

        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS_V2", nlohmann::json::array()))
            .WillByDefault(Return(data));

        auto collection = ControllerPositionCollectionFactory::Create(dependency);
        EXPECT_EQ(0, collection->GetSize());
    }

    TEST_F(ControllerPositionCollectionFactoryTest, ItDoesntAddControllerWithFrequencyMissing)
    {
        nlohmann::json data = nlohmann::json::array({
            {
                {"id", 1},
                {"callsign", "EGAA_R_APP"},
                {"top_down", nlohmann::json::array({"EGAA", "EGAC"})},
                {"requests_departure_releases", true},
                {"receives_departure_releases", true},
                {"sends_prenotes", false},
                {"receives_prenotes", false},
            },
        });

        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS_V2", nlohmann::json::array()))
            .WillByDefault(Return(data));

        auto collection = ControllerPositionCollectionFactory::Create(dependency);
        EXPECT_EQ(0, collection->GetSize());
    }

    TEST_F(ControllerPositionCollectionFactoryTest, ItDoesntAddControllerWithFrequencyNotFloat)
    {
        nlohmann::json data = nlohmann::json::array({
            {
                {"id", 1},
                {"callsign", "EGAA_R_APP"},
                {"frequency", "abc"},
                {"top_down", nlohmann::json::array({"EGAA", "EGAC"})},
                {"requests_departure_releases", true},
                {"receives_departure_releases", true},
                {"sends_prenotes", false},
                {"receives_prenotes", false},
            },
        });

        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS_V2", nlohmann::json::array()))
            .WillByDefault(Return(data));

        auto collection = ControllerPositionCollectionFactory::Create(dependency);
        EXPECT_EQ(0, collection->GetSize());
    }

    TEST_F(ControllerPositionCollectionFactoryTest, ItDoesntAddControllerWithTopDownMissing)
    {
        nlohmann::json data = nlohmann::json::array({
            {
                {"id", 1},
                {"callsign", "EGAA_R_APP"},
                {"frequency", 121.750},
                {"requests_departure_releases", true},
                {"receives_departure_releases", true},
                {"sends_prenotes", false},
                {"receives_prenotes", false},
            },
        });

        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS_V2", nlohmann::json::array()))
            .WillByDefault(Return(data));

        auto collection = ControllerPositionCollectionFactory::Create(dependency);
        EXPECT_EQ(0, collection->GetSize());
    }

    TEST_F(ControllerPositionCollectionFactoryTest, ItDoesntAddControllerWithTopDownNotArray)
    {
        nlohmann::json data = nlohmann::json::array({
            {
                {"id", 1},
                {"callsign", "EGAA_R_APP"},
                {"frequency", 121.750},
                {"top_down", "abc"},
                {"requests_departure_releases", true},
                {"receives_departure_releases", true},
                {"sends_prenotes", false},
                {"receives_prenotes", false},
            },
        });

        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS_V2", nlohmann::json::array()))
            .WillByDefault(Return(data));

        auto collection = ControllerPositionCollectionFactory::Create(dependency);
        EXPECT_EQ(0, collection->GetSize());
    }

    TEST_F(ControllerPositionCollectionFactoryTest, ItDoesntAddControllerWithTopDownInvalidItem)
    {
        nlohmann::json data = nlohmann::json::array({
            {
                {"id", 1},
                {"callsign", "EGAA_R_APP"},
                {"frequency", 121.750},
                {"top_down", nlohmann::json::array({"EGAA", 123})},
                {"requests_departure_releases", true},
                {"receives_departure_releases", true},
                {"sends_prenotes", false},
                {"receives_prenotes", false},
            },
        });

        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS_V2", nlohmann::json::array()))
            .WillByDefault(Return(data));

        auto collection = ControllerPositionCollectionFactory::Create(dependency);
        EXPECT_EQ(0, collection->GetSize());
    }

    TEST_F(ControllerPositionCollectionFactoryTest, ItDoesntAddControllerWithRequestsDepartureReleasesMissing)
    {
        nlohmann::json data = nlohmann::json::array({
            {
                {"id", 1},
                {"callsign", "EGAA_R_APP"},
                {"frequency", 121.750},
                {"top_down", nlohmann::json::array({"EGAA", "EGAC"})},
                {"receives_departure_releases", true},
                {"sends_prenotes", false},
                {"receives_prenotes", false},
            },
        });

        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS_V2", nlohmann::json::array()))
            .WillByDefault(Return(data));

        auto collection = ControllerPositionCollectionFactory::Create(dependency);
        EXPECT_EQ(0, collection->GetSize());
    }

    TEST_F(ControllerPositionCollectionFactoryTest, ItDoesntAddControllerWithRequestsDepartureReleasesNotBoolean)
    {
        nlohmann::json data = nlohmann::json::array({
            {
                {"id", 1},
                {"callsign", "EGAA_R_APP"},
                {"frequency", 121.750},
                {"top_down", nlohmann::json::array({"EGAA", "EGAC"})},
                {"requests_departure_releases", true},
                {"receives_departure_releases", "abc"},
                {"sends_prenotes", false},
                {"receives_prenotes", false},
            },
        });

        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS_V2", nlohmann::json::array()))
            .WillByDefault(Return(data));

        auto collection = ControllerPositionCollectionFactory::Create(dependency);
        EXPECT_EQ(0, collection->GetSize());
    }

    TEST_F(ControllerPositionCollectionFactoryTest, ItDoesntAddControllerWithReceivesDepartureReleasesMissing)
    {
        nlohmann::json data = nlohmann::json::array({
            {
                {"id", 1},
                {"callsign", "EGAA_R_APP"},
                {"frequency", 121.750},
                {"top_down", nlohmann::json::array({"EGAA", "EGAC"})},
                {"requests_departure_releases", true},
                {"sends_prenotes", false},
                {"receives_prenotes", false},
            },
        });

        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS_V2", nlohmann::json::array()))
            .WillByDefault(Return(data));

        auto collection = ControllerPositionCollectionFactory::Create(dependency);
        EXPECT_EQ(0, collection->GetSize());
    }

    TEST_F(ControllerPositionCollectionFactoryTest, ItDoesntAddControllerWithReceivesDepartureReleasesNotBoolean)
    {
        nlohmann::json data = nlohmann::json::array({
            {
                {"id", 1},
                {"callsign", "EGAA_R_APP"},
                {"frequency", 121.750},
                {"top_down", nlohmann::json::array({"EGAA", "EGAC"})},
                {"requests_departure_releases", true},
                {"receives_departure_releases", "abc"},
                {"sends_prenotes", false},
                {"receives_prenotes", false},
            },
        });

        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS_V2", nlohmann::json::array()))
            .WillByDefault(Return(data));

        auto collection = ControllerPositionCollectionFactory::Create(dependency);
        EXPECT_EQ(0, collection->GetSize());
    }

    TEST_F(ControllerPositionCollectionFactoryTest, ItDoesntAddControllerWithSendsPrenotesMissing)
    {
        nlohmann::json data = nlohmann::json::array({
            {
                {"id", 1},
                {"callsign", "EGAA_R_APP"},
                {"frequency", 121.750},
                {"top_down", nlohmann::json::array({"EGAA", "EGAC"})},
                {"requests_departure_releases", true},
                {"receives_departure_releases", true},
                {"receives_prenotes", false},
            },
        });

        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS_V2", nlohmann::json::array()))
            .WillByDefault(Return(data));

        auto collection = ControllerPositionCollectionFactory::Create(dependency);
        EXPECT_EQ(0, collection->GetSize());
    }

    TEST_F(ControllerPositionCollectionFactoryTest, ItDoesntAddControllerWithSendsPrenotesNotBoolean)
    {
        nlohmann::json data = nlohmann::json::array({
            {
                {"id", 1},
                {"callsign", "EGAA_R_APP"},
                {"frequency", 121.750},
                {"top_down", nlohmann::json::array({"EGAA", "EGAC"})},
                {"requests_departure_releases", true},
                {"receives_departure_releases", true},
                {"sends_prenotes", "abc"},
                {"receives_prenotes", false},
            },
        });

        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS_V2", nlohmann::json::array()))
            .WillByDefault(Return(data));

        auto collection = ControllerPositionCollectionFactory::Create(dependency);
        EXPECT_EQ(0, collection->GetSize());
    }

    TEST_F(ControllerPositionCollectionFactoryTest, ItDoesntAddControllerWithReceivesPrenotesMissing)
    {
        nlohmann::json data = nlohmann::json::array({
            {
                {"id", 1},
                {"callsign", "EGAA_R_APP"},
                {"frequency", 121.750},
                {"top_down", nlohmann::json::array({"EGAA", "EGAC"})},
                {"requests_departure_releases", true},
                {"receives_departure_releases", true},
                {"sends_prenotes", false},
            },
        });

        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS_V2", nlohmann::json::array()))
            .WillByDefault(Return(data));

        auto collection = ControllerPositionCollectionFactory::Create(dependency);
        EXPECT_EQ(0, collection->GetSize());
    }

    TEST_F(ControllerPositionCollectionFactoryTest, ItDoesntAddControllerWithReceivesPrenotesNotBoolean)
    {
        nlohmann::json data = nlohmann::json::array({
            {
                {"id", 1},
                {"callsign", "EGAA_R_APP"},
                {"frequency", 121.750},
                {"top_down", nlohmann::json::array({"EGAA", "EGAC"})},
                {"requests_departure_releases", true},
                {"receives_departure_releases", true},
                {"sends_prenotes", false},
                {"receives_prenotes", "abc"},
            },
        });

        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS_V2", nlohmann::json::array()))
            .WillByDefault(Return(data));

        auto collection = ControllerPositionCollectionFactory::Create(dependency);
        EXPECT_EQ(0, collection->GetSize());
    }
} // namespace UKControllerPluginTest::Controller
