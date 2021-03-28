#include "pch/stdafx.h"
#include "oceanic/OceanicEventHandler.h"
#include "curl/CurlInterface.h"
#include "curl/CurlRequest.h"
#include "curl/CurlResponse.h"
#include "task/TaskRunnerInterface.h"

namespace UKControllerPlugin {
    namespace Oceanic {

        const std::string nattrakUrl = "https://nattrak.vatsim.net/pluginapi.php";

        OceanicEventHandler::OceanicEventHandler(
            Curl::CurlInterface& curl,
            TaskManager::TaskRunnerInterface& taskRunner
        ): curl(curl), taskRunner(taskRunner) {}

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
    }  // namespace Oceanic
}  // namespace UKControllerPlugin
