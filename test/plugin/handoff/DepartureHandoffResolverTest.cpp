#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"
#include "handoff/DepartureHandoffResolver.h"
#include "handoff/FlightplanSidHandoffMapper.h"
#include "handoff/HandoffCollection.h"
#include "handoff/HandoffOrder.h"
#include "handoff/ResolvedHandoff.h"
#include "sid/SidCollection.h"
#include "sid/StandardInstrumentDeparture.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Handoff::DepartureHandoffResolver;
using UKControllerPlugin::Handoff::FlightplanSidHandoffMapper;
using UKControllerPlugin::Handoff::HandoffCollection;
using UKControllerPlugin::Handoff::HandoffOrder;
using UKControllerPlugin::Handoff::ResolvedHandoff;
using UKControllerPlugin::Sid::SidCollection;
using UKControllerPlugin::Sid::StandardInstrumentDeparture;

namespace UKControllerPluginTest::Handoff {
    class DepartureHandoffResolverTest : public testing::Test
    {
        public:
        DepartureHandoffResolverTest()
            : position1(1, "EGKK_DEL", 121.950, {"EGKK"}, true, false),
              position2(2, "EGKK_GND", 121.800, {"EGKK"}, true, false), handoffs(std::make_shared<HandoffCollection>()),
              mapper(std::make_shared<FlightplanSidHandoffMapper>(handoffs, sids)), resolver(mapper, callsigns)
        {
            ON_CALL(mockFlightplan, GetOrigin).WillByDefault(testing::Return("EGKK"));
            ON_CALL(mockFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));

            sids.AddSid(std::make_shared<StandardInstrumentDeparture>("EGKK", "CLN3X", 5000, 200, 1));
            const auto hierarchy = std::make_shared<ControllerPositionHierarchy>();
            hierarchy->AddPosition(position1);
            hierarchy->AddPosition(position2);
            handoffs->Add(std::make_shared<HandoffOrder>(1, hierarchy));
        }

        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> mockFlightplan;
        ActiveCallsignCollection callsigns;
        SidCollection sids;
        ControllerPosition position1;
        ControllerPosition position2;
        std::shared_ptr<HandoffCollection> handoffs;
        std::shared_ptr<FlightplanSidHandoffMapper> mapper;
        DepartureHandoffResolver resolver;
    };

    TEST_F(DepartureHandoffResolverTest, ItReturnsUnicomIfNoHandoffFound)
    {
        ON_CALL(mockFlightplan, GetSidName).WillByDefault(testing::Return("LAM5M"));

        EXPECT_EQ(122.800, resolver.Resolve(mockFlightplan)->frequency);
    }

    TEST_F(DepartureHandoffResolverTest, ItReturnsUnicomIfNoPositionsActive)
    {
        ON_CALL(mockFlightplan, GetSidName).WillByDefault(testing::Return("CLN3X"));
        EXPECT_EQ(122.800, resolver.Resolve(mockFlightplan)->frequency);
    }

    TEST_F(DepartureHandoffResolverTest, ItReturnsUnicomIfFirstPositionOnlineIsUser)
    {
        this->callsigns.AddCallsign(ActiveCallsign("EGKK_DEL", "Testy McTest", position1, true));
        this->callsigns.AddCallsign(ActiveCallsign("EGKK_GND", "Testy McTest", position2, false));

        ON_CALL(mockFlightplan, GetSidName).WillByDefault(testing::Return("CLN3X"));
        EXPECT_EQ(122.800, resolver.Resolve(mockFlightplan)->frequency);
    }

    TEST_F(DepartureHandoffResolverTest, ItReturnsResolvedHandoff)
    {
        this->callsigns.AddCallsign(ActiveCallsign("EGKK_DEL", "Testy McTest", position1, false));
        this->callsigns.AddCallsign(ActiveCallsign("EGKK_GND", "Testy McTest", position2, true));

        ON_CALL(mockFlightplan, GetSidName).WillByDefault(testing::Return("CLN3X"));
        const auto resolved = resolver.Resolve(mockFlightplan);
        EXPECT_NE(nullptr, resolved);
        EXPECT_EQ("BAW123", resolved->callsign);
        EXPECT_EQ(121.800, resolved->frequency);
        EXPECT_EQ(handoffs->Get(1)->order, resolved->hierarchy);
    }
} // namespace UKControllerPluginTest::Handoff
