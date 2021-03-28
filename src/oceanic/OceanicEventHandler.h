#pragma once
#include "timedevent/AbstractTimedEvent.h"
#include "oceanic/Clearance.h"

namespace UKControllerPlugin {

    namespace Curl {
        class CurlInterface;
    } // namespace Curl

    namespace TaskManager {
        class TaskRunnerInterface;
    } // namespace TaskManager

    namespace Oceanic {
        /*
            Handles events over the ocean
        */
        class OceanicEventHandler : public TimedEvent::AbstractTimedEvent
        {
            public:
                OceanicEventHandler(
                    Curl::CurlInterface& curl,
                    TaskManager::TaskRunnerInterface& taskRunner
                );

                void TimedEventTrigger() override;
                bool NattrakClearanceValid(const nlohmann::json& clearance) const;
                size_t CountClearances() const;
                const Clearance& GetClearanceForCallsign(std::string callsign) const;

                const Clearance& invalidClearance{"NOTAVALIDCLEARANCESORRY"};

            private:

                // For making curl requests
                Curl::CurlInterface& curl;

                // For running things asynchronously
                TaskManager::TaskRunnerInterface& taskRunner;

                // All the clearances
                std::map<std::string, Clearance> clearances;
        };
    }  // namespace Oceanic
}  // namespace UKControllerPlugin
