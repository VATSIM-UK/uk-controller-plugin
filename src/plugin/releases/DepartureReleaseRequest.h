#pragma once

namespace UKControllerPlugin::Releases {

    class DepartureReleaseRequest
    {
        public:
        DepartureReleaseRequest(
            int id,
            std::string callsign,
            int requestingController,
            int targetController,
            std::chrono::system_clock::time_point requestExpiresAt);

        void Acknowledge();
        void Reject();
        void Approve(
            std::chrono::system_clock::time_point releasedAtTime,
            std::chrono::system_clock::time_point releaseExpiresAt);

        void Approve(std::chrono::system_clock::time_point releasedAtTime);

        auto Id() const -> int;
        auto RequiresDecision() const -> bool;
        auto Callsign() const -> std::string;
        auto RequestingController() const -> int;
        auto TargetController() const -> int;
        auto Acknowledged() const -> bool;
        auto Rejected() const -> bool;
        auto Approved() const -> bool;
        auto RequestExpired() const -> bool;
        auto ApprovalExpired() const -> bool;
        auto AwaitingReleasedTime() const -> bool;
        auto ApprovedWithNoExpiry() const -> bool;
        auto RequestExpiryTime() const -> std::chrono::system_clock::time_point;
        auto ReleaseExpiryTime() const -> std::chrono::system_clock::time_point;
        auto ReleasedAtTime() const -> std::chrono::system_clock::time_point;
        auto RejectedAtTime() const -> std::chrono::system_clock::time_point;
        auto AcknowledgedAtTime() const -> std::chrono::system_clock::time_point;
        auto CreatedAt() const -> std::chrono::system_clock::time_point;

        private:
        static inline const std::chrono::system_clock::time_point noTime =
            (std::chrono::system_clock::time_point::min)();

        static inline const std::chrono::system_clock::time_point noTimeMax =
            (std::chrono::system_clock::time_point::max)() - std::chrono::hours(500);

        // The id of the request
        int id;

        // The callsign for the request
        std::string callsign;

        // Who's requesting the release
        int requestingController;

        // Who's the target of the release request
        int targetController;

        // When the request expires
        std::chrono::system_clock::time_point requestExpiresAt;

        // What time the release was acknowledged
        std::chrono::system_clock::time_point acknowledgedAtTime = noTime;

        // What time the release was rejected
        std::chrono::system_clock::time_point rejectedAtTime = noTime;

        // The time the aircraft has been approved for release
        std::chrono::system_clock::time_point releasedAtTime = noTime;

        // The time that the release expires
        std::chrono::system_clock::time_point releaseExpiresAt = noTimeMax;

        // When the request was created
        std::chrono::system_clock::time_point createdAt;
    };
} // namespace UKControllerPlugin::Releases
