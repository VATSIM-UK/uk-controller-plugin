#pragma once
namespace UKControllerPlugin {
    namespace Releases {

        class DepartureReleaseRequest
        {
            public:
                DepartureReleaseRequest(
                    int id,
                    std::string callsign,
                    int requestingController,
                    int targetController,
                    std::chrono::system_clock::time_point requestExpiresAt
                );

                void Acknowledge();
                void Reject();
                void Approve(
                    std::chrono::system_clock::time_point releasedAtTime,
                    std::chrono::system_clock::time_point releaseExpiresAt
                );

                int Id() const;
                bool RequiresDecision() const;
                std::string Callsign() const;
                int RequestingController() const;
                int TargetController() const;
                bool Acknowledged() const;
                bool Rejected() const;
                bool Approved() const;
                bool RequestExpired() const;
                std::chrono::system_clock::time_point RequestExpiryTime() const;
                std::chrono::system_clock::time_point ReleaseExpiryTime() const;
                std::chrono::system_clock::time_point ReleasedAtTime() const;
                std::chrono::system_clock::time_point RejectedAtTime() const;
                std::chrono::system_clock::time_point AcknowledgedAtTime() const;

            private:

                static inline const std::chrono::system_clock::time_point noTime = (
                    std::chrono::system_clock::time_point::min
                )();

                // The id of the request
                int id;

                // The callsign for the request
                std::string callsign;

                // Who's requesting the release
                int requestingController;

                // Who's the target of the release request
                int targetController;

                // When the the request expires
                std::chrono::system_clock::time_point requestExpiresAt;

                // What time the release was acknowledged
                std::chrono::system_clock::time_point acknowledgedAtTime = noTime;

                // What time the release was rejected
                std::chrono::system_clock::time_point rejectedAtTime = noTime;

                // The time the aircraft has been approved for release
                std::chrono::system_clock::time_point releasedAtTime = noTime;

                // The time that the release expires
                std::chrono::system_clock::time_point releaseExpiresAt = noTime;
        };

    } // namespace Releases
} // namespace UKControllerPlugin
