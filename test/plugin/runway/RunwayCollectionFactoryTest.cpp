#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "controller/ControllerPositionHierarchy.h"
#include "runway/Runway.h"
#include "runway/RunwayCollection.h"
#include "runway/RunwayCollectionFactory.h"

using UKControllerPlugin::Runway::BuildRunwayCollection;
using UKControllerPlugin::Runway::RunwayValid;

namespace UKControllerPluginTest::Runway {
    class RunwayCollectionFactoryTest : public testing::Test
    {
        public:
        RunwayCollectionFactoryTest()
        {
            airfields.AddAirfield(std::make_shared<UKControllerPlugin::Airfield::AirfieldModel>(2, "EGKK", nullptr));
            airfields.AddAirfield(std::make_shared<UKControllerPlugin::Airfield::AirfieldModel>(3, "EGLL", nullptr));
        }

        static auto
        MakeRunway(const nlohmann::json& overridingData = nlohmann::json::object(), const std::string& keyToRemove = "")
            -> const nlohmann::json
        {
            nlohmann::json runway{
                {"id", 1},
                {"airfield_id", 2},
                {"identifier", "27L"},
                {"heading", 123},
                {"threshold_latitude", 1.2},
                {"threshold_longitude", 3.4},
                {"threshold_elevation", 201},
                {"glideslope_angle", 3.0}};
            runway.update(overridingData);

            if (!keyToRemove.empty()) {
                runway.erase(runway.find(keyToRemove));
            }

            return runway;
        };

        UKControllerPlugin::Airfield::AirfieldCollection airfields;
    };

    TEST_F(RunwayCollectionFactoryTest, RunwayIsValid)
    {
        EXPECT_TRUE(RunwayValid(MakeRunway(), airfields));
    }

    TEST_F(RunwayCollectionFactoryTest, RunwayIsValidIntegerThreshold)
    {
        EXPECT_TRUE(
            RunwayValid(MakeRunway(nlohmann::json{{"threshold_latitude", 1}, {"threshold_longitude", 2}}), airfields));
    }

    TEST_F(RunwayCollectionFactoryTest, RunwayIsInvalidIfIdMissing)
    {
        EXPECT_FALSE(RunwayValid(MakeRunway(nlohmann::json::object(), "id"), airfields));
    }

    TEST_F(RunwayCollectionFactoryTest, RunwayIsInvalidIfIdNotInteger)
    {
        EXPECT_FALSE(RunwayValid(MakeRunway(nlohmann::json{{"id", "abc"}}), airfields));
    }

    TEST_F(RunwayCollectionFactoryTest, RunwayIsInvalidIfAirfieldIdMissing)
    {
        EXPECT_FALSE(RunwayValid(MakeRunway(nlohmann::json::object(), "airfield_id"), airfields));
    }

    TEST_F(RunwayCollectionFactoryTest, RunwayIsInvalidIfAirfieldIdNotInteger)
    {
        EXPECT_FALSE(RunwayValid(MakeRunway(nlohmann::json{{"airfield_id", "abc"}}), airfields));
    }

    TEST_F(RunwayCollectionFactoryTest, RunwayIsInvalidIfAirfieldIdNotValidAirfield)
    {
        EXPECT_FALSE(RunwayValid(MakeRunway(nlohmann::json{{"airfield_id", 55}}), airfields));
    }

    TEST_F(RunwayCollectionFactoryTest, RunwayIsInvalidIdentifierMissing)
    {
        EXPECT_FALSE(RunwayValid(MakeRunway(nlohmann::json::object(), "identifier"), airfields));
    }

    TEST_F(RunwayCollectionFactoryTest, RunwayIsInvalidIfIdentifierNotString)
    {
        EXPECT_FALSE(RunwayValid(MakeRunway(nlohmann::json{{"identifier", 123}}), airfields));
    }

    TEST_F(RunwayCollectionFactoryTest, RunwayIsInvalidIfThresholdLatitudeMissing)
    {
        EXPECT_FALSE(RunwayValid(MakeRunway(nlohmann::json::object(), "threshold_latitude"), airfields));
    }

    TEST_F(RunwayCollectionFactoryTest, RunwayIsInvalidIfThresholdLatitudeNotANumber)
    {
        EXPECT_FALSE(RunwayValid(MakeRunway(nlohmann::json{{"threshold_latitude", "abc"}}), airfields));
    }

    TEST_F(RunwayCollectionFactoryTest, RunwayIsInvalidIfThresholdLongitudeMissing)
    {
        EXPECT_FALSE(RunwayValid(MakeRunway(nlohmann::json::object(), "threshold_longitude"), airfields));
    }

    TEST_F(RunwayCollectionFactoryTest, RunwayIsInvalidIfThresholdLongitudeNotANumber)
    {
        EXPECT_FALSE(RunwayValid(MakeRunway(nlohmann::json{{"threshold_longitude", "abc"}}), airfields));
    }

