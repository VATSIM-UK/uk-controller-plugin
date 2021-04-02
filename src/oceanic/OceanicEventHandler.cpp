#include "pch/stdafx.h"
#include "oceanic/OceanicEventHandler.h"
#include "curl/CurlInterface.h"
#include "curl/CurlRequest.h"
#include "curl/CurlResponse.h"
#include "dialog/DialogManager.h"
#include "task/TaskRunnerInterface.h"

namespace UKControllerPlugin {
    namespace Oceanic {

        const std::string nattrakUrl = "https://nattrak.vatsim.net/pluginapi.php";
        const std::regex nattrakLevelRegex("^\\d{2,3}$");

        OceanicEventHandler::OceanicEventHandler(
            Curl::CurlInterface& curl,
            TaskManager::TaskRunnerInterface& taskRunner,
            Dialog::DialogManager& dialogManager
        ): curl(curl), taskRunner(taskRunner), dialogManager(dialogManager) {}

        void OceanicEventHandler::TimedEventTrigger()
        {
            this->taskRunner.QueueAsynchronousTask([this]()
            {
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
                } catch (nlohmann::json::exception exception) {
                    LogWarning("Unable to decode oceanic clearances from Nattrak, JSON parse failed.");
                    return;
                }

                if (!clearanceData.is_array()) {
                    LogWarning("Unable to decode oceanic clearances from Nattrak, JSON is not array.");
                    return;
                }

                // Loop the clearances and update local data
                this->clearances.clear();
                for (
                    auto clearance = clearanceData.cbegin();
                    clearance != clearanceData.cend();
                    ++clearance
                ) {
                    if (!NattrakClearanceValid(*clearance)) {
                        LogWarning("Invalid clearance received from Nattrak");
                        continue;
                    }

                    clearances.insert(
                        std::pair<std::string, Clearance>(
                            clearance->at("callsign").get<std::string>(),
                            {
                                clearance->at("callsign").get<std::string>(),
                                clearance->at("status").get<std::string>(),
                                clearance->at("nat").get<std::string>(),
                                clearance->at("fix").get<std::string>(),
                                clearance->at("level").get<std::string>(),
                                clearance->at("mach").get<std::string>(),
                                clearance->at("estimating_time").get<std::string>(),
                                clearance->at("clearance_issued").is_null()
                                    ? ""
                                    : clearance->at("clearance_issued").get<std::string>(),
                                clearance->at("extra_info").is_null()
                                    ? ""
                                    : clearance->at("extra_info").get<std::string>(),
                            }
                        )
                    );
                }
                LogInfo("Finished updating oceanic clearance data");
            });
        }

        bool OceanicEventHandler::NattrakClearanceValid(const nlohmann::json& clearance) const
        {
            return clearance.is_object() &&
                clearance.contains("callsign") &&
                clearance.at("callsign").is_string() &&
                clearance.contains("status") &&
                clearance.at("status").is_string() &&
                clearance.contains("nat") &&
                clearance.at("nat").is_string() &&
                clearance.contains("fix") &&
                clearance.at("fix").is_string() &&
                clearance.contains("level") &&
                clearance.at("level").is_string() &&
                this->NattrakLevelValid(clearance.at("level").get<std::string>()) &&
                clearance.contains("mach") &&
                clearance.at("mach").is_string() &&
                clearance.contains("estimating_time") &&
                clearance.at("estimating_time").is_string() &&
                clearance.contains("clearance_issued") &&
                (clearance.at("clearance_issued").is_string() || clearance.at("clearance_issued").is_null()) &&
                clearance.contains("extra_info") &&
                (clearance.at("extra_info").is_string() || clearance.at("extra_info").is_null());
        }

        size_t OceanicEventHandler::CountClearances() const
        {
            return this->clearances.size();
        }

        const Clearance& OceanicEventHandler::GetClearanceForCallsign(std::string callsign) const
        {
            auto clearance = this->clearances.find(callsign);
            return clearance == this->clearances.cend() ? this->invalidClearance : clearance->second;
        }

        std::string OceanicEventHandler::GetTagItemDescription(int tagItemId) const
        {
            return "Nattrak Oceanic Clearance Indicator";
        }

        void OceanicEventHandler::TagFunction(
            Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
            std::string context,
            const POINT& mousePos
        )
        {
            auto storedClearance = this->clearances.find(flightplan.GetCallsign());
            this->currentlySelectedClearance = storedClearance != this->clearances.cend()
                                                   ? storedClearance->second
                                                   : Clearance{flightplan.GetCallsign()};

            this->dialogManager.OpenDialog(
                IDD_OCEANIC_CLEARANCE,
                reinterpret_cast<LPARAM>(&this->currentlySelectedClearance)
            );
        }

        void OceanicEventHandler::SetTagItemData(Tag::TagData& tagData)
        {
            auto clearance = this->clearances.find(tagData.flightPlan.GetCallsign());

            if (clearance == this->clearances.cend()) {
                return;
            }

            // Work out the colour of the item based on whether the controller needs to do something
            COLORREF tagItemColour;
            if (clearance->second.status == clearance->second.CLEARANCE_STATUS_CLEARED) {
                int clearedAltitude = tagData.flightPlan.GetClearedAltitude();
                int cruisingLevel = tagData.flightPlan.GetCruiseLevel();
                int nattrakLevel = this->ConvertNattrakLevelToEuroscope(clearance->second.flightLevel);

                tagItemColour = clearedAltitude != 0
                                    ? this->GetClearedTagItemColour(nattrakLevel, clearedAltitude)
                                    : this->GetClearedTagItemColour(nattrakLevel, cruisingLevel);
            } else {
                tagItemColour = RGB(255, 153, 255);
            }

            // Set the item
            tagData.SetItemString("OCA");
            tagData.SetTagColour(tagItemColour);
        }

        int OceanicEventHandler::ConvertNattrakLevelToEuroscope(std::string level) const
        {
            return std::stoi(level) * 100;
        }

        bool OceanicEventHandler::NattrakLevelValid(std::string level) const
        {
            return std::regex_match(level, nattrakLevelRegex);
        }

        COLORREF OceanicEventHandler::GetClearedTagItemColour(int clearedLevel, int currentLevel) const
        {
            return clearedLevel == currentLevel
                       ? this->clearanceIndicatorOk
                       : this->clearanceIndicatorActionRequired;
        }
    }  // namespace Oceanic
}  // namespace UKControllerPlugin
