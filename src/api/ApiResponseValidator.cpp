#include "pch/stdafx.h"
#include "api/ApiResponseValidator.h"

namespace UKControllerPlugin {
    namespace Api {

        /*
            Loops through all the data and checks that it matches the provided schema.
        */
        bool ApiResponseValidator::ValidateSchema(nlohmann::json data, std::map<std::string, int> schema)
        {
            // The data should start as a JSON object
            if (!data.is_object()) {
                return false;
            }

            // Check each field.
            for (std::map<std::string, int>::const_iterator it = schema.cbegin(); it != schema.cend(); ++it) {
                if (!ApiResponseValidator::CheckCorrectType(data, it->first, it->second)) {
                    return false;
                }
            }

            return true;
        }

        /*
            Checks that the given field is of the correct type.

            Returns true if so, false otherwise.
        */
        bool ApiResponseValidator::CheckCorrectType(nlohmann::json data, std::string field, int type)
        {

            // Check its type.
            switch (type) {
                case ApiResponseValidator::boolean:
                    return data[field].is_boolean();
                case ApiResponseValidator::integer:
                    return data[field].is_number_integer();
                case ApiResponseValidator::floatingpoint:
                    return data[field].is_number_float();
                case ApiResponseValidator::string:
                    return data[field].is_string();
                case ApiResponseValidator::object:
                    return data[field].is_object();
                default:
                    return false;
            }
        }
    }  // namespace Api
}  // namespace UKControllerPlugin
