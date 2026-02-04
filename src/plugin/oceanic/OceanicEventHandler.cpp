#include "OceanicEventHandler.h"
#include "curl/CurlInterface.h"
#include "curl/CurlRequest.h"
#include "datablock/DatablockFunctions.h"
#include "dialog/DialogManager.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "tag/TagData.h"
#include "task/TaskRunnerInterface.h"

namespace UKControllerPlugin::Oceanic {

    OceanicEventHandler::OceanicEventHandler(
        Curl::CurlInterface& curl, TaskManager::TaskRunnerInterface& taskRunner, Dialog::DialogManager& dialogManager)
        : curl(curl), taskRunner(taskRunner), dialogManager(dialogManager)
    {
    }

    void OceanicEventHandler::TimedEventTrigger()
    {
        this->taskRunner.QueueAsynchronousTask([this]() {
            LogInfo("Updating oceanic clearance data from Nattrak");
            Curl::CurlRequest apiUpdateRequest(nattrakUrl, Curl::CurlRequest::METHOD_GET);
            Curl::CurlResponse apiUpdateResponse = this->curl.MakeCurlRequest(apiUpdateRequest);

            if (apiUpdateResponse.IsCurlError() || !apiUpdateResponse.StatusOk()) {
                LogWarning("Unable to retrieve oceanic clearances from Nattrak.");
                return;
            }

            nlohmann::json clearanceData;
            try {
                clearanceData = nlohmann::json::parse(apiUpdateResponse.GetResponse());
            } catch (nlohmann::json::exception& exception) {
                LogWarning("Unable to decode oceanic clearances from Nattrak, JSON parse failed.");
                return;
            }

            if (!clearanceData.is_array()) {
                LogWarning("Unable to decode oceanic clearances from Nattrak, JSON is not array.");
                return;
            }

            // Loop the clearances and update local data
            auto lock = std::lock_guard(this->clearanceMapMutex);
            this->clearances.clear();
            for (const nlohmann::json& clearance : clearanceData) {
                if (!NattrakClearanceValid(clearance)) {
                    LogWarning("Invalid clearance received from Nattrak");
                    continue;
                }

                this->clearances.insert(
                    std::pair<std::string, Clearance>(
                        clearance.at("callsign").get<std::string>(),
                        {
                            clearance.at("callsign").get<std::string>(),
                            clearance.at("status").get<std::string>(),
                            clearance.at("nat").is_null() ? "" : clearance.at("nat").get<std::string>(),
                            clearance.at("fix").get<std::string>(),
                            std::to_string(
                                Datablock::NormaliseFlightLevelFromString(clearance.at("level").get<std::string>())),
                            clearance.at("mach").get<std::string>(),
                            clearance.at("estimating_time").get<std::string>(),
                            clearance.at("clearance_issued").is_null()
                                ? ""
                                : clearance.at("clearance_issued").get<std::string>(),
                            clearance.at("extra_info").is_null() ? "" : clearance.at("extra_info").get<std::string>(),
                        }));
            }
            LogInfo("Finished updating oceanic clearance data");
        });
    }

    /*
        When a flightplan is updated (including when an aircraft is assumed), clear any cached
        clearance data so that fresh data is displayed when the next TimedEventTrigger runs.
        This fixes the issue where assumed aircraft would show incorrect track information.
    */
    void OceanicEventHandler::FlightPlanEvent(
        Euroscope::EuroScopeCFlightPlanInterface& flightPlan, Euroscope::EuroScopeCRadarTargetInterface& radarTarget)
    {
        {
            auto lock = std::lock_guard(this->clearanceMapMutex);
            this->clearances.erase(flightPlan.GetCallsign());
        }

        const std::string cs = flightPlan.GetCallsign();

        if (ShouldFetchClxNow_(cs)) {
            // Fetch detailed CLX for THIS callsign (accurate track)
            RefreshClxForCallsignAsync_(cs);

            // AND still run the normal plugin-feed poll so all aircraft stay updated
            this->TimedEventTrigger();
        }
    }

