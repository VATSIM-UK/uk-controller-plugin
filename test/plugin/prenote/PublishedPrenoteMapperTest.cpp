#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "controller/ControllerPositionHierarchy.h"
#include "flightrule/FlightRule.h"
#include "flightrule/FlightRuleCollection.h"
#include "prenote/PairedAirfieldPrenote.h"
#include "prenote/PublishedPrenote.h"
#include "prenote/PublishedPrenoteCollection.h"
#include "prenote/PublishedPrenoteMapper.h"
#include "sid/StandardInstrumentDeparture.h"

using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::FlightRules::FlightRule;
using UKControllerPlugin::FlightRules::FlightRuleCollection;
using UKControllerPlugin::Prenote::PairedAirfieldPrenote;
using UKControllerPlugin::Prenote::PublishedPrenote;
using UKControllerPlugin::Prenote::PublishedPrenoteCollection;
using UKControllerPlugin::Prenote::PublishedPrenoteMapper;
using UKControllerPlugin::Sid::StandardInstrumentDeparture;

namespace UKControllerPluginTest::Prenote {
    class PublishedPrenoteMapperTest : public testing::Test
    {
        public:
        PublishedPrenoteMapperTest() : mapper(prenotes, airfields, sidMapper, flightRules)
        {
            ON_CALL(mockFlightplan, GetOrigin).WillByDefault(testing::Return("EGGD"));
            ON_CALL(mockFlightplan, GetDestination).WillByDefault(testing::Return("EGKK"));
            ON_CALL(mockFlightplan, GetFlightRules).WillByDefault(testing::Return("I"));
            ON_CALL(mockFlightplan, GetSidName).WillByDefault(testing::Return("BADIM1X"));

            this->flightRules.Add(std::make_shared<FlightRule>(1, "I", "IFR"));
            this->flightRules.Add(std::make_shared<FlightRule>(2, "V", "VFR"));
            this->prenotes.Add(std::make_shared<PublishedPrenote>(1, nullptr));
            this->prenotes.Add(std::make_shared<PublishedPrenote>(2, nullptr));
            this->prenotes.Add(std::make_shared<PublishedPrenote>(3, nullptr));
        }

        testing::NiceMock<Sid::MockSidMapperInterface> sidMapper;
        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> mockFlightplan;
        AirfieldCollection airfields;
        FlightRuleCollection flightRules;
        PublishedPrenoteCollection prenotes;
        PublishedPrenoteMapper mapper;
    };

    TEST_F(PublishedPrenoteMapperTest, ItReturnsSidPrenotes)
    {
        EXPECT_CALL(sidMapper, MapFlightplanToSid(testing::Ref(mockFlightplan)))
            .Times(1)
            .WillOnce(testing::Return(
                std::make_shared<StandardInstrumentDeparture>(1, 2, "BADIM1X", 3000, 100, 1, std::set<int>({1, 2}))));

        EXPECT_EQ(
            std::set<std::shared_ptr<PublishedPrenote>>({this->prenotes.Get(1), this->prenotes.Get(2)}),
            this->mapper.MapForFlightplan(this->mockFlightplan));
    }

    TEST_F(PublishedPrenoteMapperTest, ItIgnoresSidPrenotesThatDontExist)
    {
        EXPECT_CALL(sidMapper, MapFlightplanToSid(testing::Ref(mockFlightplan)))
            .Times(1)
            .WillOnce(testing::Return(
                std::make_shared<StandardInstrumentDeparture>(1, 2, "BADIM1X", 3000, 100, 1, std::set<int>({1, 4}))));

        EXPECT_EQ(
            std::set<std::shared_ptr<PublishedPrenote>>({this->prenotes.Get(1)}),
            this->mapper.MapForFlightplan(this->mockFlightplan));
    }

    TEST_F(PublishedPrenoteMapperTest, ItIgnoresSidsThatDontExist)
    {
        EXPECT_CALL(sidMapper, MapFlightplanToSid(testing::Ref(mockFlightplan)))
            .Times(1)
            .WillOnce(testing::Return(nullptr));

        EXPECT_EQ(std::set<std::shared_ptr<PublishedPrenote>>({}), this->mapper.MapForFlightplan(this->mockFlightplan));
    }

    TEST_F(PublishedPrenoteMapperTest, ItReturnsAirfieldPairingPrenotes)
    {
        this->airfields.AddAirfield(std::make_shared<AirfieldModel>(
            1,
            "EGGD",
            nullptr,
            std::vector<std::shared_ptr<PairedAirfieldPrenote>>{
                std::make_shared<PairedAirfieldPrenote>(2, 1, 1),
                std::make_shared<PairedAirfieldPrenote>(2, 1, 2),
            }));

        this->airfields.AddAirfield(
            std::make_shared<AirfieldModel>(2, "EGKK", nullptr, std::vector<std::shared_ptr<PairedAirfieldPrenote>>{}));

        EXPECT_EQ(
            std::set<std::shared_ptr<PublishedPrenote>>({this->prenotes.Get(1), this->prenotes.Get(2)}),
            this->mapper.MapForFlightplan(this->mockFlightplan));
    }

    TEST_F(PublishedPrenoteMapperTest, ItIgnoresInvalidAirfieldPairingPrenotes)
    {
        this->airfields.AddAirfield(std::make_shared<AirfieldModel>(
            1,
            "EGGD",
            nullptr,
            std::vector<std::shared_ptr<PairedAirfieldPrenote>>{
                std::make_shared<PairedAirfieldPrenote>(2, 1, 1),
                std::make_shared<PairedAirfieldPrenote>(2, 1, 4),
            }));

        this->airfields.AddAirfield(
            std::make_shared<AirfieldModel>(2, "EGKK", nullptr, std::vector<std::shared_ptr<PairedAirfieldPrenote>>{}));

        EXPECT_EQ(
            std::set<std::shared_ptr<PublishedPrenote>>({this->prenotes.Get(1)}),
            this->mapper.MapForFlightplan(this->mockFlightplan));
    }

