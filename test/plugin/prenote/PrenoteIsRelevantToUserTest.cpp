#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "prenote/PrenoteIsRelevantToUser.h"
#include "prenote/PrenoteMessage.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Prenote::PrenoteIsRelevantToUser;
using UKControllerPlugin::Prenote::PrenoteMessage;

namespace UKControllerPluginTest::Prenote {
    class PrenoteIsRelevantToUserTest : public testing::Test
    {
        public:
        PrenoteIsRelevantToUserTest() : relevance(activeCallsigns)
        {
            sendingPosition = std::make_unique<ControllerPosition>(
                1, "EGKK_TWR", 124.225, std::vector<std::string>{"EGKK"}, true, false);
            receivingPosition = std::make_unique<ControllerPosition>(
                2, "EGKK_F_APP", 124.225, std::vector<std::string>{"EGKK"}, true, false);
            otherReceivingPosition = std::make_unique<ControllerPosition>(
                3, "EGKK_APP", 124.225, std::vector<std::string>{"EGKK"}, true, false);
        }
        ActiveCallsignCollection activeCallsigns;
        PrenoteIsRelevantToUser relevance;
        std::shared_ptr<ControllerPosition> sendingPosition;
        std::shared_ptr<ControllerPosition> receivingPosition;
        std::shared_ptr<ControllerPosition> otherReceivingPosition;
    };

    TEST_F(PrenoteIsRelevantToUserTest, PrenoteIsReleevantTouser)
    {
        this->activeCallsigns.AddUserCallsign(ActiveCallsign("EGKK_TWR", "Testy McTest", *receivingPosition, false));
        const PrenoteMessage message(
            1,
            "BAW123",
            "EGKK",
            "TEST1A",
            "EGLL",
            sendingPosition,
            receivingPosition,
            std::chrono::system_clock::now());
        EXPECT_TRUE(relevance.IsRelevant(message));
    }

    TEST_F(PrenoteIsRelevantToUserTest, ItIsNotRelevantIfControllerIsAlsoReceiving)
    {
        this->activeCallsigns.AddUserCallsign(ActiveCallsign("EGKK_TWR", "Testy McTest", *sendingPosition, false));
        const PrenoteMessage message(
            1, "BAW123", "EGKK", "TEST1A", "EGLL", sendingPosition, sendingPosition, std::chrono::system_clock::now());
        EXPECT_FALSE(relevance.IsRelevant(message));
    }

    TEST_F(PrenoteIsRelevantToUserTest, ItIsNotRelevantIfTargetingWrongController)
    {
        this->activeCallsigns.AddUserCallsign(ActiveCallsign("EGKK_TWR", "Testy McTest", *receivingPosition, false));
        const PrenoteMessage message(
            1,
            "BAW123",
            "EGKK",
            "TEST1A",
            "EGLL",
            sendingPosition,
            otherReceivingPosition,
            std::chrono::system_clock::now());
        EXPECT_FALSE(relevance.IsRelevant(message));
    }

    TEST_F(PrenoteIsRelevantToUserTest, ItDoesntPlaySoundIfUserNotActive)
    {
        this->activeCallsigns.AddCallsign(ActiveCallsign("EGKK_TWR", "Testy McTest", *receivingPosition, false));
        const PrenoteMessage message(
            1,
            "BAW123",
            "EGKK",
            "TEST1A",
            "EGLL",
            sendingPosition,
            receivingPosition,
            std::chrono::system_clock::now());
        EXPECT_FALSE(relevance.IsRelevant(message));
    }
} // namespace UKControllerPluginTest::Prenote
