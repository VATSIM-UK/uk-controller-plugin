#include "pch/pch.h"
#include "controller/ControllerPositionParser.h"
#include "helper/HelperFunctions.h"

namespace UKControllerPlugin {
    namespace Controller {

        ControllerPositionParser::ControllerPositionParser()
        {
        }

        /*
            Normalises a callsign, by replacing hypens with underscores.
        */
        std::string ControllerPositionParser::NormaliseCallsign(std::string callsign) const
        {
            std::replace(callsign.begin(), callsign.end(), '-', '_');
            return callsign;
        }

        /*
            Returns the facility (airfield or LON) from the callsign.
        */
        std::string ControllerPositionParser::ParseFacilityFromCallsign(std::string callsign) const
        {
            std::string normalise = this->NormaliseCallsign(callsign);
            return normalise.substr(0, normalise.find('_'));
        }

        std::string ControllerPositionParser::ParseTypeFromCallsign(std::string callsign) const
        {
            std::string normalise = this->NormaliseCallsign(callsign);
            return normalise.substr(normalise.find_last_of('_') + 1);
        }

        /*
            Returns whether the position is a mentors, trainees or non-mentoring
            position.
        */
        int ControllerPositionParser::IsMentoringPosition(std::string callsign) const
        {
            std::string normalise = this->NormaliseCallsign(callsign);
            std::vector<std::string> tokens = HelperFunctions::TokeniseString('_', normalise);

            // If it's the incorrect format, definitely not mentoring.
            if (tokens.size() != 3) {
                return this->positionNotMentoring;
            }

            // Look for the tale-tale signs of being a mentoring session.
            if (tokens[1].compare(this->mentorMidCharcter) == 0) {
                return this->positionMentor;
            }

            if (tokens[1].compare(this->traineeMidCharacter) == 0) {
                return this->positionTrainee;
            }

            return this->positionNotMentoring;
        }
    }  // namespace Controller
}  // namespace UKControllerPlugin