    TEST_F(PublishedPrenoteMapperTest, ItIgnoresAirfieldPairingPrenotesForTheWrongFlightRule)
    {
        this->airfields.AddAirfield(std::make_shared<AirfieldModel>(
            1,
            "EGGD",
            nullptr,
            std::vector<std::shared_ptr<PairedAirfieldPrenote>>{
                std::make_shared<PairedAirfieldPrenote>(2, 2, 1),
                std::make_shared<PairedAirfieldPrenote>(2, 1, 2),
            }));

        this->airfields.AddAirfield(
            std::make_shared<AirfieldModel>(2, "EGKK", nullptr, std::vector<std::shared_ptr<PairedAirfieldPrenote>>{}));

        EXPECT_EQ(
            std::set<std::shared_ptr<PublishedPrenote>>({this->prenotes.Get(2)}),
            this->mapper.MapForFlightplan(this->mockFlightplan));
    }

    TEST_F(PublishedPrenoteMapperTest, ItIgnoresAirfieldPairingPrenotesForTheWrongAirfield)
    {
        this->airfields.AddAirfield(std::make_shared<AirfieldModel>(
            1,
            "EGGD",
            nullptr,
            std::vector<std::shared_ptr<PairedAirfieldPrenote>>{
                std::make_shared<PairedAirfieldPrenote>(2, 1, 1),
                std::make_shared<PairedAirfieldPrenote>(3, 1, 2),
            }));

        this->airfields.AddAirfield(
            std::make_shared<AirfieldModel>(2, "EGKK", nullptr, std::vector<std::shared_ptr<PairedAirfieldPrenote>>{}));

        EXPECT_EQ(
            std::set<std::shared_ptr<PublishedPrenote>>({this->prenotes.Get(1)}),
            this->mapper.MapForFlightplan(this->mockFlightplan));
    }

    TEST_F(PublishedPrenoteMapperTest, ItIgnoresArrivalAirfieldsThatDontExistForAirfieldPairings)
    {
        this->airfields.AddAirfield(std::make_shared<AirfieldModel>(
            1,
            "EGGD",
            nullptr,
            std::vector<std::shared_ptr<PairedAirfieldPrenote>>{
                std::make_shared<PairedAirfieldPrenote>(2, 1, 1),
                std::make_shared<PairedAirfieldPrenote>(2, 1, 2),
            }));

        EXPECT_EQ(std::set<std::shared_ptr<PublishedPrenote>>({}), this->mapper.MapForFlightplan(this->mockFlightplan));
    }

    TEST_F(PublishedPrenoteMapperTest, ItIgnoresDepartureAirfieldsThatDontExistForAirfieldPairings)
    {
        this->airfields.AddAirfield(
            std::make_shared<AirfieldModel>(2, "EGKK", nullptr, std::vector<std::shared_ptr<PairedAirfieldPrenote>>{}));

        EXPECT_EQ(std::set<std::shared_ptr<PublishedPrenote>>({}), this->mapper.MapForFlightplan(this->mockFlightplan));
    }

    TEST_F(PublishedPrenoteMapperTest, ItIgnoresFlightRulesThatDontExistForAirfieldPairings)
    {
        ON_CALL(mockFlightplan, GetFlightRules).WillByDefault(testing::Return("X"));

        this->airfields.AddAirfield(std::make_shared<AirfieldModel>(
            1,
            "EGGD",
            nullptr,
            std::vector<std::shared_ptr<PairedAirfieldPrenote>>{
                std::make_shared<PairedAirfieldPrenote>(2, 1, 1),
                std::make_shared<PairedAirfieldPrenote>(2, 1, 2),
            }));

        this->airfields.AddAirfield(
            std::make_shared<AirfieldModel>(2, "EGKK", nullptr, std::vector<std::shared_ptr<PairedAirfieldPrenote>>{}));

        EXPECT_EQ(std::set<std::shared_ptr<PublishedPrenote>>({}), this->mapper.MapForFlightplan(this->mockFlightplan));
    }

    TEST_F(PublishedPrenoteMapperTest, ItRemovesDuplicatePrenotes)
    {
        EXPECT_CALL(sidMapper, MapFlightplanToSid(testing::Ref(mockFlightplan)))
            .Times(1)
            .WillOnce(testing::Return(
                std::make_shared<StandardInstrumentDeparture>(1, 2, "BADIM1X", 3000, 100, 1, std::set<int>({1, 2}))));

        this->airfields.AddAirfield(std::make_shared<AirfieldModel>(
            1,
            "EGGD",
            nullptr,
            std::vector<std::shared_ptr<PairedAirfieldPrenote>>{
                std::make_shared<PairedAirfieldPrenote>(2, 1, 2),
                std::make_shared<PairedAirfieldPrenote>(2, 1, 3),
            }));

        this->airfields.AddAirfield(
            std::make_shared<AirfieldModel>(2, "EGKK", nullptr, std::vector<std::shared_ptr<PairedAirfieldPrenote>>{}));

        EXPECT_EQ(
            std::set<std::shared_ptr<PublishedPrenote>>(
                {this->prenotes.Get(1), this->prenotes.Get(2), this->prenotes.Get(3)}),
            this->mapper.MapForFlightplan(this->mockFlightplan));
    }
} // namespace UKControllerPluginTest::Prenote
