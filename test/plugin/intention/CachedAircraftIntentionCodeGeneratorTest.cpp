#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"
#include "intention/AircraftIntentionCode.h"
#include "intention/AllOf.h"
#include "intention/AnyOf.h"
#include "intention/CachedAircraftIntentionCodeGenerator.h"
#include "intention/Condition.h"
#include "intention/IntentionCodeCollection.h"
#include "intention/IntentionCodeEventHandlerCollection.h"
#include "intention/IntentionCodeEventHandlerInterface.h"
#include "intention/IntentionCodeMetadata.h"
#include "intention/IntentionCodeModel.h"
#include "intention/SingleCode.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::IntentionCode::AircraftIntentionCode;
using UKControllerPlugin::IntentionCode::AllOf;
using UKControllerPlugin::IntentionCode::AnyOf;
using UKControllerPlugin::IntentionCode::CachedAircraftIntentionCodeGenerator;
using UKControllerPlugin::IntentionCode::Condition;
using UKControllerPlugin::IntentionCode::IntentionCodeCollection;
using UKControllerPlugin::IntentionCode::IntentionCodeEventHandlerCollection;
using UKControllerPlugin::IntentionCode::IntentionCodeEventHandlerInterface;
using UKControllerPlugin::IntentionCode::IntentionCodeMetadata;
using UKControllerPlugin::IntentionCode::IntentionCodeModel;
using UKControllerPlugin::IntentionCode::SingleCode;

namespace UKControllerPluginTest::IntentionCode {

    // Mock for the test
    class CachedAircraftIntentionCodeGeneratorTestEventHandlerMock : public IntentionCodeEventHandlerInterface
    {
        public:
        void IntentionCodeUpdated(const AircraftIntentionCode& intentionCode) override
        {
            passedCode = &intentionCode;
            updatedCalled = true;
        }

        const AircraftIntentionCode* passedCode = nullptr;
        bool updatedCalled = false;
    };

    class CachedAircraftIntentionCodeGeneratorTest : public testing::Test
    {
        public:
        CachedAircraftIntentionCodeGeneratorTest()
            : code1(std::make_shared<IntentionCodeModel>(
                  1,
                  std::make_unique<SingleCode>("A1"),
                  std::make_unique<AnyOf>(std::list<std::shared_ptr<Condition>>({})),
                  std::unique_ptr<IntentionCodeMetadata>(new IntentionCodeMetadata))),
              code2(std::make_shared<IntentionCodeModel>(
                  2,
                  std::make_unique<SingleCode>("A2"),
                  std::make_unique<AllOf>(std::list<std::shared_ptr<Condition>>({})),
                  std::unique_ptr<IntentionCodeMetadata>(new IntentionCodeMetadata))),
              code3(std::make_shared<IntentionCodeModel>(
                  3,
                  std::make_unique<SingleCode>("A3"),
                  std::make_unique<AllOf>(std::list<std::shared_ptr<Condition>>({})),
                  std::unique_ptr<IntentionCodeMetadata>(new IntentionCodeMetadata))),
              position(1, "LON_S_CTR", 129.420, std::vector<std::string>{}, true, false),
              nonUserPosition("LON_S_CTR", "Not User", position, false),
              userPosition("LON_S_CTR", "User", position, true), codes(std::make_shared<IntentionCodeCollection>()),
              mockEventHandler(std::make_shared<CachedAircraftIntentionCodeGeneratorTestEventHandlerMock>()),
              eventHandlers(std::make_shared<IntentionCodeEventHandlerCollection>()), generator(codes, eventHandlers)
        {
            ON_CALL(flightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));

