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

    namespace Dialog {
        class DialogManager;
    } // namespace Dialog

    namespace Oceanic {
        /*
            Handles events over the ocean
        */
        class OceanicEventHandler : public TimedEvent::AbstractTimedEvent, public Tag::TagItemInterface
        {
            public:
                OceanicEventHandler(
                    Curl::CurlInterface& curl,
                    TaskManager::TaskRunnerInterface& taskRunner,
                    Dialog::DialogManager& dialogManager
                );

                void TimedEventTrigger() override;
                bool NattrakClearanceValid(const nlohmann::json& clearance) const;
                size_t CountClearances() const;
                const Clearance& GetClearanceForCallsign(std::string callsign) const;
                std::string GetTagItemDescription(int tagItemId) const override;
                void TagFunction(
                    Euroscope::EuroScopeCFlightPlanInterface& flightplan,
                    Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
                    std::string context,
                    const POINT& mousePos
                );
                void SetTagItemData(Tag::TagData& tagData) override;


                const Clearance invalidClearance{"NOTAVALIDCLEARANCESORRY"};
                const COLORREF clearanceIndicatorOk = RGB(36, 138, 73);
                const COLORREF clearanceIndicatorActionRequired = RGB(255, 128, 0);
                const COLORREF clearanceIndicatorPending = RGB(255, 153, 255);

            private:

                int ConvertNattrakLevelToEuroscope(std::string level) const;
                bool NattrakLevelValid(std::string level) const;
                COLORREF GetClearedTagItemColour(int clearedLevel, int currentLevel) const;
                void SetClearanceIndicatorTagItem(Tag::TagData& tagData, const Clearance& clearance) const;
                void SetClearedLevelTagItem(Tag::TagData& tagData, const Clearance& clearance) const;
                static void SetClearedMachNumberTagItem(Tag::TagData& tagData, const Clearance& clearance);
                static void SetClearedEntryPointTagItem(Tag::TagData& tagData, const Clearance& clearance);

                // For making curl requests
                Curl::CurlInterface& curl;

                // For running things asynchronously
                TaskManager::TaskRunnerInterface& taskRunner;

                // All the clearances
                std::map<std::string, Clearance> clearances;

                // Dialog manager
                Dialog::DialogManager& dialogManager;

                // Currently selected Clearance
                Clearance currentlySelectedClearance = this->invalidClearance;
        };
    } // namespace Oceanic
} // namespace UKControllerPlugin
