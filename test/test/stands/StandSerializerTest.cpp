#include "pch/pch.h"
#include "stands/StandSerializer.h"
#include "stands/Stand.h"
#include "stands/StandSerializer.h"

using ::testing::Test;
using UKControllerPlugin::Stands::from_json;
using UKControllerPlugin::Stands::DependencyValid;
using UKControllerPlugin::Stands::CompareStands;
using UKControllerPlugin::Stands::Stand;

namespace UKControllerPluginTest {
    namespace Stands {

        class StandSerializerTest : public Test
        {

        };

        TEST_F(StandSerializerTest, DependencyValidReturnsTrueIfValid)
        {
            nlohmann::json gatwick = nlohmann::json::array();
            gatwick.push_back(
                {
                    {"id", 1},
                    {"identifier", "31R"},
                }
            );
            gatwick.push_back(
                {
                    {"id", 2},
                    {"identifier", "35"},
                }
            );
            nlohmann::json heathrow = nlohmann::json::array();
            heathrow.push_back(
                {
                    {"id", 3},
                    {"identifier", "76R"},
                }
            );
            nlohmann::json dependency = {
                {"EGKK", gatwick},
                {"EGLL", heathrow}
            };

            EXPECT_TRUE(DependencyValid(dependency));
        }

        TEST_F(StandSerializerTest, DependencyValidReturnsFalseIfIdInvalid)
        {
            nlohmann::json gatwick = nlohmann::json::array();
            gatwick.push_back(
                {
                    {"id", 1},
                    {"identifier", "31R"},
                }
            );
            gatwick.push_back(
                {
                    {"id", "abc"},
                    {"identifier", "35"},
                }
            );
            nlohmann::json heathrow = nlohmann::json::array();
            heathrow.push_back(
                {
                    {"id", 3},
                    {"identifier", "76R"},
                }
            );
            nlohmann::json dependency = {
                {"EGKK", gatwick},
                {"EGLL", heathrow}
            };

            EXPECT_FALSE(DependencyValid(dependency));
        }

        TEST_F(StandSerializerTest, DependencyValidReturnsFalseIfIdMissing)
        {
            nlohmann::json gatwick = nlohmann::json::array();
            gatwick.push_back(
                {
                    {"id", 1},
                    {"identifier", "31R"},
                }
            );
            gatwick.push_back(
                {
                    {"id", 2},
                    {"identifier", "35"},
                }
            );
            nlohmann::json heathrow = nlohmann::json::array();
            heathrow.push_back(
                {
                    {"identifier", "76R"},
                }
            );
            nlohmann::json dependency = {
                {"EGKK", gatwick},
                {"EGLL", heathrow}
            };

            EXPECT_FALSE(DependencyValid(dependency));
        }

        TEST_F(StandSerializerTest, DependencyValidReturnsFalseIfIdentifierInvalid)
        {
            nlohmann::json gatwick = nlohmann::json::array();
            gatwick.push_back(
                {
                    {"id", 1},
                    {"identifier", 123},
                }
            );
            gatwick.push_back(
                {
                    {"id", 2},
                    {"identifier", "35"},
                }
            );
            nlohmann::json heathrow = nlohmann::json::array();
            heathrow.push_back(
                {
                    {"id", 3},
                    {"identifier", "76R"},
                }
            );
            nlohmann::json dependency = {
                {"EGKK", gatwick},
                {"EGLL", heathrow}
            };

            EXPECT_FALSE(DependencyValid(dependency));
        }

        TEST_F(StandSerializerTest, DependencyValidReturnsFalseIfIdentifyingMissing)
        {
            nlohmann::json gatwick = nlohmann::json::array();
            gatwick.push_back(
                {
                    {"id", 1},
                    {"identifier", "31R"},
                }
            );
            gatwick.push_back(
                {
                    {"id", 2},
                }
            );
            nlohmann::json heathrow = nlohmann::json::array();
            heathrow.push_back(
                {
                    {"id", 3},
                    {"identifier", "76R"},
                }
            );
            nlohmann::json dependency = {
                {"EGKK", gatwick},
                {"EGLL", heathrow}
            };

            EXPECT_FALSE(DependencyValid(dependency));
        }

        TEST_F(StandSerializerTest, DependencyValidReturnsFalseIfAirfieldDataNotArray)
        {
            nlohmann::json gatwick = nlohmann::json::array();
            gatwick.push_back(
                {
                    {"id", 1},
                    {"identifier", "31R"},
                }
            );
            gatwick.push_back(
                {
                    {"id", 2},
                    {"identifier", "35"},
                }
            );
            nlohmann::json heathrow = nlohmann::json::object();
            nlohmann::json dependency = {
                {"EGKK", gatwick},
                {"EGLL", heathrow}
            };

            EXPECT_FALSE(DependencyValid(dependency));
        }

        TEST_F(StandSerializerTest, DependencyValidReturnsFalseIfDependencyNotObject)
        {
            nlohmann::json dependency = nlohmann::json::array();
            EXPECT_FALSE(DependencyValid(dependency));
        }

        TEST_F(StandSerializerTest, FromJsonHandlesHandlesInvalidDependendency)
        {
            nlohmann::json gatwick = nlohmann::json::array();
            gatwick.push_back(
                {
                    {"id", 1},
                    {"identifier", "31R"},
                }
            );
            gatwick.push_back(
                {
                    {"id", 2},
                    {"identifier", "35"},
                }
            );
            nlohmann::json heathrow = nlohmann::json::array();
            heathrow.push_back(
                {
                    {"id", 3}
                }
            );
            nlohmann::json dependency = {
                {"EGKK", gatwick},
                {"EGLL", heathrow}
            };

            std::set<Stand, CompareStands> stands;
            from_json(dependency, stands);
            EXPECT_EQ(0, stands.size());
        }

        TEST_F(StandSerializerTest, FromJsonCreatesStandsFromDependency)
        {
            nlohmann::json gatwick = nlohmann::json::array();
            gatwick.push_back(
                {
                    {"id", 1},
                    {"identifier", "31R"},
                }
            );
            gatwick.push_back(
                {
                    {"id", 2},
                    {"identifier", "35"},
                }
            );
            nlohmann::json heathrow = nlohmann::json::array();
            heathrow.push_back(
                {
                    {"id", 3},
                    {"identifier", "76R"},
                }
            );
            nlohmann::json dependency = {
                {"EGKK", gatwick},
                {"EGLL", heathrow}
            };

            std::set<Stand, CompareStands> stands;
            from_json(dependency, stands);

            EXPECT_EQ(3, stands.size());
            EXPECT_EQ(1, stands.find(1)->id);
            EXPECT_EQ("EGKK", stands.find(1)->airfieldCode);
            EXPECT_EQ("31R", stands.find(1)->identifier);
            EXPECT_EQ(2, stands.find(2)->id);
            EXPECT_EQ("EGKK", stands.find(2)->airfieldCode);
            EXPECT_EQ("35", stands.find(2)->identifier);
            EXPECT_EQ(3, stands.find(3)->id);
            EXPECT_EQ("EGLL", stands.find(3)->airfieldCode);
            EXPECT_EQ("76R", stands.find(3)->identifier);
        }
    }  // namespace Stands
}  // namespace UKControllerPluginTest
