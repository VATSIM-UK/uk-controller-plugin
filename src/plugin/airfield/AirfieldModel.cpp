#include "pch/stdafx.h"
#include "airfield/AirfieldModel.h"

namespace UKControllerPlugin {
    namespace Airfield {

        AirfieldModel::AirfieldModel(std::string icao, std::vector<std::string> ownership)
        {
            if (icao.size() > 4 || icao.substr(0, 2).compare("EG") != 0) {
                throw std::invalid_argument("Invalid UK code.");
            }

            this->icao = icao;
            this->ownershipPresedence = ownership;
        }

        std::string AirfieldModel::GetIcao(void) const
        {
            return this->icao;
        }

        /*
            Returns the ownership presedence for the airfield.
        */
        std::vector<std::string> AirfieldModel::GetOwnershipPresedence(void) const
        {
            return this->ownershipPresedence;
        }

        /*
            Returns true if the two airfields are the same.
        */
        bool AirfieldModel::operator==(const AirfieldModel& compare)
        {
            return this->icao == compare.icao;
        }
    }  // namespace Airfield
}  // namespace UKControllerPlugin
