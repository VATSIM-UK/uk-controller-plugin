#include "pch/pch.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPosition.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;

namespace UKControllerPluginTest {
    namespace Controller {

        class ControllerPositionCollectionTest : public testing::Test
        {
            public:
                void SetUp() override
                {
                    controllerFirst.reset(
                        new ControllerPosition(
                            1,
                            "EGFF_APP",
                            125.850,
                            std::vector<std::string>{"EGGD", "EGFF"},
                            true,
                            false
                        )
                    );
                    controllerSecond.reset(
                        new ControllerPosition(
                            2,
                            "EGFF_APP",
                            999.999,
                            std::vector<std::string>{"EGGD", "EGFF"},
                            true,
                            false
                        )
                    );
                    controllerThird.reset(
                        new ControllerPosition(
                            3,
                            "ESSEX_APP",
                            120.620,
                            std::vector<std::string>{"EGSS", "EGGW"},
                            true,
                            false
                        )
                    );
                    controllerFourth.reset(
                        new ControllerPosition(
                            4,
                            "THAMES_APP",
                            133.700,
                            std::vector<std::string>{"EGLC"},
                            true,
                            false
                        )
                    );
                    controllerFifth.reset(
                        new ControllerPosition(
                            5,
                            "SOLENT_APP",
                            120.220,
                            std::vector<std::string>{"EGHI"},
                            true,
                            false
                        )
                    );
                }

                std::shared_ptr<ControllerPosition> controllerFirst;
                std::shared_ptr<ControllerPosition> controllerSecond;
                std::shared_ptr<ControllerPosition> controllerThird;
                std::shared_ptr<ControllerPosition> controllerFourth;
                std::shared_ptr<ControllerPosition> controllerFifth;
        };

        TEST_F(ControllerPositionCollectionTest, AddsCallsignOnlyAddsOnce)
        {
            ControllerPositionCollection collection;
            EXPECT_TRUE(collection.AddPosition(controllerFirst));
            EXPECT_FALSE(collection.AddPosition(controllerSecond));
        }

        TEST_F(ControllerPositionCollectionTest, GetSizeReturnsNumberOfPositions)
        {
            ControllerPositionCollection collection;
            EXPECT_EQ(0, collection.GetSize());
            collection.AddPosition(controllerFirst);
            collection.AddPosition(controllerThird);
            EXPECT_EQ(2, collection.GetSize());
        }

        TEST_F(ControllerPositionCollectionTest, FetchPositionByCallsignReturnsPositionIfFound)
        {
            ControllerPositionCollection collection;
            collection.AddPosition(controllerFirst);
            EXPECT_EQ(*controllerFirst, collection.FetchPositionByCallsign("EGFF_APP"));
        }

        TEST_F(ControllerPositionCollectionTest, FetchPositionByCallsignThrowsExceptionIfNotFound)
        {
            ControllerPositionCollection collection;
            EXPECT_THROW(collection.FetchPositionByCallsign("EGFF_APP"), std::out_of_range);
        }

        TEST_F(ControllerPositionCollectionTest, FetchPositionByIdReturnsPositionIfFound)
        {
            ControllerPositionCollection collection;
            collection.AddPosition(controllerFirst);
            EXPECT_EQ(controllerFirst, collection.FetchPositionById(1));
        }

        TEST_F(ControllerPositionCollectionTest, FetchPositionByIdReturnsNullptrIfNotFound)
        {
            ControllerPositionCollection collection;
            EXPECT_EQ(nullptr, collection.FetchPositionById(-55));
        }

        TEST_F(ControllerPositionCollectionTest,
               FetchPositionByFacilityTypeAndFrequencyThrowsExceptionIfPositionNotFound)
        {
            ControllerPositionCollection collection;
            EXPECT_THROW(collection.FetchPositionByFacilityTypeAndFrequency("EGBB", "APP", 121.200), std::out_of_range);
        }

