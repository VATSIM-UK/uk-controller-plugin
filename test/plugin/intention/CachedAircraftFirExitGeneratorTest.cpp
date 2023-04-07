#include "flightplan/FlightplanPoint.h"
#include "flightplan/ParsedFlightplan.h"
#include "intention/AircraftFirExit.h"
#include "intention/CachedAircraftFirExitGenerator.h"
#include "intention/FirExitPoint.h"
#include "intention/FirExitPointCollection.h"

using UKControllerPlugin::Flightplan::FlightplanPoint;
using UKControllerPlugin::Flightplan::ParsedFlightplan;
using UKControllerPlugin::IntentionCode::AircraftFirExit;
using UKControllerPlugin::IntentionCode::CachedAircraftFirExitGenerator;
using UKControllerPlugin::IntentionCode::FirExitPoint;
using UKControllerPlugin::IntentionCode::FirExitPointCollection;

namespace UKControllerPluginTest::IntentionCode {
    class CachedAircraftFirExitGeneratorTest : public testing::Test
    {
        public:
        CachedAircraftFirExitGeneratorTest()
            : coordinate(std::make_shared<testing::NiceMock<Euroscope::MockEuroscopeCoordinateInterface>>()),
              parsedFlightplan(std::make_shared<ParsedFlightplan>()),
              exitDetermination1(std::make_shared<testing::NiceMock<MockExitDetermination>>()),
              exitDetermination2(std::make_shared<testing::NiceMock<MockExitDetermination>>()),
              exitDetermination3(std::make_shared<testing::NiceMock<MockExitDetermination>>()),
              exitDetermination4(std::make_shared<testing::NiceMock<MockExitDetermination>>()),
              exitDetermination5(std::make_shared<testing::NiceMock<MockExitDetermination>>()),
              exitDetermination6(std::make_shared<testing::NiceMock<MockExitDetermination>>()),
              collection(std::make_shared<FirExitPointCollection>()),
              point1(std::make_shared<FirExitPoint>(1, "FOO", false, exitDetermination1)),
              point2(std::make_shared<FirExitPoint>(2, "WOO", false, exitDetermination2)),
              point3(std::make_shared<FirExitPoint>(3, "DOO", false, exitDetermination3)),
              point4(std::make_shared<FirExitPoint>(4, "MOO", true, exitDetermination4)),
              point5(std::make_shared<FirExitPoint>(5, "BOO", true, exitDetermination5)),
              point6(std::make_shared<FirExitPoint>(6, "LOO", true, exitDetermination6)), generator(collection)
        {
            ON_CALL(flightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));
            ON_CALL(flightplan, GetParsedFlightplan).WillByDefault(testing::Return(parsedFlightplan));