    /*
        When a flightplan disconnects, remove its clearance from the cache.
    */
    void OceanicEventHandler::FlightPlanDisconnectEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan)
    {
        auto lock = std::lock_guard(this->clearanceMapMutex);
        this->clearances.erase(flightPlan.GetCallsign());
    }

    /*
        Controller flightplan data events don't require any special handling for oceanic clearances.
    */
    void OceanicEventHandler::ControllerFlightPlanDataEvent(
        Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType)
    {
    }

    auto OceanicEventHandler::NattrakClearanceValid(const nlohmann::json& clearance) -> bool
    {
        return clearance.is_object() && clearance.contains("callsign") && clearance.at("callsign").is_string() &&
               clearance.contains("status") && clearance.at("status").is_string() && clearance.contains("nat") &&
               (clearance.at("nat").is_null() || clearance.at("nat").is_string()) && clearance.contains("fix") &&
               clearance.at("fix").is_string() && clearance.contains("level") && clearance.at("level").is_string() &&
               NattrakLevelValid(clearance.at("level").get<std::string>()) && clearance.contains("mach") &&
               clearance.at("mach").is_string() && clearance.contains("estimating_time") &&
               clearance.at("estimating_time").is_string() && clearance.contains("clearance_issued") &&
               (clearance.at("clearance_issued").is_string() || clearance.at("clearance_issued").is_null()) &&
               clearance.contains("extra_info") &&
               (clearance.at("extra_info").is_string() || clearance.at("extra_info").is_null());
    }

    auto OceanicEventHandler::CountClearances() const -> size_t
    {
        auto lock = std::lock_guard(this->clearanceMapMutex);
        return this->clearances.size();
    }

    auto OceanicEventHandler::GetClearanceForCallsign(const std::string& callsign) const -> const Clearance&
    {
        auto lock = std::lock_guard(this->clearanceMapMutex);
        auto clearance = this->clearances.find(callsign);
        return clearance == this->clearances.cend() ? this->invalidClearance : clearance->second;
    }

    auto OceanicEventHandler::GetTagItemDescription(int tagItemId) const -> std::string
    {
        switch (tagItemId) {
        case CLEARANCE_INDICATOR_TAG_ITEM_ID:
            return "Nattrak Oceanic Clearance Indicator";
        case CLEARANCE_LEVEL_TAG_ITEM_ID:
            return "Nattrak Oceanic Clearance Level";
        case CLEARANCE_MACH_TAG_ITEM_ID:
            return "Nattrak Oceanic Clearance Mach Number";
        case CLEARANCE_ENTRY_POINT_TAG_ITEM_ID:
            return "Nattrak Oceanic Clearance Entry Point";
        case CLEARANCE_TRACK_TAG_ITEM_ID:
            return "Nattrak Oceanic Clearance Track Identifier";
        case CLEARANCE_ENTRY_ESTIMATE_TAG_ITEM_ID:
            return "Nattrak Oceanic Clearance Entry Estimate";
        }

        return "";
    }

    void OceanicEventHandler::TagFunction(
        Euroscope::EuroScopeCFlightPlanInterface& flightplan,
        Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
        const std::string& context,
        const POINT& mousePos)
    {
        SetCurrentlySelectedClearance(flightplan);
        this->dialogManager.OpenDialog(
            IDD_OCEANIC_CLEARANCE,
            reinterpret_cast<LPARAM>(&this->currentlySelectedClearance) // NOLINT
        );
    }

    void OceanicEventHandler::SetTagItemData(Tag::TagData& tagData)
    {
        auto lock = std::lock_guard(this->clearanceMapMutex);
        auto clearance = this->clearances.find(tagData.GetFlightplan().GetCallsign());

        if (clearance == this->clearances.cend()) {
            return;
        }

        switch (tagData.GetItemCode()) {
        case CLEARANCE_INDICATOR_TAG_ITEM_ID:
            this->SetClearanceIndicatorTagItem(tagData, clearance->second);
            break;
        case CLEARANCE_LEVEL_TAG_ITEM_ID:
            this->SetClearedLevelTagItem(tagData, clearance->second);
            break;
        case CLEARANCE_MACH_TAG_ITEM_ID:
            SetClearedMachNumberTagItem(tagData, clearance->second);
            break;
        case CLEARANCE_ENTRY_POINT_TAG_ITEM_ID:
            SetClearedEntryPointTagItem(tagData, clearance->second);
            break;
        case CLEARANCE_TRACK_TAG_ITEM_ID:
            SetClearedTrackIdentifierTagItem(tagData, clearance->second);
            break;
        case CLEARANCE_ENTRY_ESTIMATE_TAG_ITEM_ID:
            SetClearedEntryTimeTagItem(tagData, clearance->second);
            break;
        }
    }

    auto OceanicEventHandler::ConvertNattrakLevelToEuroscope(const std::string& level) -> int
    {
        return std::stoi(level) * NATTRAK_EUROSCOPE_FLIGHT_LEVEL_CONVERSION_FACTOR;
    }

    auto OceanicEventHandler::NattrakLevelValid(std::string level) -> bool
    {
        return Datablock::NormaliseFlightLevelFromString(std::move(level)) != -1;
    }

    auto OceanicEventHandler::GetClearedTagItemColour(int clearedLevel, int currentLevel) -> COLORREF
    {
        return clearedLevel == currentLevel ? clearanceIndicatorOk : clearanceIndicatorActionRequired;
    }

    void OceanicEventHandler::SetClearanceIndicatorTagItem(Tag::TagData& tagData, const Clearance& clearance) const
    {
        // Work out the colour of the item based on whether the controller needs to do something
        const auto& flightplan = tagData.GetFlightplan();
        COLORREF tagItemColour = 0;
        if (clearance.status == clearance.CLEARANCE_STATUS_CLEARED) {
            int clearedAltitude = flightplan.GetClearedAltitude();
            int cruisingLevel = flightplan.GetCruiseLevel();
            int nattrakLevel = this->ConvertNattrakLevelToEuroscope(clearance.flightLevel);

            tagItemColour = clearedAltitude != 0 ? this->GetClearedTagItemColour(nattrakLevel, clearedAltitude)
                                                 : this->GetClearedTagItemColour(nattrakLevel, cruisingLevel);
        } else {
            tagItemColour = RGB(255, 153, 255);
        }

        // Set the item
        tagData.SetItemString("OCA");
        tagData.SetTagColour(tagItemColour);
    }

    void OceanicEventHandler::SetClearedLevelTagItem(Tag::TagData& tagData, const Clearance& clearance) const
    {
        const auto& flightplan = tagData.GetFlightplan();
        int clearedAltitude = flightplan.GetClearedAltitude();
        int cruisingLevel = flightplan.GetCruiseLevel();
        int nattrakLevel = this->ConvertNattrakLevelToEuroscope(clearance.flightLevel);
        tagData.SetTagColour(
            clearedAltitude != 0 ? this->GetClearedTagItemColour(nattrakLevel, clearedAltitude)
                                 : this->GetClearedTagItemColour(nattrakLevel, cruisingLevel));
        tagData.SetItemString(clearance.flightLevel);
    }

    void OceanicEventHandler::SetClearedMachNumberTagItem(Tag::TagData& tagData, const Clearance& clearance)
    {
        tagData.SetItemString(clearance.mach);
    }

    void OceanicEventHandler::SetClearedEntryPointTagItem(Tag::TagData& tagData, const Clearance& clearance)
    {
        tagData.SetItemString(clearance.entryFix);
    }

    void OceanicEventHandler::SetClearedTrackIdentifierTagItem(Tag::TagData& tagData, const Clearance& clearance)
    {
        tagData.SetItemString(clearance.track);
    }

    void OceanicEventHandler::SetClearedEntryTimeTagItem(Tag::TagData& tagData, const Clearance& clearance)
    {
        tagData.SetItemString(clearance.entryTime);
    }

    auto OceanicEventHandler::GetInvalidClearance() const -> const Clearance&
    {
        return this->invalidClearance;
    }

    bool OceanicEventHandler::ShouldFetchClxNow_(const std::string& callsign)
    {
        using clock = std::chrono::steady_clock;
        const auto now = clock::now();

        auto it = lastAssumeClxFetch_.find(callsign);

        if (it != lastAssumeClxFetch_.end() && (now - it->second) < ASSUME_CLX_DEBOUNCE) {
            return false;
        }

        lastAssumeClxFetch_[callsign] = now;
        return true;
    }

    auto OceanicEventHandler::GetDefaultClearanceForCallsign(Euroscope::EuroScopeCFlightPlanInterface& flightplan)
        -> Clearance
    {
        return Clearance{flightplan.GetCallsign()};
    }

    void OceanicEventHandler::SetCurrentlySelectedClearance(Euroscope::EuroScopeCFlightPlanInterface& flightplan)
    {
        auto lock = std::lock_guard(this->clearanceMapMutex);
        auto storedClearance = this->clearances.find(flightplan.GetCallsign());
        this->currentlySelectedClearance = storedClearance != this->clearances.cend()
                                               ? storedClearance->second
                                               : GetDefaultClearanceForCallsign(flightplan);
    }
} // namespace UKControllerPlugin::Oceanic
