#include "pch/pch.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPosition.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;

namespace UKControllerPluginTest {
    namespace Controller {

        TEST(ControllerPositionCollection, AddsCallsignOnlyAddsOnce)
        {
            std::unique_ptr<ControllerPosition> controllerFirst(
                new ControllerPosition("EGFF_APP", 125.850, "APP", std::vector<std::string> {"EGGD, EGFF"})
            );
            std::unique_ptr<ControllerPosition> controllerSecond(
                new ControllerPosition("EGFF_APP", 999.999, "APP", std::vector<std::string> {"EGGD, EGFF"})
            );

            ControllerPositionCollection collection;
            EXPECT_TRUE(collection.AddPosition(std::move(std::move(controllerFirst))));
            EXPECT_FALSE(collection.AddPosition(std::move(controllerSecond)));
        }

        TEST(ControllerPositionCollection, GetSizeReturnsNumberOfPositions)
        {
            std::unique_ptr<ControllerPosition> controllerFirst(
                new ControllerPosition("EGFF_APP", 125.850, "APP", std::vector<std::string> {"EGGD, EGFF"})
            );
            std::unique_ptr<ControllerPosition> controllerSecond(
                new ControllerPosition("EGGD_APP", 125.650, "APP", std::vector<std::string> {"EGGD"})
            );

            ControllerPositionCollection collection;
            EXPECT_EQ(0, collection.GetSize());
            collection.AddPosition(std::move(controllerFirst));
            collection.AddPosition(std::move(controllerSecond));
            EXPECT_EQ(2, collection.GetSize());
        }

        TEST(ControllerPositionCollection, FetchPositionByCallsignReturnsPositionIfFound)
        {
            std::unique_ptr<ControllerPosition> controller(
                new ControllerPosition("EGFF_APP", 125.850, "APP", std::vector<std::string> {"EGGD, EGFF"})
            );

            ControllerPosition * rawController = controller.get();
            ControllerPositionCollection collection;
            collection.AddPosition(std::move(controller));
            EXPECT_EQ(*rawController, collection.FetchPositionByCallsign("EGFF_APP"));
        }

        TEST(ControllerPositionCollection, FetchPositionByCallsignThrowsExceptionIfNotFound)
        {
            ControllerPositionCollection collection;
            EXPECT_THROW(collection.FetchPositionByCallsign("EGFF_APP"), std::out_of_range);
        }

        TEST(ControllerPositionCollection, FetchPositionByFacilityAndFrequencyThrowsExceptionIfNotPossible)
        {
            ControllerPositionCollection collection;
            EXPECT_THROW(collection.FetchPositionByFacilityAndFrequency("EIDW", 121.200), std::out_of_range);
        }

        TEST(ControllerPositionCollection, FetchPositionByFacilityAndFrequencyThrowsExceptionIfPositionNotFound)
        {
            ControllerPositionCollection collection;
            EXPECT_THROW(collection.FetchPositionByFacilityAndFrequency("EGBB", 121.200), std::out_of_range);
        }

        TEST(ControllerPositionCollection, FetchPositionByFacilityAndFrequencyThrowsExceptionIfNoFrequencyMatch)
        {
            ControllerPositionCollection collection;
            std::unique_ptr<ControllerPosition> controller(
                new ControllerPosition("EGFF_APP", 125.850, "APP", std::vector<std::string> {"EGGD, EGFF"})
            );
            collection.AddPosition(std::move(controller));
            EXPECT_THROW(collection.FetchPositionByFacilityAndFrequency("EGFF", 121.200), std::out_of_range);
        }

        TEST(ControllerPositionCollection, FetchPositionByFacilityAndFrequencyThrowsExceptionIfNoFacilityMatch)
        {
            ControllerPositionCollection collection;
            std::unique_ptr<ControllerPosition> controller(
                new ControllerPosition("EGFF_APP", 125.850, "APP", std::vector<std::string> {"EGGD, EGFF"})
            );
            collection.AddPosition(std::move(controller));
            EXPECT_THROW(collection.FetchPositionByFacilityAndFrequency("EGHI", 125.850), std::out_of_range);
        }