            eventHandlers->AddHandler(mockEventHandler);
        }

        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> flightplan;
        testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface> radarTarget;
        std::shared_ptr<IntentionCodeModel> code1;
        std::shared_ptr<IntentionCodeModel> code2;
        std::shared_ptr<IntentionCodeModel> code3;
        ControllerPosition position;
        ActiveCallsign nonUserPosition;
        ActiveCallsign userPosition;
        std::shared_ptr<IntentionCodeCollection> codes;
        std::shared_ptr<CachedAircraftIntentionCodeGeneratorTestEventHandlerMock> mockEventHandler;
        std::shared_ptr<IntentionCodeEventHandlerCollection> eventHandlers;
        CachedAircraftIntentionCodeGenerator generator;
    };

    TEST_F(CachedAircraftIntentionCodeGeneratorTest, ItReturnsCachedEntry)
    {
        const auto entry = std::make_shared<AircraftIntentionCode>();
        entry->callsign = "BAW123";
        generator.AddCacheEntry(entry);

        EXPECT_EQ(entry, generator.GetCacheEntryForCallsign("BAW123"));
        EXPECT_EQ(entry, generator.Generate(flightplan, radarTarget));
        EXPECT_FALSE(mockEventHandler->updatedCalled);
    }

    TEST_F(CachedAircraftIntentionCodeGeneratorTest, ItReturnsEmptyCodeIfNoMatches)
    {
        const auto generated = generator.Generate(flightplan, radarTarget);
        EXPECT_NE(nullptr, generated);
        EXPECT_EQ("BAW123", generated->callsign);
        EXPECT_EQ("--", generated->intentionCode);
        EXPECT_EQ(nullptr, generated->matchedIntentionCode);
    }

    TEST_F(CachedAircraftIntentionCodeGeneratorTest, ItReturnsFirstMatchedCode)
    {
        codes->Add(code1);
        codes->Add(code2);
        codes->Add(code3);

        const auto generated = generator.Generate(flightplan, radarTarget);
        EXPECT_NE(nullptr, generated);
        EXPECT_EQ("BAW123", generated->callsign);
        EXPECT_EQ("A2", generated->intentionCode);
        EXPECT_EQ(code2, generated->matchedIntentionCode);
    }

    TEST_F(CachedAircraftIntentionCodeGeneratorTest, ItCachesMatchedCode)
    {
        codes->Add(code1);
        codes->Add(code2);
        codes->Add(code3);

        const auto generated = generator.Generate(flightplan, radarTarget);
        const auto generated1 = generator.Generate(flightplan, radarTarget);
        EXPECT_NE(nullptr, generated);
        EXPECT_NE(nullptr, generated1);
        EXPECT_EQ(generated, generated1);
        EXPECT_EQ(generated, generator.GetCacheEntryForCallsign("BAW123"));
    }

    TEST_F(CachedAircraftIntentionCodeGeneratorTest, ItTriggersUpdatedEventWhenCodeGenerated)
    {
        codes->Add(code1);
        codes->Add(code2);
        codes->Add(code3);

        const auto generated = generator.Generate(flightplan, radarTarget);
        EXPECT_TRUE(mockEventHandler->updatedCalled);
        EXPECT_EQ(generated.get(), mockEventHandler->passedCode);
    }

    TEST_F(CachedAircraftIntentionCodeGeneratorTest, FlightplanEventRemovesEntryFromCache)
    {
        const auto entry = std::make_shared<AircraftIntentionCode>();
        entry->callsign = "BAW123";
        generator.AddCacheEntry(entry);

        const auto entry2 = std::make_shared<AircraftIntentionCode>();
        entry2->callsign = "BAW456";
        generator.AddCacheEntry(entry2);

        generator.FlightPlanEvent(flightplan, radarTarget);
        EXPECT_EQ(nullptr, generator.GetCacheEntryForCallsign("BAW123"));
        EXPECT_EQ(entry2, generator.GetCacheEntryForCallsign("BAW456"));
    }

    TEST_F(CachedAircraftIntentionCodeGeneratorTest, FlightplanDisconnectedEventRemovesEntryFromCache)
    {
        const auto entry = std::make_shared<AircraftIntentionCode>();
        entry->callsign = "BAW123";
        generator.AddCacheEntry(entry);

        const auto entry2 = std::make_shared<AircraftIntentionCode>();
        entry2->callsign = "BAW456";
        generator.AddCacheEntry(entry2);

        generator.FlightPlanDisconnectEvent(flightplan);
        EXPECT_EQ(nullptr, generator.GetCacheEntryForCallsign("BAW123"));
        EXPECT_EQ(entry2, generator.GetCacheEntryForCallsign("BAW456"));
    }

    TEST_F(CachedAircraftIntentionCodeGeneratorTest, ActiveCallsignAddedEventClearsCacheIfUser)
    {
        const auto entry = std::make_shared<AircraftIntentionCode>();
        entry->callsign = "BAW123";
        generator.AddCacheEntry(entry);

        const auto entry2 = std::make_shared<AircraftIntentionCode>();
        entry2->callsign = "BAW456";
        generator.AddCacheEntry(entry2);

        generator.ActiveCallsignAdded(userPosition);

        EXPECT_EQ(nullptr, generator.GetCacheEntryForCallsign("BAW123"));
        EXPECT_EQ(nullptr, generator.GetCacheEntryForCallsign("BAW456"));
    }

    TEST_F(CachedAircraftIntentionCodeGeneratorTest, ActiveCallsignRemovedEventClearsCacheIfUser)
    {
        const auto entry = std::make_shared<AircraftIntentionCode>();
        entry->callsign = "BAW123";
        generator.AddCacheEntry(entry);

        const auto entry2 = std::make_shared<AircraftIntentionCode>();
        entry2->callsign = "BAW456";
        generator.AddCacheEntry(entry2);

        generator.ActiveCallsignRemoved(userPosition);

        EXPECT_EQ(nullptr, generator.GetCacheEntryForCallsign("BAW123"));
        EXPECT_EQ(nullptr, generator.GetCacheEntryForCallsign("BAW456"));
    }

    TEST_F(CachedAircraftIntentionCodeGeneratorTest, ActiveCallsignAddedEventDoesntClearCacheIfNotUser)
    {
        const auto entry = std::make_shared<AircraftIntentionCode>();
        entry->callsign = "BAW123";
        generator.AddCacheEntry(entry);

        const auto entry2 = std::make_shared<AircraftIntentionCode>();
        entry2->callsign = "BAW456";
        generator.AddCacheEntry(entry2);

        generator.ActiveCallsignAdded(nonUserPosition);

        EXPECT_EQ(entry, generator.GetCacheEntryForCallsign("BAW123"));
        EXPECT_EQ(entry2, generator.GetCacheEntryForCallsign("BAW456"));
    }

    TEST_F(CachedAircraftIntentionCodeGeneratorTest, ActiveCallsignRemovedEventDoesntClearCacheIfNotUser)
    {
        const auto entry = std::make_shared<AircraftIntentionCode>();
        entry->callsign = "BAW123";
        generator.AddCacheEntry(entry);

        const auto entry2 = std::make_shared<AircraftIntentionCode>();
        entry2->callsign = "BAW456";
        generator.AddCacheEntry(entry2);

        generator.ActiveCallsignRemoved(nonUserPosition);

        EXPECT_EQ(entry, generator.GetCacheEntryForCallsign("BAW123"));
        EXPECT_EQ(entry2, generator.GetCacheEntryForCallsign("BAW456"));
    }
} // namespace UKControllerPluginTest::IntentionCode
