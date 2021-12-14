#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"
#include "handoff/DepartureHandoffResolver.h"
#include "handoff/FlightplanAirfieldHandoffMapper.h"
#include "handoff/FlightplanSidHandoffMapper.h"
#include "handoff/HandoffCollection.h"
#include "handoff/HandoffOrder.h"
#include "handoff/ResolvedHandoff.h"
#include "prenote/PairedAirfieldPrenote.h"
#include "sid/SidCollection.h"
#include "sid/StandardInstrumentDeparture.h"

using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Handoff::DepartureHandoffResolver;
using UKControllerPlugin::Handoff::FlightplanAirfieldHandoffMapper;
using UKControllerPlugin::Handoff::FlightplanSidHandoffMapper;
using UKControllerPlugin::Handoff::HandoffCollection;
using UKControllerPlugin::Handoff::HandoffOrder;
using UKControllerPlugin::Handoff::ResolvedHandoff;
using UKControllerPlugin::Prenote::PairedAirfieldPrenote;
using UKControllerPlugin::Sid::SidCollection;
using UKControllerPlugin::Sid::StandardInstrumentDeparture;

namespace UKControllerPluginTest::Handoff {
    class DepartureHandoffResolverTest : public testing::Test
    {
        public:
        DepartureHandoffResolverTest()
            : position1(std::make_shared<ControllerPosition>(
                  1, "EGKK_DEL", 121.950, std::vector<std::string>{"EGKK"}, true, false)),
              position2(std::make_shared<ControllerPosition>(
                  2, "EGKK_GND", 121.800, std::vector<std::string>{"EGKK"}, true, false)),
              sidMapper(handoffs, sids), airfieldMapper(handoffs, airfields),
              resolver(sidMapper, airfieldMapper, callsigns)
        {
            ON_CALL(mockFlightplan, GetOrigin).WillByDefault(testing::Return("EGKK"));
            ON_CALL(mockFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));

            airfields.AddAirfield(std::make_shared<AirfieldModel>(
                2, "EGKK", nullptr, std::vector<std::shared_ptr<PairedAirfieldPrenote>>{}, 2));
            sids.AddSid(std::make_shared<StandardInstrumentDeparture>("EGKK", "CLN3X", 5000, 200, 1));
            hierarchy = std::make_shared<ControllerPositionHierarchy>();
            hierarchy->AddPosition(position1);
            hierarchy->AddPosition(position2);
            handoffs.Add(std::make_shared<HandoffOrder>(1, hierarchy));
        }

        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> mockFlightplan;
        ActiveCallsignCollection callsigns;
        AirfieldCollection airfields;
        SidCollection sids;
        std::shared_ptr<ControllerPositionHierarchy> hierarchy;
        std::shared_ptr<ControllerPosition> position1;
        std::shared_ptr<ControllerPosition> position2;
        HandoffCollection handoffs;
        FlightplanSidHandoffMapper sidMapper;
        FlightplanAirfieldHandoffMapper airfieldMapper;
        DepartureHandoffResolver resolver;
    };

    TEST_F(DepartureHandoffResolverTest, ItReturnsUnicomIfNoHandoffFound)
    {
        ON_CALL(mockFlightplan, GetSidName).WillByDefault(testing::Return("LAM5M"));

        const auto resolved = resolver.Resolve(mockFlightplan);
        EXPECT_EQ(122.800, resolved->frequency);
        EXPECT_EQ(0, resolved->hierarchy->CountPositions());
    }

    TEST_F(DepartureHandoffResolverTest, ItReturnsUnicomIfNoPositionsActive)
    {
        ON_CALL(mockFlightplan, GetSidName).WillByDefault(testing::Return("CLN3X"));

        const auto resolved = resolver.Resolve(mockFlightplan);
        EXPECT_EQ(122.800, resolved->frequency);
        EXPECT_EQ(this->hierarchy, resolved->hierarchy);
    }

    TEST_F(DepartureHandoffResolverTest, ItReturnsUnicomIfFirstPositionOnlineIsUser)
    {
        this->callsigns.AddCallsign(ActiveCallsign("EGKK_DEL", "Testy McTest", *position1, true));
        this->callsigns.AddCallsign(ActiveCallsign("EGKK_GND", "Testy McTest", *position2, false));

        ON_CALL(mockFlightplan, GetSidName).WillByDefault(testing::Return("CLN3X"));

        const auto resolved = resolver.Resolve(mockFlightplan);
        EXPECT_EQ(122.800, resolved->frequency);
        EXPECT_EQ(this->hierarchy, resolved->hierarchy);
    }

    TEST_F(DepartureHandoffResolverTest, ItReturnsResolvedHandoff)
    {
        this->callsigns.AddCallsign(ActiveCallsign("EGKK_DEL", "Testy McTest", *position1, false));
        this->callsigns.AddCallsign(ActiveCallsign("EGKK_GND", "Testy McTest", *position2, true));

        ON_CALL(mockFlightplan, GetSidName).WillByDefault(testing::Return("CLN3X"));
        const auto resolved = resolver.Resolve(mockFlightplan);
        EXPECT_NE(nullptr, resolved);
        EXPECT_EQ("BAW123", resolved->callsign);
        EXPECT_EQ(121.950, resolved->frequency);
        EXPECT_EQ(this->hierarchy, resolved->hierarchy);
    }

    TEST_F(DepartureHandoffResolverTest, ItReturnsResolvedAirfieldHandoffIfNoMatchingSid)
    {
        this->callsigns.AddCallsign(ActiveCallsign("EGKK_DEL", "Testy McTest", *position1, false));
        this->callsigns.AddCallsign(ActiveCallsign("EGKK_GND", "Testy McTest", *position2, true));

        auto hierarchy2 = std::make_shared<ControllerPositionHierarchy>();
        hierarchy2->AddPosition(position2);
        handoffs.Add(std::make_shared<HandoffOrder>(2, hierarchy2));
        ON_CALL(mockFlightplan, GetSidName).WillByDefault(testing::Return(""));

        const auto resolved = resolver.Resolve(mockFlightplan);
        EXPECT_NE(nullptr, resolved);
        EXPECT_EQ("BAW123", resolved->callsign);
        EXPECT_EQ(121.800, resolved->frequency);
        EXPECT_EQ(hierarchy2, resolved->hierarchy);
    }
} // namespace UKControllerPluginTest::Handoff