        TEST(ControllerPositionCollection, FetchPositionByFacilityAndFrequencyReturnsOnMatch)
        {
            ControllerPositionCollection collection;
            std::unique_ptr<ControllerPosition> controller(
                new ControllerPosition("EGFF_APP", 125.850, "APP", std::vector<std::string> {"EGGD, EGFF"})
            );

            ControllerPosition * controllerRaw = controller.get();
            collection.AddPosition(std::move(controller));
            EXPECT_EQ(*controllerRaw, collection.FetchPositionByFacilityAndFrequency("EGFF", 125.850));
        }

        TEST(ControllerPositionCollection, FetchPositionByFacilityAndFrequencyReturnsOnMatchFrequencyDelta)
        {
            ControllerPositionCollection collection;
            std::unique_ptr<ControllerPosition> controller(
                new ControllerPosition("EGFF_APP", 125.8505, "APP", std::vector<std::string> {"EGGD, EGFF"})
            );

            ControllerPosition * controllerRaw = controller.get();
            collection.AddPosition(std::move(controller));
            EXPECT_EQ(*controllerRaw, collection.FetchPositionByFacilityAndFrequency("EGFF", 125.8514));
        }

        TEST(ControllerPositionCollection, FetchPositionByFacilityAndFrequencyWillWorkForEssex)
        {
            ControllerPositionCollection collection;
            std::unique_ptr<ControllerPosition> controller(
                new ControllerPosition("ESSEX_APP", 120.620, "APP", std::vector<std::string> {"EGSS, EGGW, EGSC"})
            );

            ControllerPosition * controllerRaw = controller.get();
            collection.AddPosition(std::move(controller));
            EXPECT_EQ(*controllerRaw, collection.FetchPositionByFacilityAndFrequency("ESSEX", 120.620));
        }

        TEST(ControllerPositionCollection, FetchPositionByFacilityAndFrequencyWillWorkForThames)
        {
            ControllerPositionCollection collection;
            std::unique_ptr<ControllerPosition> controller(
                new ControllerPosition("THAMES", 132.700, "APP", std::vector<std::string> {"EGLC, EGKB"})
            );

            ControllerPosition * controllerRaw = controller.get();
            collection.AddPosition(std::move(controller));
            EXPECT_EQ(*controllerRaw, collection.FetchPositionByFacilityAndFrequency("THAMES", 132.700));
        }

        TEST(ControllerPositionCollection, FetchPositionByFacilityAndFrequencyWillWorkForSolent)
        {
            ControllerPositionCollection collection;
            std::unique_ptr<ControllerPosition> controller(
                new ControllerPosition("SOLENT", 120.220, "APP", std::vector<std::string> {"EGLC, EGKB"})
            );

            ControllerPosition * controllerRaw = controller.get();
            collection.AddPosition(std::move(controller));
            EXPECT_EQ(*controllerRaw, collection.FetchPositionByFacilityAndFrequency("SOLENT", 120.220));
        }

        TEST(ControllerPositionCollection, FetchPositionByFacilityAndFrequencyWillWorkForAbbreviations)
        {
            ControllerPositionCollection collection;
            std::unique_ptr<ControllerPosition> controller(
                new ControllerPosition("ESSEX_APP", 120.620, "APP", std::vector<std::string> {"EGSS, EGGW, EGSC"})
            );

            ControllerPosition * controllerRaw = controller.get();
            collection.AddPosition(std::move(controller));
            EXPECT_EQ(*controllerRaw, collection.FetchPositionByFacilityAndFrequency("ESX", 120.620));
        }
    }  // namespace Controller
}  // namespace UKControllerPluginTest
