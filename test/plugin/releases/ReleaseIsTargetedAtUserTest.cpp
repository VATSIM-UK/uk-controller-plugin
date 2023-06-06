#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "releases/DepartureReleaseRequest.h"
#include "releases/ReleaseIsTargetedAtUser.h"

namespace UKControllerPluginTest::Releases {
    class ReleaseIsTargetedAtUserTest : public ::testing::Test
    {
        protected:
        ReleaseIsTargetedAtUserTest()
            : position1(std::make_shared<UKControllerPlugin::Controller::ControllerPosition>(
                  1, "EGGD_APP", 125.650, std::vector<std::string>{}, true, true)),
              position2(std::make_shared<UKControllerPlugin::Controller::ControllerPosition>(
                  2, "EGGD_TWR", 133.850, std::vector<std::string>{}, true, true)),
              userCallsign("EGGD_APP", "EGGD_APP", *position1, true),
              nonUserCallsign("EGGD_TWR", "EGGD_TWR", *position2, false),
              callsignCollection(std::make_shared<UKControllerPlugin::Controller::ActiveCallsignCollection>()),
              releaseIsTargetedAtUser(callsignCollection)
        {
            callsignCollection->AddCallsign(nonUserCallsign);
        }

        std::shared_ptr<UKControllerPlugin::Controller::ControllerPosition> position1;
        std::shared_ptr<UKControllerPlugin::Controller::ControllerPosition> position2;
        UKControllerPlugin::Controller::ActiveCallsign userCallsign;
        UKControllerPlugin::Controller::ActiveCallsign nonUserCallsign;
        std::shared_ptr<UKControllerPlugin::Controller::ActiveCallsignCollection> callsignCollection;
        UKControllerPlugin::Releases::ReleaseIsTargetedAtUser releaseIsTargetedAtUser;
    };

    TEST_F(ReleaseIsTargetedAtUserTest, IsRelevantReturnsTrueWhenCallsignIsUser)
    {
        callsignCollection->AddUserCallsign(userCallsign);
        UKControllerPlugin::Releases::DepartureReleaseRequest releaseRequest(
            1, "BAW555", 99, 1, std::chrono::system_clock::now());
        EXPECT_TRUE(releaseIsTargetedAtUser.IsRelevant(releaseRequest));
    }

    TEST_F(ReleaseIsTargetedAtUserTest, IsRelevantReturnsFalseWhenCallsignIsNotUser)
    {
        callsignCollection->AddUserCallsign(userCallsign);
        UKControllerPlugin::Releases::DepartureReleaseRequest releaseRequest(
            1, "BAW555", 99, 2, std::chrono::system_clock::now());
        EXPECT_FALSE(releaseIsTargetedAtUser.IsRelevant(releaseRequest));
    }

    TEST_F(ReleaseIsTargetedAtUserTest, IsRelevantReturnsFalseWhenUserCallsignIsNotInCollection)
    {
        UKControllerPlugin::Releases::DepartureReleaseRequest releaseRequest(
            1, "BAW555", 99, 1, std::chrono::system_clock::now());
        EXPECT_FALSE(releaseIsTargetedAtUser.IsRelevant(releaseRequest));
    }
} // namespace UKControllerPluginTest::Releases
