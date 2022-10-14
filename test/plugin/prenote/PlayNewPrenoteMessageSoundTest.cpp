#include "controller/ControllerPosition.h"
#include "prenote/PlayNewPrenoteMessageSound.h"
#include "prenote/PrenoteMessage.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Prenote::PlayNewPrenoteMessageSound;
using UKControllerPlugin::Prenote::PrenoteMessage;

namespace UKControllerPluginTest::Prenote {
    class PlayNewPrenoteMessageSoundTest : public testing::Test
    {
        public:
        PlayNewPrenoteMessageSoundTest()
            : mockPrenoteRelevance(std::make_shared<testing::NiceMock<MockPrenoteUserRelevanceChecker>>()),
              playSound(mockPrenoteRelevance, windows)
        {
            sendingPosition = std::make_shared<ControllerPosition>(
                1, "EGKK_TWR", 124.225, std::vector<std::string>{"EGKK"}, true, false);
            receivingPosition = std::make_shared<ControllerPosition>(
                2, "EGKK_F_APP", 124.225, std::vector<std::string>{"EGKK"}, true, false);
        }

        std::shared_ptr<ControllerPosition> sendingPosition;
        std::shared_ptr<ControllerPosition> receivingPosition;
        std::shared_ptr<testing::NiceMock<MockPrenoteUserRelevanceChecker>> mockPrenoteRelevance;
        testing::NiceMock<Windows::MockWinApi> windows;
        PlayNewPrenoteMessageSound playSound;
    };

    TEST_F(PlayNewPrenoteMessageSoundTest, ItPlaysSoundOnNewPrenoteMessage)
    {
        const PrenoteMessage message(
            1,
            "BAW123",
            "EGKK",
            "TEST1A",
            "EGLL",
            sendingPosition,
            receivingPosition,
            std::chrono::system_clock::now());
        EXPECT_CALL(windows, PlayWave(MAKEINTRESOURCE(WAVE_NEW_PRENOTE))).Times(1); // NOLINT
        EXPECT_CALL(*mockPrenoteRelevance, IsRelevant(testing::Ref(message))).Times(1).WillOnce(testing::Return(true));
        playSound.NewMessage(message);
    }

    TEST_F(PlayNewPrenoteMessageSoundTest, ItDoesntPlaySoundIfNotRelevantToController)
    {
        const PrenoteMessage message(
            1,
            "BAW123",
            "EGKK",
            "TEST1A",
            "EGLL",
            sendingPosition,
            receivingPosition,
            std::chrono::system_clock::now());
        EXPECT_CALL(windows, PlayWave(MAKEINTRESOURCE(WAVE_NEW_PRENOTE))).Times(0); // NOLINT
        EXPECT_CALL(*mockPrenoteRelevance, IsRelevant(testing::Ref(message))).Times(1).WillOnce(testing::Return(false));
        playSound.NewMessage(message);
    }
} // namespace UKControllerPluginTest::Prenote
