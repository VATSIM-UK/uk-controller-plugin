#pragma once
#include "airfield/AirfieldModel.h"

namespace UKControllerPlugin {
    namespace Airfield {

        /*
            A class representing a collection of airfields. Allows airfields to be
            retrieved by their ICAO code.
        */
        class AirfieldCollection
        {
            public:
                void AddAirfield(std::unique_ptr<UKControllerPlugin::Airfield::AirfieldModel> airfield);
                const AirfieldModel & FetchAirfieldByIcao(std::string icao) const;
                size_t GetSize(void) const;
            private:
                bool IsHomeAirfield(std::string icao) const;

                // A map of ICAO code to airfield.
                std::map<std::string, std::unique_ptr<UKControllerPlugin::Airfield::AirfieldModel>> airfieldMap;
        };
    }  // namespace Airfield
}  // namespace UKControllerPlugin
