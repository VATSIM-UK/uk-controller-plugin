#pragma once
#include "pch/stdafx.h"
#include "stands/StandSerializer.h"

namespace UKControllerPlugin {
    namespace Stands {

        /*
            Create Stand data from JSON
        */
        void from_json(
            const nlohmann::json& json,
            std::set<Stand, CompareStands>& stands
        ) {
            if (!DependencyValid(json)) {
                LogInfo("Stand data is not valid");
                return;
            }

            int numberOfAirfields = 0;
            int numberOfStands = 0;
            for (auto airfieldIt = json.cbegin(); airfieldIt != json.cend(); ++airfieldIt) {
                numberOfAirfields++;
                for (auto standIt = airfieldIt->cbegin(); standIt != airfieldIt->cend(); ++standIt) {
                    numberOfStands++;
                    stands.insert({
                        standIt->at("id").get<int>(),
                        airfieldIt.key(),
                        standIt->at("identifier").get<std::string>()
                    });
                }
            }

            LogInfo(
                "Loaded " + std::to_string(numberOfStands) + " stands across " +
                std::to_string(numberOfAirfields) + " airfields"
            );
        }

        /*
            Checks that the entire stand dependency is valid
        */
        bool DependencyValid(const nlohmann::json& data)
        {
            if (!data.is_object()) {
                return false;
            }

            for (auto it = data.cbegin(); it != data.cend(); ++it) {
                if (!AirportDataValid(*it)) {
                    return false;
                }
            }

            return true;
        }

        /*
            Checks that each airport has a valid list of stands
        */
        bool AirportDataValid(const nlohmann::json& data)
        {
            if (!data.is_array()) {
                return false;
            }

            for (auto it = data.cbegin(); it != data.cend(); ++it) {
                if (!StandDataValid(*it)) {
                    return false;
                }
            }

            return true;
        }

        /*
            Checks that each stand is valid
        */
        bool StandDataValid(const nlohmann::json& data)
        {
            return data.is_object() &&
                data.contains("id") &&
                data.at("id").is_number_integer() &&
                data.contains("identifier") &&
                data.at("identifier").is_string();
        }
    }  // namespace Stands
}  // namespace UKControllerPlugin