        TEST_F(ControllerPositionCollectionTest,
               FetchPositionByFacilityTypeAndFrequencyThrowsExceptionIfNoFrequencyMatch)
        {
            ControllerPositionCollection collection;
            collection.AddPosition(controllerFirst);
            EXPECT_THROW(collection.FetchPositionByFacilityTypeAndFrequency("EGFF", "APP", 121.200), std::out_of_range);
        }

        TEST_F(ControllerPositionCollectionTest, FetchPositionByFacilityTypeAndFrequencyThrowsExceptionIfNoTypeMatch)
        {
            ControllerPositionCollection collection;
            collection.AddPosition(controllerFirst);
            EXPECT_THROW(collection.FetchPositionByFacilityTypeAndFrequency("EGFF", "TWR", 125.850), std::out_of_range);
        }

        TEST_F(ControllerPositionCollectionTest,
               FetchPositionByFacilityTypeAndFrequencyThrowsExceptionIfNoFacilityMatch)
        {
            ControllerPositionCollection collection;
            collection.AddPosition(controllerFirst);
            EXPECT_THROW(collection.FetchPositionByFacilityTypeAndFrequency("EGHI", "APP", 125.850), std::out_of_range);
        }

        TEST_F(ControllerPositionCollectionTest, FetchPositionByFacilityTypeAndFrequencyReturnsOnMatch)
        {
            ControllerPositionCollection collection;
            collection.AddPosition(controllerFirst);
            EXPECT_EQ(*controllerFirst, collection.FetchPositionByFacilityTypeAndFrequency("EGFF", "APP", 125.850));
        }

        TEST_F(ControllerPositionCollectionTest, FetchPositionByFacilityTypeAndFrequencyReturnsOnMatchFrequencyDelta)
        {
            ControllerPositionCollection collection;
            collection.AddPosition(controllerFirst);
            EXPECT_EQ(*controllerFirst, collection.FetchPositionByFacilityTypeAndFrequency("EGFF", "APP", 125.8501));
        }

        TEST_F(ControllerPositionCollectionTest, FetchPositionByFacilityTypeAndFrequencyWillWorkForEssex)
        {
            ControllerPositionCollection collection;
            collection.AddPosition(controllerThird);
            EXPECT_EQ(*controllerThird, collection.FetchPositionByFacilityTypeAndFrequency("ESSEX", "APP", 120.620));
        }

        TEST_F(ControllerPositionCollectionTest, FetchPositionByFacilityTypeAndFrequencyWillWorkForThames)
        {
            ControllerPositionCollection collection;
            collection.AddPosition(controllerFourth);
            EXPECT_EQ(*controllerFourth, collection.FetchPositionByFacilityTypeAndFrequency("THAMES", "APP", 133.700));
        }

        TEST_F(ControllerPositionCollectionTest, FetchPositionByFacilityTypeAndFrequencyWillWorkForSolent)
        {
            ControllerPositionCollection collection;
            collection.AddPosition(controllerFifth);
            EXPECT_EQ(*controllerFifth, collection.FetchPositionByFacilityTypeAndFrequency("SOLENT", "APP", 120.220));
        }

        TEST_F(ControllerPositionCollectionTest, FetchPositionByFacilityTypeAndFrequencyWillWorkForAbbreviations)
        {
            ControllerPositionCollection collection;
            collection.AddPosition(controllerThird);
            EXPECT_EQ(*controllerThird, collection.FetchPositionByFacilityTypeAndFrequency("ESX", "APP", 120.620));
        }

        TEST_F(ControllerPositionCollectionTest, HasPositionReturnsFalseIfDoesntExist)
        {
            ControllerPositionCollection collection;
            collection.AddPosition(controllerFirst);
            EXPECT_FALSE(collection.HasPosition("EGHI_TWR"));
        }

        TEST_F(ControllerPositionCollectionTest, HasPositionReturnsTrueIfExists)
        {
            ControllerPositionCollection collection;
            collection.AddPosition(controllerFirst);
            EXPECT_TRUE(collection.HasPosition("EGFF_APP"));
        }
    }  // namespace Controller
}  // namespace UKControllerPluginTest
