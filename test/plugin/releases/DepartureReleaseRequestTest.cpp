#include "releases/DepartureReleaseRequest.h"
#include "time/SystemClock.h"

using testing::Test;
using UKControllerPlugin::Releases::DepartureReleaseRequest;
using UKControllerPlugin::Time::SetTestNow;
using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPluginTes::Releases {

    class DepartureReleaseRequestTest : public Test
    {
        public:
        DepartureReleaseRequestTest()
            : requestExpiresAt(std::chrono::system_clock::now() + std::chrono::minutes(3)),
              request(1, "BAW123", 2, 3, requestExpiresAt)
        {
            SetTestNow(std::chrono::system_clock::now());
        }

        std::chrono::system_clock::time_point requestExpiresAt;
        DepartureReleaseRequest request;
    };

    TEST_F(DepartureReleaseRequestTest, ItHasAnId)
    {
        EXPECT_EQ(1, request.Id());
    }

    TEST_F(DepartureReleaseRequestTest, ItHasACallsign)
    {
        EXPECT_EQ("BAW123", request.Callsign());
    }

    TEST_F(DepartureReleaseRequestTest, ItHasARequestingController)
    {
        EXPECT_EQ(2, request.RequestingController());
    }

    TEST_F(DepartureReleaseRequestTest, ItHasATargetController)
    {
        EXPECT_EQ(3, request.TargetController());
    }

    TEST_F(DepartureReleaseRequestTest, ItHasARequestExpiryTime)
    {
        EXPECT_EQ(requestExpiresAt, request.RequestExpiryTime());
    }

    TEST_F(DepartureReleaseRequestTest, ItStartsNotAcknowledged)
    {
        EXPECT_FALSE(request.Acknowledged());
    }

    TEST_F(DepartureReleaseRequestTest, ItStartsNotApproved)
    {
        EXPECT_FALSE(request.Approved());
    }

    TEST_F(DepartureReleaseRequestTest, ItStartsNotRejected)
    {
        EXPECT_FALSE(request.Rejected());
    }

    TEST_F(DepartureReleaseRequestTest, ItCanBeAcknowledged)
    {
        request.Acknowledge();
        EXPECT_TRUE(request.Acknowledged());
    }

    TEST_F(DepartureReleaseRequestTest, AcknowledgingTheRequestSetsAcknowledgeTime)
    {
        request.Acknowledge();
        EXPECT_TRUE(std::chrono::system_clock::now() - request.AcknowledgedAtTime() < std::chrono::seconds(5));
    }

    TEST_F(DepartureReleaseRequestTest, ItCanBeRejected)
    {
        request.Reject();
        EXPECT_TRUE(request.Rejected());
    }

    TEST_F(DepartureReleaseRequestTest, RejectingTheRequestSetsRejectTime)
    {
        request.Reject();
        EXPECT_TRUE(std::chrono::system_clock::now() - request.RejectedAtTime() < std::chrono::seconds(5));
    }

    TEST_F(DepartureReleaseRequestTest, ItCanBeApproved)
    {
        request.Approve(std::chrono::system_clock::now(), std::chrono::system_clock::now());
        EXPECT_TRUE(request.Approved());
        EXPECT_FALSE(request.ApprovedWithNoExpiry());
    }

    TEST_F(DepartureReleaseRequestTest, ItCanBeApprovedWithNoExpiryTime)
    {
        request.Approve(std::chrono::system_clock::now(), std::chrono::system_clock::now());
        EXPECT_TRUE(request.Approved());
        EXPECT_FALSE(request.ApprovedWithNoExpiry());
    }

    TEST_F(DepartureReleaseRequestTest, ApprovingTheRequestSetsReleasedAtTime)
    {
        std::chrono::system_clock::time_point releasedAt = std::chrono::system_clock::now() + std::chrono::minutes(1);
        std::chrono::system_clock::time_point releaseExpiresAt =
            std::chrono::system_clock::now() + std::chrono::minutes(2);
        request.Approve(releasedAt, releaseExpiresAt);
        EXPECT_EQ(releasedAt, request.ReleasedAtTime());
    }

    TEST_F(DepartureReleaseRequestTest, ApprovingTheRequestSetsReleaseExpiryTime)
    {
        std::chrono::system_clock::time_point releasedAt = std::chrono::system_clock::now() + std::chrono::minutes(1);
        std::chrono::system_clock::time_point releaseExpiresAt =
            std::chrono::system_clock::now() + std::chrono::minutes(2);
        request.Approve(releasedAt, releaseExpiresAt);
        EXPECT_EQ(releaseExpiresAt, request.ReleaseExpiryTime());
    }

    TEST_F(DepartureReleaseRequestTest, RequestNotExpiredIfTimeHasNotPassed)
    {
        EXPECT_FALSE(this->request.RequestExpired());
    }

    TEST_F(DepartureReleaseRequestTest, RequestExpiredIfTimePassed)
    {
        UKControllerPlugin::Time::SetTestNow(std::chrono::system_clock::now() + std::chrono::minutes(4));
        EXPECT_TRUE(this->request.RequestExpired());
    }

    TEST_F(DepartureReleaseRequestTest, RequestRequiresDecision)
    {
        EXPECT_TRUE(this->request.RequiresDecision());
    }

    TEST_F(DepartureReleaseRequestTest, RequestDoesNotRequireDecisionIfExpired)
    {
        UKControllerPlugin::Time::SetTestNow(std::chrono::system_clock::now() + std::chrono::minutes(4));
        EXPECT_FALSE(this->request.RequiresDecision());
    }

    TEST_F(DepartureReleaseRequestTest, RequestDoesNotRequireDecisionIfAlreadyApproved)
    {
        request.Approve(std::chrono::system_clock::now(), std::chrono::system_clock::now());
        EXPECT_FALSE(this->request.RequiresDecision());
    }

    TEST_F(DepartureReleaseRequestTest, RequestDoesNotRequireDecisionIfAlreadyRejected)
    {
        request.Reject();
        EXPECT_FALSE(this->request.RequiresDecision());
    }

    TEST_F(DepartureReleaseRequestTest, RequestsCanBePendingReleasedAtTime)
    {
        std::chrono::system_clock::time_point releasedAt = std::chrono::system_clock::now() + std::chrono::minutes(1);
        std::chrono::system_clock::time_point releaseExpiresAt =
            std::chrono::system_clock::now() + std::chrono::minutes(2);
        request.Approve(releasedAt, releaseExpiresAt);
        EXPECT_TRUE(this->request.AwaitingReleasedTime());
    }

    TEST_F(DepartureReleaseRequestTest, RequestsCanBeReadyToGo)
    {
        std::chrono::system_clock::time_point releasedAt = std::chrono::system_clock::now() - std::chrono::minutes(1);
        std::chrono::system_clock::time_point releaseExpiresAt =
            std::chrono::system_clock::now() + std::chrono::minutes(2);
        request.Approve(releasedAt, releaseExpiresAt);
        EXPECT_FALSE(this->request.AwaitingReleasedTime());
    }

    TEST_F(DepartureReleaseRequestTest, ItHasACreatedAtTime)
    {
        std::chrono::system_clock::time_point releasedAt = std::chrono::system_clock::now() + std::chrono::minutes(1);
        std::chrono::system_clock::time_point releaseExpiresAt =
            std::chrono::system_clock::now() + std::chrono::minutes(2);
        request.Approve(releasedAt, releaseExpiresAt);
        EXPECT_EQ(TimeNow(), request.CreatedAt());
    }
} // namespace UKControllerPluginTes::Releases
