#pragma once
#include "timedevent/AbstractTimedEvent.h"
#include "oceanic/Clearance.h"
#include "tag/TagItemInterface.h"

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
        class OceanicEventHandler : public TimedEvent::AbstractTimedEvent, public Tag::TagItemInterface
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

                std::string GetTagItemDescription(int tagItemId) const override;
                void SetTagItemData(Tag::TagData& tagData) override;
                const Clearance& invalidClearance{"NOTAVALIDCLEARANCESORRY"};

                const COLORREF clearanceIndicatorOk = RGB(36, 138, 73);
                const COLORREF clearanceIndicatorActionRequired = RGB(255, 128, 0);
                const COLORREF clearanceIndicatorPending = RGB(255, 153, 255);

            private:

                int ConvertNattrakLevelToEuroscope(std::string level) const;
                bool NattrakLevelValid(std::string level) const;
                COLORREF GetClearedTagItemColour(int clearedLevel, int currentLevel) const;

                // For making curl requests
                Curl::CurlInterface& curl;

                // For running things asynchronously
                TaskManager::TaskRunnerInterface& taskRunner;

                // All the clearances
                std::map<std::string, Clearance> clearances;
        };
    }  // namespace Oceanic
}  // namespace UKControllerPlugin
