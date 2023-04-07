#include "approach/ApproachSequence.h"
#include "approach/ApproachSequencer.h"
#include "approach/RemoveLandedAircraft.h"

using UKControllerPlugin::Approach::ApproachSequencer;
using UKControllerPlugin::Approach::ApproachSequencingMode;
using UKControllerPlugin::Approach::RemoveLandedAircraft;

namespace UKControllerPluginTest::Approach {
    class RemoveLandedAircraftTest : public testing::Test
    {
        public:
        RemoveLandedAircraftTest() : remover(sequencer, plugin)
        {
            radarTarget1 = std::make_shared<Euroscope::MockEuroScopeCRadarTargetInterface>();
            radarTarget2 = std::make_shared<Euroscope::MockEuroScopeCRadarTargetInterface>();
            radarTarget3 = std::make_shared<Euroscope::MockEuroScopeCRadarTargetInterface>();

            ON_CALL(plugin, GetRadarTargetForCallsign("BAW123")).WillByDefault(testing::Return(radarTarget1));
            ON_CALL(plugin, GetRadarTargetForCallsign("BAW456")).WillByDefault(testing::Return(radarTarget2));
            ON_CALL(plugin, GetRadarTargetForCallsign("BAW789")).WillByDefault(testing::Return(radarTarget3));

            ON_CALL(*radarTarget1, GetFlightLevel).WillByDefault(testing::Return(1000));
            ON_CALL(*radarTarget1, GetGroundSpeed).WillByDefault(testing::Return(60));
            ON_CALL(*radarTarget2, GetFlightLevel).WillByDefault(testing::Return(1000));
            ON_CALL(*radarTarget2, GetGroundSpeed).WillByDefault(testing::Return(60));
            ON_CALL(*radarTarget3, GetFlightLevel).WillByDefault(testing::Return(1000));
            ON_CALL(*radarTarget3, GetGroundSpeed).WillByDefault(testing::Return(60));

            sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::WakeTurbulence);
            sequencer.AddAircraftToSequence("EGLL", "BAW456", ApproachSequencingMode::WakeTurbulence);
            sequencer.AddAircraftToSequence("EGLL", "BAW789", ApproachSequencingMode::WakeTurbulence);
        }

        std::shared_ptr<Euroscope::MockEuroScopeCRadarTargetInterface> radarTarget1;
        std::shared_ptr<Euroscope::MockEuroScopeCRadarTargetInterface> radarTarget2;
        std::shared_ptr<Euroscope::MockEuroScopeCRadarTargetInterface> radarTarget3;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
        ApproachSequencer sequencer;
        RemoveLandedAircraft remover;
    };

    TEST_F(RemoveLandedAircraftTest, TimedEventRemovesLandedAircraft)
    {
        ON_CALL(*radarTarget2, GetFlightLevel).WillByDefault(testing::Return(999));
        ON_CALL(*radarTarget2, GetGroundSpeed).WillByDefault(testing::Return(59));

        remover.TimedEventTrigger();

        EXPECT_EQ(std::list<std::string>({"BAW123"}), sequencer.GetForAirfield("EGKK").Callsigns());
        EXPECT_EQ(std::list<std::string>({"BAW789"}), sequencer.GetForAirfield("EGLL").Callsigns());
    }

    TEST_F(RemoveLandedAircraftTest, TimedEventRemovesMultipleLandedAircraft)
    {
        ON_CALL(*radarTarget1, GetFlightLevel).WillByDefault(testing::Return(999));
        ON_CALL(*radarTarget1, GetGroundSpeed).WillByDefault(testing::Return(59));
        ON_CALL(*radarTarget2, GetFlightLevel).WillByDefault(testing::Return(999));
        ON_CALL(*radarTarget2, GetGroundSpeed).WillByDefault(testing::Return(59));
        ON_CALL(*radarTarget3, GetFlightLevel).WillByDefault(testing::Return(999));
        ON_CALL(*radarTarget3, GetGroundSpeed).WillByDefault(testing::Return(59));

        remover.TimedEventTrigger();

        EXPECT_EQ(std::list<std::string>(), sequencer.GetForAirfield("EGKK").Callsigns());
        EXPECT_EQ(std::list<std::string>(), sequencer.GetForAirfield("EGLL").Callsigns());
    }

    TEST_F(RemoveLandedAircraftTest, TimedEventDoesntRemoveAircraftIfSpeedIsTooHigh)
    {
        ON_CALL(*radarTarget2, GetFlightLevel).WillByDefault(testing::Return(999));
        ON_CALL(*radarTarget2, GetGroundSpeed).WillByDefault(testing::Return(60));

        remover.TimedEventTrigger();

        EXPECT_EQ(std::list<std::string>({"BAW123"}), sequencer.GetForAirfield("EGKK").Callsigns());
        EXPECT_EQ(std::list<std::string>({"BAW456", "BAW789"}), sequencer.GetForAirfield("EGLL").Callsigns());
    }

    TEST_F(RemoveLandedAircraftTest, TimedEventDoesntRemoveAircraftIfAltitudeIsTooHigh)
    {
        ON_CALL(*radarTarget2, GetFlightLevel).WillByDefault(testing::Return(1000));
        ON_CALL(*radarTarget2, GetGroundSpeed).WillByDefault(testing::Return(59));

        remover.TimedEventTrigger();

        EXPECT_EQ(std::list<std::string>({"BAW123"}), sequencer.GetForAirfield("EGKK").Callsigns());
        EXPECT_EQ(std::list<std::string>({"BAW456", "BAW789"}), sequencer.GetForAirfield("EGLL").Callsigns());
    }

    TEST_F(RemoveLandedAircraftTest, TimedEventDoesntRemoveAircraftIfRadarTargetIsNotFound)
    {
        ON_CALL(plugin, GetRadarTargetForCallsign("BAW456")).WillByDefault(testing::Return(nullptr));
        ON_CALL(*radarTarget2, GetFlightLevel).WillByDefault(testing::Return(999));
        ON_CALL(*radarTarget2, GetGroundSpeed).WillByDefault(testing::Return(59));

        remover.TimedEventTrigger();

        EXPECT_EQ(std::list<std::string>({"BAW123"}), sequencer.GetForAirfield("EGKK").Callsigns());
        EXPECT_EQ(std::list<std::string>({"BAW456", "BAW789"}), sequencer.GetForAirfield("EGLL").Callsigns());
    }
} // namespace UKControllerPluginTest::Approach
