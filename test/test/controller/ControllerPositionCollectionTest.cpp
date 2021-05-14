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

                std::unique_ptr<ControllerPosition> controllerFirst;
                std::unique_ptr<ControllerPosition> controllerSecond;
                std::unique_ptr<ControllerPosition> controllerThird;
                std::unique_ptr<ControllerPosition> controllerFourth;
                std::unique_ptr<ControllerPosition> controllerFifth;
        };

        TEST_F(ControllerPositionCollectionTest, AddsCallsignOnlyAddsOnce)
        {
            ControllerPositionCollection collection;
            EXPECT_TRUE(collection.AddPosition(std::move(std::move(controllerFirst))));
            EXPECT_FALSE(collection.AddPosition(std::move(controllerSecond)));
        }

        TEST_F(ControllerPositionCollectionTest, GetSizeReturnsNumberOfPositions)
        {
            ControllerPositionCollection collection;
            EXPECT_EQ(0, collection.GetSize());
            collection.AddPosition(std::move(controllerFirst));
            collection.AddPosition(std::move(controllerThird));
            EXPECT_EQ(2, collection.GetSize());
        }

        TEST_F(ControllerPositionCollectionTest, FetchPositionByCallsignReturnsPositionIfFound)
        {
            ControllerPosition* rawController = controllerFirst.get();
            ControllerPositionCollection collection;
            collection.AddPosition(std::move(controllerFirst));
            EXPECT_EQ(*rawController, collection.FetchPositionByCallsign("EGFF_APP"));
        }

        TEST_F(ControllerPositionCollectionTest, FetchPositionByCallsignThrowsExceptionIfNotFound)
        {
            ControllerPositionCollection collection;
            EXPECT_THROW(collection.FetchPositionByCallsign("EGFF_APP"), std::out_of_range);
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
            collection.AddPosition(std::move(controllerFirst));
            EXPECT_THROW(collection.FetchPositionByFacilityTypeAndFrequency("EGFF", "APP", 121.200), std::out_of_range);
        }

        TEST_F(ControllerPositionCollectionTest, FetchPositionByFacilityTypeAndFrequencyThrowsExceptionIfNoTypeMatch)
        {
            ControllerPositionCollection collection;
            collection.AddPosition(std::move(controllerFirst));
            EXPECT_THROW(collection.FetchPositionByFacilityTypeAndFrequency("EGFF", "TWR", 125.850), std::out_of_range);
        }

        TEST_F(ControllerPositionCollectionTest,
               FetchPositionByFacilityTypeAndFrequencyThrowsExceptionIfNoFacilityMatch)
        {
            ControllerPositionCollection collection;
            collection.AddPosition(std::move(controllerFirst));
            EXPECT_THROW(collection.FetchPositionByFacilityTypeAndFrequency("EGHI", "APP", 125.850), std::out_of_range);
        }

        TEST_F(ControllerPositionCollectionTest, FetchPositionByFacilityTypeAndFrequencyReturnsOnMatch)
        {
            ControllerPositionCollection collection;
            ControllerPosition* controllerRaw = controllerFirst.get();
            collection.AddPosition(std::move(controllerFirst));
            EXPECT_EQ(*controllerRaw, collection.FetchPositionByFacilityTypeAndFrequency("EGFF", "APP", 125.850));
        }

        TEST_F(ControllerPositionCollectionTest, FetchPositionByFacilityTypeAndFrequencyReturnsOnMatchFrequencyDelta)
        {
            ControllerPositionCollection collection;
            ControllerPosition* controllerRaw = controllerFirst.get();
            collection.AddPosition(std::move(controllerFirst));
            EXPECT_EQ(*controllerRaw, collection.FetchPositionByFacilityTypeAndFrequency("EGFF", "APP", 125.8501));
        }

        TEST_F(ControllerPositionCollectionTest, FetchPositionByFacilityTypeAndFrequencyWillWorkForEssex)
        {
            ControllerPositionCollection collection;
            ControllerPosition* controllerRaw = controllerThird.get();
            collection.AddPosition(std::move(controllerThird));
            EXPECT_EQ(*controllerRaw, collection.FetchPositionByFacilityTypeAndFrequency("ESSEX", "APP", 120.620));
        }

        TEST_F(ControllerPositionCollectionTest, FetchPositionByFacilityTypeAndFrequencyWillWorkForThames)
        {
            ControllerPositionCollection collection;
            ControllerPosition* controllerRaw = controllerFourth.get();
            collection.AddPosition(std::move(controllerFourth));
            EXPECT_EQ(*controllerRaw, collection.FetchPositionByFacilityTypeAndFrequency("THAMES", "APP", 133.700));
        }

        TEST_F(ControllerPositionCollectionTest, FetchPositionByFacilityTypeAndFrequencyWillWorkForSolent)
        {
            ControllerPositionCollection collection;
            ControllerPosition* controllerRaw = controllerFifth.get();
            collection.AddPosition(std::move(controllerFifth));
            EXPECT_EQ(*controllerRaw, collection.FetchPositionByFacilityTypeAndFrequency("SOLENT", "APP", 120.220));
        }

        TEST_F(ControllerPositionCollectionTest, FetchPositionByFacilityTypeAndFrequencyWillWorkForAbbreviations)
        {
            ControllerPositionCollection collection;
            ControllerPosition* controllerRaw = controllerThird.get();
            collection.AddPosition(std::move(controllerThird));
            EXPECT_EQ(*controllerRaw, collection.FetchPositionByFacilityTypeAndFrequency("ESX", "APP", 120.620));
        }

        TEST_F(ControllerPositionCollectionTest, HasPositionReturnsFalseIfDoesntExist)
        {
            ControllerPositionCollection collection;
            collection.AddPosition(std::move(controllerFirst));
            EXPECT_FALSE(collection.HasPosition("EGHI_TWR"));
        }

        TEST_F(ControllerPositionCollectionTest, HasPositionReturnsTrueIfExists)
        {
            ControllerPositionCollection collection;
            collection.AddPosition(std::move(controllerFirst));
            EXPECT_TRUE(collection.HasPosition("EGFF_APP"));
        }
    }  // namespace Controller
}  // namespace UKControllerPluginTest
