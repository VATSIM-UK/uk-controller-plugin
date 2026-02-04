#pragma once

#include "Clearance.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "tag/TagItemInterface.h"
#include "timedevent/AbstractTimedEvent.h"
#include <unordered_map>
#include <chrono>
#include <optional>

namespace UKControllerPlugin {
    namespace Curl {
        class CurlInterface;
    }

    namespace TaskManager {
        class TaskRunnerInterface;
    }

    namespace Dialog {
        class DialogManager;
    }

    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Oceanic {

    class OceanicEventHandler : public TimedEvent::AbstractTimedEvent,
                                public Tag::TagItemInterface,
                                public Flightplan::FlightPlanEventHandlerInterface
    {
        public:
        OceanicEventHandler(
            Curl::CurlInterface& curl,
            TaskManager::TaskRunnerInterface& taskRunner,
            Dialog::DialogManager& dialogManager);

        void TimedEventTrigger() override;

        void FlightPlanEvent(
            Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) override;

        void FlightPlanDisconnectEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan) override;

        void ControllerFlightPlanDataEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType) override;

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

        void SetCurrentlySelectedClearance(Euroscope::EuroScopeCFlightPlanInterface& flightplan);

        [[nodiscard]] static auto GetClearedTagItemColour(int clearedLevel, int currentLevel) -> COLORREF;

        void SetClearanceIndicatorTagItem(Tag::TagData& tagData, const Clearance& clearance) const;

        void SetClearedLevelTagItem(Tag::TagData& tagData, const Clearance& clearance) const;

        static void SetClearedMachNumberTagItem(Tag::TagData& tagData, const Clearance& clearance);

        static void SetClearedEntryPointTagItem(Tag::TagData& tagData, const Clearance& clearance);

        static void SetClearedTrackIdentifierTagItem(Tag::TagData& tagData, const Clearance& clearance);

        static void SetClearedEntryTimeTagItem(Tag::TagData& tagData, const Clearance& clearance);

        // Curl interface
        Curl::CurlInterface& curl;

        // Async tasks
        TaskManager::TaskRunnerInterface& taskRunner;

        // Stored clearances
        std::map<std::string, Clearance> clearances;

        // Dialog manager
        Dialog::DialogManager& dialogManager;

        // Nattrak API URLs
        const std::string nattrakUrl = "https://nattrak.vatsim.net/api/plugins";
        const std::string nattrakClxUrl = "https://nattrak.vatsim.net/api/clx-messages";

        // Returned if clearance doesn't exist
        const Clearance invalidClearance = Clearance("NOTAVALIDCLEARANCESORRY");

        static const int NATTRAK_EUROSCOPE_FLIGHT_LEVEL_CONVERSION_FACTOR = 100;

        // Currently selected clearance (initialised to invalid)
        Clearance currentlySelectedClearance{"NOTAVALIDCLEARANCESORRY"};

        // Tag IDs
        static const int CLEARANCE_INDICATOR_TAG_ITEM_ID = 118;
        static const int CLEARANCE_LEVEL_TAG_ITEM_ID = 119;
        static const int CLEARANCE_MACH_TAG_ITEM_ID = 120;
        static const int CLEARANCE_ENTRY_POINT_TAG_ITEM_ID = 121;
        static const int CLEARANCE_TRACK_TAG_ITEM_ID = 122;
        static const int CLEARANCE_ENTRY_ESTIMATE_TAG_ITEM_ID = 123;

        // Protects the clearance map
        mutable std::mutex clearanceMapMutex;

        // ===== On‑assume CLX refresh (debounced) =====
        std::unordered_map<std::string, std::chrono::steady_clock::time_point> lastAssumeClxFetch_;
        static constexpr std::chrono::seconds ASSUME_CLX_DEBOUNCE{10};

        bool ShouldFetchClxNow_(const std::string& callsign);
        void RefreshClxForCallsignAsync_(const std::string& callsign);
        std::optional<Clearance> BuildClearanceFromClx_(const nlohmann::json& json) const;
    };

} // namespace UKControllerPlugin::Oceanic