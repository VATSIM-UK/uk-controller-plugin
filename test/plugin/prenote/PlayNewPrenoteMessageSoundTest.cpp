#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "prenote/PlayNewPrenoteMessageSound.h"
#include "prenote/PrenoteMessage.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Prenote::PlayNewPrenoteMessageSound;
using UKControllerPlugin::Prenote::PrenoteMessage;

namespace UKControllerPluginTest::Prenote {
    class PlayNewPrenoteMessageSoundTest : public testing::Test
    {
        public:
        PlayNewPrenoteMessageSoundTest() : playSound(activeCallsigns, windows)
        {
            position = std::make_unique<ControllerPosition>(
                2, "EGKK_TWR", 124.225, std::vector<std::string>{"EGKK"}, true, false);
        }
        testing::NiceMock<Windows::MockWinApi> windows;
        ActiveCallsignCollection activeCallsigns;
        PlayNewPrenoteMessageSound playSound;
        std::unique_ptr<ControllerPosition> position;
    };

    TEST_F(PlayNewPrenoteMessageSoundTest, ItPlaysSoundOnNewPrenoteMessage)
    {
        this->activeCallsigns.AddUserCallsign(ActiveCallsign("EGKK_TWR", "Testy McTest", *position, false));

        EXPECT_CALL(windows, PlayWave(MAKEINTRESOURCE(WAVE_NEW_PRENOTE))).Times(1); // NOLINT

        const PrenoteMessage message(1, "BAW123", "EGKK", "TEST1A", "EGLL", 1, 2, std::chrono::system_clock::now());
        playSound.NewMessage(message);
    }

    TEST_F(PlayNewPrenoteMessageSoundTest, ItDoesntPlaySoundOnSendingControllerIsAlsoReceiving)
    {
        this->activeCallsigns.AddUserCallsign(ActiveCallsign("EGKK_TWR", "Testy McTest", *position, false));

        EXPECT_CALL(windows, PlayWave(MAKEINTRESOURCE(WAVE_NEW_PRENOTE))).Times(0); // NOLINT

        const PrenoteMessage message(1, "BAW123", "EGKK", "TEST1A", "EGLL", 2, 2, std::chrono::system_clock::now());
        playSound.NewMessage(message);
    }

    TEST_F(PlayNewPrenoteMessageSoundTest, ItDoesntPlaySoundIfTargetingWrongController)
    {
        this->activeCallsigns.AddUserCallsign(ActiveCallsign("EGKK_TWR", "Testy McTest", *position, false));

        EXPECT_CALL(windows, PlayWave(MAKEINTRESOURCE(WAVE_NEW_PRENOTE))).Times(0); // NOLINT

        const PrenoteMessage message(1, "BAW123", "EGKK", "TEST1A", "EGLL", 1, 3, std::chrono::system_clock::now());
        playSound.NewMessage(message);
    }

    TEST_F(PlayNewPrenoteMessageSoundTest, ItDoesntPlaySoundIfUserNotActive)
    {
        this->activeCallsigns.AddCallsign(ActiveCallsign("EGKK_TWR", "Testy McTest", *position, false));

        EXPECT_CALL(windows, PlayWave(MAKEINTRESOURCE(WAVE_NEW_PRENOTE))).Times(0); // NOLINT

        const PrenoteMessage message(1, "BAW123", "EGKK", "TEST1A", "EGLL", 1, 2, std::chrono::system_clock::now());
        playSound.NewMessage(message);
    }
} // namespace UKControllerPluginTest::Prenote
