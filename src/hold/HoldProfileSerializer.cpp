#pragma once
#include "pch/stdafx.h"
#include "hold/HoldProfileSerializer.h"
#include "hold/HoldProfile.h"

namespace UKControllerPlugin {
    namespace Hold {

        const HoldProfile holdProfileSerializerInvalid = { 0, "INVALID" };

        /*
            Convert HoldProfile to JSON
        */
        void to_json(nlohmann::json & json, const HoldProfile & holdProfile) {
            json = nlohmann::json { 
                {"id", holdProfile.id},
                {"name", holdProfile.name},
                {"holds", holdProfile.holds}
            };
        }

        /*
            Create HoldProfile from JSON
        */
        void from_json(const nlohmann::json & json, HoldProfile & holdProfile) {
            if (!ProfileJsonValid(json)) {
                holdProfile = holdProfileSerializerInvalid;
                return;
            }

            json.at("id").get_to(holdProfile.id);
            json.at("name").get_to(holdProfile.name);
            json.at("holds").get_to(holdProfile.holds);
        }

        /*
            Return whether hold ids are valid
        */
        bool HoldIdsValid(const nlohmann::json & data)
        {
            bool valid = true;

            for (nlohmann::json::const_iterator it = data.cbegin(); it != data.cend(); ++it) {
                valid = valid && it->is_number_integer() && it.value() > 0;
            }

            return valid;
        }

        /*
            Returns true if the holding data is valid
        */
        bool ProfileJsonValid(const nlohmann::json & data)
        {
            return data.is_object() &&
                data.find("id") != data.end() &&
                data.at("id").is_number_integer() &&
                data.find("name") != data.end() &&
                data.at("name").is_string() &&
                data.find("holds") != data.end() &&
                data.at("holds").is_array() &&
                HoldIdsValid(data.at("holds"));
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
