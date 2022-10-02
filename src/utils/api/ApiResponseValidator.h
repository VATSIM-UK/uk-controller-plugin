#pragma once

namespace UKControllerPlugin {
    namespace Api {

        /*
            Validates responses from the API.
        */
        class ApiResponseValidator
        {
            public:
            static bool ValidateSchema(nlohmann::json data, std::map<std::string, int> schema);

            // Representing an boolean datatype
            static const int boolean = 0;

            // Representing an integer datatype
            static const int integer = 1;

            // Representing an float datatype
            static const int floatingpoint = 2;

            // Representing an string datatype
            static const int string = 3;

            // Representing an object datatype
            static const int object = 4;

            private:
            static bool CheckCorrectType(nlohmann::json data, std::string field, int type);
        };
    } // namespace Api
} // namespace UKControllerPlugin
