#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "prenote/PrenoteIsTargetedToUser.h"
#include "prenote/PrenoteMessage.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Prenote::PrenoteIsTargetedToUser;
using UKControllerPlugin::Prenote::PrenoteMessage;

namespace UKControllerPluginTest::Prenote {
    class PrenoteIsTargetedToUserTest : public testing::Test
    {
        public:
        PrenoteIsTargetedToUserTest() : relevance(activeCallsigns)
        {
            sendingPosition = std::make_unique<ControllerPosition>(
                1, "EGKK_TWR", 124.225, std::vector<std::string>{"EGKK"}, true, false);
            receivingPosition = std::make_unique<ControllerPosition>(
                2, "EGKK_F_APP", 124.225, std::vector<std::string>{"EGKK"}, true, false);
            otherReceivingPosition = std::make_unique<ControllerPosition>(
                3, "EGKK_APP", 124.225, std::vector<std::string>{"EGKK"}, true, false);
        }
        ActiveCallsignCollection activeCallsigns;
        PrenoteIsTargetedToUser relevance;
        std::shared_ptr<ControllerPosition> sendingPosition;
        std::shared_ptr<ControllerPosition> receivingPosition;
        std::shared_ptr<ControllerPosition> otherReceivingPosition;
    };

    TEST_F(PrenoteIsTargetedToUserTest, PrenoteIsRelevantTouser)
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

    TEST_F(PrenoteIsTargetedToUserTest, ItIsNotRelevantIfControllerIsAlsoReceiving)
    {
        this->activeCallsigns.AddUserCallsign(ActiveCallsign("EGKK_TWR", "Testy McTest", *sendingPosition, false));
        const PrenoteMessage message(
            1, "BAW123", "EGKK", "TEST1A", "EGLL", sendingPosition, sendingPosition, std::chrono::system_clock::now());
        EXPECT_FALSE(relevance.IsRelevant(message));
    }

    TEST_F(PrenoteIsTargetedToUserTest, ItIsNotRelevantIfTargetingWrongController)
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

    TEST_F(PrenoteIsTargetedToUserTest, ItDoesntPlaySoundIfUserNotActive)
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
