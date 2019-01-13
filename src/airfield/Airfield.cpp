#include "pch/stdafx.h"
#include "airfield/Airfield.h"

namespace UKControllerPlugin {
    namespace Airfield {

        Airfield::Airfield(std::string icao, std::vector<std::string> ownership)
        {
            if (icao.size() > 4 || icao.substr(0, 2).compare("EG") != 0) {
                throw std::invalid_argument("Invalid UK code.");
            }

            this->icao = icao;
            this->ownershipPresedence = ownership;
        }

        std::string Airfield::GetIcao(void) const
        {
            return this->icao;
        }

        /*
            Returns the ownership presedence for the airfield.
        */
        std::vector<std::string> Airfield::GetOwnershipPresedence(void) const
        {
            return this->ownershipPresedence;
        }

        /*
            Returns true if the two airfields are the same.
        */
        bool Airfield::operator==(const Airfield & compare)
        {
            return this->icao == compare.icao;
        }
    }  // namespace Airfield
}  // namespace UKControllerPlugin
