#include "pch/stdafx.h"
#include "sid/SidCollectionFactory.h"
#include "sid/SidCollection.h"
#include "sid/StandardInstrumentDeparture.h"

namespace UKControllerPlugin {
    namespace Sid {

        std::unique_ptr<SidCollection> MakeSidCollection(const nlohmann::json& sidData)
        {
            std::unique_ptr<SidCollection> sids = std::make_unique<SidCollection>();

            if (!SidDataValid(sidData)) {
                LogWarning("Sids data is not valid");
                return std::move(sids);
            }

            for (auto sid : sidData) {
                sids->AddSid(
                    std::make_shared<StandardInstrumentDeparture>(
                        sid.at("airfield").get<std::string>(),
                        sid.at("identifier").get<std::string>(),
                        sid.at("initial_altitude").get<int>(),
                        sid.at("initial_heading").is_number_integer()
                            ? sid.at("initial_heading").get<int>()
                            : 0
                    )
                );
            }

            LogInfo("Loaded " + std::to_string(sids->CountSids()) + " standard instrument departures");
            return std::move(sids);
        }

        bool SidDataValid(const nlohmann::json& sidData)
        {
            return sidData.is_array() &&
                std::find_if_not(
                    sidData.cbegin(),
                    sidData.cend(),
                    [](const nlohmann::json& sid) -> bool
                    {
                        return sid.is_object() &&
                            sid.contains("airfield") &&
                            sid.at("airfield").is_string() &&
                            sid.contains("identifier") &&
                            sid.at("identifier").is_string() &&
                            sid.contains("initial_altitude") &&
                            sid.at("initial_altitude").is_number_integer() &&
                            sid.contains("initial_heading") &&
                            InitialHeadingValid(sid);
                    }
                ) == sidData.cend();
        }

        bool InitialHeadingValid(const nlohmann::json& sidData)
        {
            if (sidData.at("initial_heading").is_null()) {
                return true;
            }

            if (!sidData.at("initial_heading").is_number_integer()) {
                return false;
            }

            int initialHeading = sidData.at("initial_heading").get<int>();
            return initialHeading > 0 && initialHeading < 361;
        }
    }  // namespace Sid
}  // namespace UKControllerPlugin