            collection->Add(point1);
            collection->Add(point2);
            collection->Add(point3);
            collection->Add(point4);
            collection->Add(point5);
            collection->Add(point6);
        }

        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroscopeCoordinateInterface>> coordinate;
        std::shared_ptr<ParsedFlightplan> parsedFlightplan;
        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> flightplan;
        testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface> radarTarget;
        std::shared_ptr<testing::NiceMock<MockExitDetermination>> exitDetermination1;
        std::shared_ptr<testing::NiceMock<MockExitDetermination>> exitDetermination2;
        std::shared_ptr<testing::NiceMock<MockExitDetermination>> exitDetermination3;
        std::shared_ptr<testing::NiceMock<MockExitDetermination>> exitDetermination4;
        std::shared_ptr<testing::NiceMock<MockExitDetermination>> exitDetermination5;
        std::shared_ptr<testing::NiceMock<MockExitDetermination>> exitDetermination6;
        std::shared_ptr<FirExitPointCollection> collection;
        std::shared_ptr<FirExitPoint> point1;
        std::shared_ptr<FirExitPoint> point2;
        std::shared_ptr<FirExitPoint> point3;
        std::shared_ptr<FirExitPoint> point4;
        std::shared_ptr<FirExitPoint> point5;
        std::shared_ptr<FirExitPoint> point6;
        CachedAircraftFirExitGenerator generator;
    };

    TEST_F(CachedAircraftFirExitGeneratorTest, ItReturnsCachedEntry)
    {
        const auto entry = std::make_shared<AircraftFirExit>();
        entry->callsign = "BAW123";
        generator.AddCacheEntry(entry);

        EXPECT_EQ(entry, generator.GetCacheEntryForCallsign("BAW123"));
        EXPECT_EQ(entry, generator.Generate(flightplan));
    }

    TEST_F(CachedAircraftFirExitGeneratorTest, ItReturnsEmptyEntryIfNoFlightplan)
    {
        const auto firExit = generator.Generate(flightplan);
        EXPECT_EQ("BAW123", firExit->callsign);
        EXPECT_EQ(nullptr, firExit->internalExitPoint);
        EXPECT_EQ(nullptr, firExit->ukExitPoint);
        EXPECT_EQ(firExit, generator.GetCacheEntryForCallsign("BAW123"));
    }

    TEST_F(CachedAircraftFirExitGeneratorTest, ItReturnsOnlyTheUkExitPointIfSpecified)
    {
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(0, "NOTTHIS", coordinate));
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(1, "FOO", coordinate));

        EXPECT_CALL(
            *exitDetermination1,
            AircraftIsExiting(testing::Ref(*parsedFlightplan->PointByIndex(1)), testing::Ref(flightplan)))
            .Times(1)
            .WillOnce(testing::Return(true));

        const auto firExit = generator.Generate(flightplan);
        EXPECT_EQ("BAW123", firExit->callsign);
        EXPECT_EQ(nullptr, firExit->internalExitPoint);
        EXPECT_EQ(1, firExit->ukExitPoint->Id());
        EXPECT_EQ(firExit, generator.GetCacheEntryForCallsign("BAW123"));
    }

    TEST_F(CachedAircraftFirExitGeneratorTest, ItReturnsTheFirstExitingUkExitPoint)
    {
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(0, "NOTTHIS", coordinate));
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(1, "WOO", coordinate));
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(2, "FOO", coordinate));
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(3, "DOO", coordinate));

        EXPECT_CALL(
            *exitDetermination1,
            AircraftIsExiting(testing::Ref(*parsedFlightplan->PointByIndex(2)), testing::Ref(flightplan)))
            .Times(1)
            .WillOnce(testing::Return(true));

        EXPECT_CALL(
            *exitDetermination2,
            AircraftIsExiting(testing::Ref(*parsedFlightplan->PointByIndex(1)), testing::Ref(flightplan)))
            .Times(1)
            .WillOnce(testing::Return(false));

        EXPECT_CALL(*exitDetermination3, AircraftIsExiting).Times(0);

        const auto firExit = generator.Generate(flightplan);
        EXPECT_EQ("BAW123", firExit->callsign);
        EXPECT_EQ(nullptr, firExit->internalExitPoint);
        EXPECT_EQ(1, firExit->ukExitPoint->Id());
        EXPECT_EQ(firExit, generator.GetCacheEntryForCallsign("BAW123"));
    }

    TEST_F(CachedAircraftFirExitGeneratorTest, ItReturnsInternalAndUkExitPoints)
    {
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(0, "NOTTHIS", coordinate));
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(1, "MOO", coordinate));
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(2, "FOO", coordinate));

        EXPECT_CALL(
            *exitDetermination1,
            AircraftIsExiting(testing::Ref(*parsedFlightplan->PointByIndex(2)), testing::Ref(flightplan)))
            .Times(1)
            .WillOnce(testing::Return(true));

        EXPECT_CALL(
            *exitDetermination4,
            AircraftIsExiting(testing::Ref(*parsedFlightplan->PointByIndex(1)), testing::Ref(flightplan)))
            .Times(1)
            .WillOnce(testing::Return(true));

        const auto firExit = generator.Generate(flightplan);
        EXPECT_EQ("BAW123", firExit->callsign);
        EXPECT_EQ(4, firExit->internalExitPoint->Id());
        EXPECT_EQ(1, firExit->ukExitPoint->Id());
        EXPECT_EQ(firExit, generator.GetCacheEntryForCallsign("BAW123"));
    }

    TEST_F(CachedAircraftFirExitGeneratorTest, ItReturnsTheFirstInternalExitPointFound)
    {
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(0, "NOTTHIS", coordinate));
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(1, "MOO", coordinate));
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(2, "BOO", coordinate));
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(3, "LOO", coordinate));

        EXPECT_CALL(
            *exitDetermination4,
            AircraftIsExiting(testing::Ref(*parsedFlightplan->PointByIndex(1)), testing::Ref(flightplan)))
            .Times(1)
            .WillOnce(testing::Return(false));

        EXPECT_CALL(
            *exitDetermination5,
            AircraftIsExiting(testing::Ref(*parsedFlightplan->PointByIndex(2)), testing::Ref(flightplan)))
            .Times(1)
            .WillOnce(testing::Return(true));

        EXPECT_CALL(*exitDetermination6, AircraftIsExiting).Times(0);

        const auto firExit = generator.Generate(flightplan);
        EXPECT_EQ("BAW123", firExit->callsign);
        EXPECT_EQ(5, firExit->internalExitPoint->Id());
        EXPECT_EQ(nullptr, firExit->ukExitPoint);
        EXPECT_EQ(firExit, generator.GetCacheEntryForCallsign("BAW123"));
    }

    TEST_F(CachedAircraftFirExitGeneratorTest, ItRemovesCachedEntry)
    {
        const auto entry = std::make_shared<AircraftFirExit>();
        entry->callsign = "BAW123";
        generator.AddCacheEntry(entry);
        generator.RemoveCacheEntryForCallsign("BAW123");
        EXPECT_EQ(nullptr, generator.GetCacheEntryForCallsign("BAW123"));
    }

    TEST_F(CachedAircraftFirExitGeneratorTest, FlightplanUpdateEventClearsCache)
    {
        const auto entry = std::make_shared<AircraftFirExit>();
        entry->callsign = "BAW123";
        generator.AddCacheEntry(entry);
        generator.FlightPlanEvent(flightplan, radarTarget);
        EXPECT_EQ(nullptr, generator.GetCacheEntryForCallsign("BAW123"));
    }

    TEST_F(CachedAircraftFirExitGeneratorTest, FlightplanDisconnectEventClearsCache)
    {
        const auto entry = std::make_shared<AircraftFirExit>();
        entry->callsign = "BAW123";
        generator.AddCacheEntry(entry);
        generator.FlightPlanDisconnectEvent(flightplan);
        EXPECT_EQ(nullptr, generator.GetCacheEntryForCallsign("BAW123"));
    }
} // namespace UKControllerPluginTest::IntentionCode
