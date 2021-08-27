#include "bootstrap/PersistenceContainer.h"
#include "mock/MockDependencyLoader.h"
#include "navaids/NavaidCollection.h"
#include "navaids/NavaidModule.h"
#include "sectorfile/SectorFileCoordinates.h"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Navaids::BootstrapPlugin;
using UKControllerPlugin::Navaids::NavaidValid;
using UKControllerPlugin::SectorFile::ParseSectorFileCoordinates;
using UKControllerPluginTest::Dependency::MockDependencyLoader;

namespace UKControllerPluginTest::Navaids {

    class NavaidModuleTest : public Test
    {
        public:
        NiceMock<MockDependencyLoader> dependency;
        PersistenceContainer container;
    };

    TEST_F(NavaidModuleTest, NavaidValidReturnsTrueIfValid)
    {
        nlohmann::json data = {
            {"id", 1}, {"identifier", "TIMBA"}, {"latitude", "N051.18.18.000"}, {"longitude", "W000.26.50.000"}};
        EXPECT_TRUE(NavaidValid(data));
    }

    TEST_F(NavaidModuleTest, NavaidValidReturnsFalseIfIdNotInteger)
    {
        nlohmann::json data = {
            {"id", "test"}, {"latitude", "N051.18.18.000"}, {"longitude", "W000.26.50.000"}, {"identifier", "TIMBA"}};
        EXPECT_FALSE(NavaidValid(data));
    }

    TEST_F(NavaidModuleTest, NavaidValidReturnsFalseIfNoId)
    {
        nlohmann::json data = {
            {"latitude", "N051.18.18.000"}, {"longitude", "W000.26.50.000"}, {"identifier", "TIMBA"}};
        EXPECT_FALSE(NavaidValid(data));
    }

    TEST_F(NavaidModuleTest, NavaidValidReturnsFalseIfIdentifierNotString)
    {
        nlohmann::json data = {
            {"id", 1}, {"latitude", "N051.18.18.000"}, {"longitude", "W000.26.50.000"}, {"identifier", 1}};
        EXPECT_FALSE(NavaidValid(data));
    }

    TEST_F(NavaidModuleTest, NavaidValidReturnsFalseIfIdentifierMissing)
    {
        nlohmann::json data = {
            {"latitude", "N051.18.18.000"},
            {"longitude", "W000.26.50.000"},
            {"id", 1},
        };
        EXPECT_FALSE(NavaidValid(data));
    }

    TEST_F(NavaidModuleTest, NavaidValidReturnsFalseIfLatitudeInvalid)
    {
        nlohmann::json data = {
            {"id", 1}, {"latitude", "abc"}, {"longitude", "W000.26.50.000"}, {"identifier", "TIMBA"}};
        EXPECT_FALSE(NavaidValid(data));
    }

    TEST_F(NavaidModuleTest, NavaidValidReturnsFalseIfLatitudeNotString)
    {
        nlohmann::json data = {{"id", 1}, {"latitude", 123}, {"longitude", "W000.26.50.000"}, {"identifier", "TIMBA"}};
        EXPECT_FALSE(NavaidValid(data));
    }

    TEST_F(NavaidModuleTest, NavaidValidReturnsFalseIfLatitudeMissing)
    {
        nlohmann::json data = {{"id", 1}, {"longitude", "W000.26.50.000"}, {"identifier", "TIMBA"}};
        EXPECT_FALSE(NavaidValid(data));
    }

    TEST_F(NavaidModuleTest, NavaidValidReturnsFalseIfLongitudeInvalid)
    {
        nlohmann::json data = {
            {"id", 1}, {"latitude", "W000.26.50.000"}, {"longitude", "abc"}, {"identifier", "TIMBA"}};
        EXPECT_FALSE(NavaidValid(data));
    }

    TEST_F(NavaidModuleTest, NavaidValidReturnsFalseIfLongitudeNotString)
    {
        nlohmann::json data = {{"id", 1}, {"longitude", 123}, {"latitude", "W000.26.50.000"}, {"identifier", "TIMBA"}};
        EXPECT_FALSE(NavaidValid(data));
    }

    TEST_F(NavaidModuleTest, NavaidValidReturnsFalseIfLongitudeMissing)
    {
        nlohmann::json data = {{"id", 1}, {"latitude", "W000.26.50.000"}, {"identifier", "TIMBA"}};
        EXPECT_FALSE(NavaidValid(data));
    }

    TEST_F(NavaidModuleTest, NavaidValidReturnsFalseIfNotObject)
    {
        EXPECT_FALSE(NavaidValid(nlohmann::json::array()));
    }

    TEST_F(NavaidModuleTest, BootstrapPluginHandlesInvalidDependency)
    {
        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_NAVAIDS", "{}"_json))
            .WillByDefault(Return(nlohmann::json::object()));

        BootstrapPlugin(this->container, this->dependency);
        EXPECT_EQ(0, this->container.navaids->Count());
    }

    TEST_F(NavaidModuleTest, BootstrapPluginHandlesInvalidNavaid)
    {
        nlohmann::json data = nlohmann::json::array();
        data.push_back({{"id", "nah"}});

        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_NAVAIDS", "{}"_json)).WillByDefault(Return(data));

        BootstrapPlugin(this->container, this->dependency);
        EXPECT_EQ(0, this->container.navaids->Count());
    }

    TEST_F(NavaidModuleTest, BootstrapPluginLoadsNavaids)
    {
        nlohmann::json data = nlohmann::json::array();
        data.push_back(
            {{"id", 1}, {"identifier", "TIMBA"}, {"latitude", "N051.18.18.000"}, {"longitude", "W000.26.50.000"}});
        data.push_back(
            {{"id", 2}, {"identifier", "WILLO"}, {"latitude", "N051.18.18.000"}, {"longitude", "W000.26.50.000"}});

        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_NAVAIDS", "{}"_json)).WillByDefault(Return(data));

        EuroScopePlugIn::CPosition expectedPosition = ParseSectorFileCoordinates("N051.18.18.000", "W000.26.50.000");

        BootstrapPlugin(this->container, this->dependency);
        EXPECT_EQ(2, this->container.navaids->Count());
        EXPECT_EQ("TIMBA", this->container.navaids->GetByIdentifier("TIMBA").identifier);
        EXPECT_EQ(
            expectedPosition.m_Latitude, this->container.navaids->GetByIdentifier("TIMBA").coordinates.m_Latitude);

        EXPECT_EQ(
            expectedPosition.m_Longitude, this->container.navaids->GetByIdentifier("TIMBA").coordinates.m_Longitude);
        EXPECT_EQ("WILLO", this->container.navaids->GetByIdentifier("WILLO").identifier);
        EXPECT_EQ(
            expectedPosition.m_Latitude, this->container.navaids->GetByIdentifier("WILLO").coordinates.m_Latitude);

        EXPECT_EQ(
            expectedPosition.m_Longitude, this->container.navaids->GetByIdentifier("WILLO").coordinates.m_Longitude);
    }
} // namespace UKControllerPluginTest::Navaids