    TEST_F(RunwayCollectionFactoryTest, RunwayIsInvalidIfThresholdElevationMissing)
    {
        EXPECT_FALSE(RunwayValid(MakeRunway(nlohmann::json::object(), "threshold_elevation"), airfields));
    }

    TEST_F(RunwayCollectionFactoryTest, RunwayIsInvalidIfThresholdElevationNotAnInteger)
    {
        EXPECT_FALSE(RunwayValid(MakeRunway(nlohmann::json{{"threshold_elevation", 1.5}}), airfields));
    }

    TEST_F(RunwayCollectionFactoryTest, RunwayIsInvalidIfGlideslopeAngleMissing)
    {
        EXPECT_FALSE(RunwayValid(MakeRunway(nlohmann::json::object(), "glideslope_angle"), airfields));
    }

    TEST_F(RunwayCollectionFactoryTest, RunwayIsInvalidIfThresholdElevationNotANumber)
    {
        EXPECT_FALSE(RunwayValid(MakeRunway(nlohmann::json{{"glideslope_angle", "abc"}}), airfields));
    }

    TEST_F(RunwayCollectionFactoryTest, RunwayIsInvalidIfNotAnObject)
    {
        EXPECT_FALSE(RunwayValid(nlohmann::json::array(), airfields));
    }

    TEST_F(RunwayCollectionFactoryTest, ItReturnsEmptyCollectionIfDependencyNotArray)
    {
        EXPECT_EQ(0, BuildRunwayCollection(nlohmann::json::object(), airfields)->Count());
    }

    TEST_F(RunwayCollectionFactoryTest, ItReturnsACollection)
    {
        auto dependency = nlohmann::json::array();
        dependency.push_back(nlohmann::json{
            {"id", 1},
            {"airfield_id", 2},
            {"identifier", "27L"},
            {"heading", 123},
            {"threshold_latitude", 1.2},
            {"threshold_longitude", 3.4},
            {"threshold_elevation", 201},
            {"glideslope_angle", 3.0}});
        dependency.push_back(nlohmann::json{
            {"id", 2},
            {"airfield_id", 3},
            {"identifier", "04"},
            {"heading", 234},
            {"threshold_latitude", 3.4},
            {"threshold_longitude", 4.5},
            {"threshold_elevation", 202},
            {"glideslope_angle", 3.1}});

        auto collection = BuildRunwayCollection(dependency, airfields);
        EXPECT_EQ(2, collection->Count());

        auto runway1 = collection->GetById(1);
        EXPECT_EQ(1, runway1->Id());
        EXPECT_EQ(2, runway1->AirfieldId());
        EXPECT_EQ("EGKK", runway1->AirfieldIdentifier());
        EXPECT_EQ("27L", runway1->Identifier());
        EXPECT_EQ(123, runway1->Heading());
        EXPECT_FLOAT_EQ(1.2, runway1->Threshold().m_Latitude);
        EXPECT_FLOAT_EQ(3.4, runway1->Threshold().m_Longitude);
        EXPECT_EQ(201, runway1->ThresholdElevation());
        EXPECT_FLOAT_EQ(3.0, runway1->GlideslopeAngle());

        auto runway2 = collection->GetById(2);
        EXPECT_EQ(2, runway2->Id());
        EXPECT_EQ(3, runway2->AirfieldId());
        EXPECT_EQ("EGLL", runway2->AirfieldIdentifier());
        EXPECT_EQ("04", runway2->Identifier());
        EXPECT_EQ(234, runway2->Heading());
        EXPECT_FLOAT_EQ(3.4, runway2->Threshold().m_Latitude);
        EXPECT_FLOAT_EQ(4.5, runway2->Threshold().m_Longitude);
        EXPECT_EQ(202, runway2->ThresholdElevation());
        EXPECT_DOUBLE_EQ(3.1, runway2->GlideslopeAngle());
    }

    TEST_F(RunwayCollectionFactoryTest, ItIgnoresInvalidRunways)
    {
        auto dependency = nlohmann::json::array();
        dependency.push_back(nlohmann::json{
            {"id", "abc"}, // Invalid
            {"airfield_id", 2},
            {"identifier", "27L"},
            {"heading", 123},
            {"threshold_latitude", 1.2},
            {"threshold_longitude", 3.4},
            {"threshold_elevation", 201},
            {"glideslope_angle", 3.0}});
        dependency.push_back(nlohmann::json{
            {"id", 2},
            {"airfield_id", 3},
            {"identifier", "04"},
            {"heading", 234},
            {"threshold_latitude", "abc"}, // Invalid
            {"threshold_longitude", 4.5},
            {"threshold_elevation", 201},
            {"glideslope_angle", 3.0}});

        EXPECT_EQ(0, BuildRunwayCollection(nlohmann::json::object(), airfields)->Count());
    }
} // namespace UKControllerPluginTest::Runway
