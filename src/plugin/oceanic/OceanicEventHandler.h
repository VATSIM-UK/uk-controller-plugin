#pragma once
#include "Clearance.h"
#include "tag/TagItemInterface.h"
#include "timedevent/AbstractTimedEvent.h"

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
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Oceanic {
    /*
        Handles events over the ocean
    */
    class OceanicEventHandler : public TimedEvent::AbstractTimedEvent, public Tag::TagItemInterface
    {
        public:
        OceanicEventHandler(
            Curl::CurlInterface& curl,
            TaskManager::TaskRunnerInterface& taskRunner,
            Dialog::DialogManager& dialogManager);

        void TimedEventTrigger() override;
        [[nodiscard]] static auto NattrakClearanceValid(const nlohmann::json& clearance) -> bool;
        [[nodiscard]] auto CountClearances() const -> size_t;
        [[nodiscard]] auto GetClearanceForCallsign(const std::string& callsign) const -> const Clearance&;
        [[nodiscard]] auto GetTagItemDescription(int tagItemId) const -> std::string override;
        void TagFunction(
            Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
            const std::string& context,
            const POINT& mousePos);
        void SetTagItemData(Tag::TagData& tagData) override;
        [[nodiscard]] auto GetInvalidClearance() const -> const Clearance&;

        // Colours
        inline static const COLORREF clearanceIndicatorOk = RGB(36, 138, 73);
        inline static const COLORREF clearanceIndicatorActionRequired = RGB(255, 128, 0);
        inline static const COLORREF clearanceIndicatorPending = RGB(255, 153, 255);

        private:
        static auto GetDefaultClearanceForCallsign(Euroscope::EuroScopeCFlightPlanInterface& flightplan) -> Clearance;
        static auto ConvertNattrakLevelToEuroscope(const std::string& level) -> int;
        static auto NattrakLevelValid(std::string level) -> bool;
        [[nodiscard]] static auto GetClearedTagItemColour(int clearedLevel, int currentLevel) -> COLORREF;
        void SetClearanceIndicatorTagItem(Tag::TagData& tagData, const Clearance& clearance) const;
        void SetClearedLevelTagItem(Tag::TagData& tagData, const Clearance& clearance) const;
        static void SetClearedMachNumberTagItem(Tag::TagData& tagData, const Clearance& clearance);
        static void SetClearedEntryPointTagItem(Tag::TagData& tagData, const Clearance& clearance);
        static void SetClearedTrackIdentifierTagItem(Tag::TagData& tagData, const Clearance& clearance);
        static void SetClearedEntryTimeTagItem(Tag::TagData& tagData, const Clearance& clearance);

        // For making curl requests
        Curl::CurlInterface& curl;

        // For running things asynchronously
        TaskManager::TaskRunnerInterface& taskRunner;

        // All the clearances
        std::map<std::string, Clearance> clearances;

        // Dialog manager
        Dialog::DialogManager& dialogManager;

        // The URL to find nattrak
        const std::string nattrakUrl = "https://nattrak.vatsim.net/pluginapi.php";

        // Returned if clearance doesnt exist
        const Clearance invalidClearance = Clearance("NOTAVALIDCLEARANCESORRY");

        static const int NATTRAK_EUROSCOPE_FLIGHT_LEVEL_CONVERSION_FACTOR = 100;

        // Currently selected Clearance
        Clearance currentlySelectedClearance = this->invalidClearance;

        static const int CLEARANCE_INDICATOR_TAG_ITEM_ID = 118;
        static const int CLEARANCE_LEVEL_TAG_ITEM_ID = 119;
        static const int CLEARANCE_MACH_TAG_ITEM_ID = 120;
        static const int CLEARANCE_ENTRY_POINT_TAG_ITEM_ID = 121;
        static const int CLEARANCE_TRACK_TAG_ITEM_ID = 122;
        static const int CLEARANCE_ENTRY_ESTIMATE_TAG_ITEM_ID = 123;

        // Protects the map during updates
        mutable std::mutex clearanceMapMutex;
    };
} // namespace UKControllerPlugin::Oceanic
