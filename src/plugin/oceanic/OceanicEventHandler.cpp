#include "OceanicEventHandler.h"
#include "curl/CurlInterface.h"
#include "curl/CurlRequest.h"
#include "datablock/DatablockFunctions.h"
#include "dialog/DialogManager.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "tag/TagData.h"
#include "task/TaskRunnerInterface.h"
#include <algorithm>
#include <cctype>

namespace UKControllerPlugin::Oceanic {

    OceanicEventHandler::OceanicEventHandler(
        Curl::CurlInterface& curl, TaskManager::TaskRunnerInterface& taskRunner, Dialog::DialogManager& dialogManager)
        Manager(dialogManager)
    {
    }

    // ============================================================================
    // One-shot CLX fetch for a specific callsign (async)
    // ============================================================================
    void OceanicEventHandler::RefreshClxForCallsignAsync_(const std::string& callsign)
    {
        this->taskRunner.QueueAsynchronousTask(
            this, callsign {
                try {
                    Curl::CurlRequest req(nattrakClxUrl, Curl::CurlRequest::METHOD_GET);
                    Curl::CurlResponse res = this->curl.MakeCurlRequest(req);

                    if (res.IsCurlError() || !res.StatusOk()) {
                        LogWarning("CLX fetch failed for " + callsign);
                        return;
                    }

                    nlohmann::json clxArray;
                    try {
                        clxArray = nlohmann::json::parse(res.GetResponse());
                    } catch (...) {
                        LogWarning("CLX JSON parse failed");
                        return;
                    }

                    if (!clxArray.is_array())
                        return;

                    // Uppercase helper
                    auto upper = std::string s
                    {
                        std::transform(
                            s.begin(), s.end(), s.begin(), unsigned char c {
                                return static_cast<char>(std::toupper(c));
                            });
                        return s;
                    };

                    const std::string target = upper(callsign);

                    for (const auto& clx : clxArray) {
                        if (!clx.is_object())
                            continue;
                        if (!clx.contains("callsign") || !clx.at("callsign").is_string())
                            continue;

                        std::string cs = upper(clx.at("callsign").get<std::string>());
                        if (cs != target)
                            continue;

                        auto built = BuildClearanceFromClx_(clx);
                        if (!built.has_value())
                            return;

                        {
                            auto lock = std::lock_guard(this->clearanceMapMutex);
                            this->clearances.erase(callsign);
                            this->clearances.emplace(callsign, *built);
                        }

                        return;
                    }
                } catch (const std::exception& e) {
                    LogWarning(std::string("Exception during CLX refresh: ") + e.what());
                }
            });
    }

    void OceanicEventHandler::TimedEventTrigger()
    {
        this->taskRunner.QueueAsynchronousTask(this {
            LogInfo("Updating oceanic clearance data from Nattrak");
            Curl::CurlRequest apiUpdateRequest(nattrakUrl, Curl::CurlRequest::METHOD_GET);
            Curl::CurlResponse apiUpdateResponse = this->curl.MakeCurlRequest(apiUpdateRequest);

            if (apiUpdateResponse.IsCurlError() || !apiUpdateResponse.StatusOk()) {
                LogWarning("Unable to retrieve oceanic clearattrak.");
                return;
            }

            nlohmann::json clearanceData;
            try {
                clearanceData = nlohmann::json::parse(apiUpdateResponse.GetResponse());
            } catch (nlohmann::json::exception& exception) {
                LogWarning("Unable to decode oceanic clearances from Nattrak, JSON parse failed.");
                return;
            }

::string, Clearance>(
                        clearance.at("callsign").get<std::string>(),
                        {
                            clearance.at("callsign").get<std::string>(),
                            clearance.at("status").get<std::string>(),
                           string>())),
                            clearance.at("mach").get<std::string>(),
                            clearance.at("estimating_time").get<std::string>(),
                            clearance.at("clearance_issued").is_null()
                                ? ""
                                : clearance.at("clearance_issued").get").get<std::string>(),
                        }));
            }
            LogInfo("Finished updating oceanic clearance data");
    });
}

void OceanicEventHandler::FlightPlanEvent(
    Euroscope::EuroScopeCFlightPlanInterface& flightPlan, Euroscope::EuroScopeCRadarTargetInterface& radarTarget)
{
    {
        auto lock = std::lock_guard(this->clearanceMapMutex);
        this->clearances.erase(flightPlan.GetCallsign());
    }

    const std::string cs = flightPlan.GetCallsign();

    if (ShouldFetchClxNow_(cs)) {
        RefreshClxForCallsignAsync_(cs);
        this->TimedEventTrigger();
    }
}

void OceanicEventHandler::FlightPlanDisconnectEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan)
{
    auto lock = std::lock_guard(this->clearanceMapMutex);
    this->clearances.erase(flightPlan.GetCallsign());
}

void OceanicEventHandler::ControllerFlightPlanDataEvent(
    Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType)
{
}

auto OceanicEventHandler::NattrakClearanceValid(const nlohmann::json& clearance) -> bool
{
        return clearance_string()) && clearance.contains("fix") &&
               clearance.at("fix").is_string() && clearance.contains("level") && clearance.at("level").is_string() &&
               NattrakLevelValid(clearance.at("level").get<std::string>()) && clearance.contains("mach") &&
               clearance.at("mach").is_string() && clearance.contains("estimating_time") &&
               clearance.at("estimating_time").is_string() && clearance.contains("clearance_issued") &&
               (clearance.at("clearance_issued").is_string() || clearance.at("clearance_issued").is_null()) &&
                      auto lock = std::lock_guard(this->clearanceMapMutex);
        auto clearance = this->clearances.find(callsign);
        return clearance == this->clearances.cend() ? this->invalidClearance : clearance->second;
}

auto OceanicEventHandler::GetTagItemDescription(int tagItemId) const -> std::string Nattrak Oceanic Clearance Level ";
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

void OceanicEventHandler::SetTagItemData(Tag::TagData& tagData)
{
    auto lock = std::lock_guard(this->clearanceMapMutex);
    auto clearance = this->clearances.find(tagData.GetFlightplan().GetCallsign());

    if (clearance == this->clearances.cend()) {
        return;
    }

);
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

   )) != -1;
   }

   auto OceanicEventHandler::GetClearedTagItemColour(int clearedLevel, int currentLevel) -> COLORREF
   {
       return clearedLevel == currentLevel ? clearanceIndicatorOk : clearanceIndicatorActionRequired;
   }

   void OceanicEventHandler::SetClearedIndicatorTagItem(Tag::TagData& tagData, const Clearance& clearance) const
   {
       const auto& flightplan = tagData.GetFlightplan();
       COLORREF tagItemColour = 0;

       if (clearance.status == clearance.CLEARANCE_STATUS_CLEARED) {
           153, 255);
       }

       tagData.SetItemString("OCA");
       tagData.SetTagColour(tagItemColour);
   }

   auto OceanicEventHandler::GetInvalidClearance() const
       -> const Clearance& if (it != lastAssumeClxFetch_.end() && (now - it->second) < ASSUME_CLX_DEBOUNCE)
   {
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