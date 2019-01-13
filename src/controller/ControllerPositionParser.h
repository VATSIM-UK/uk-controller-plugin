#pragma once

namespace UKControllerPlugin {
    namespace Controller {

        class ControllerPositionParser
        {
            public:
                ControllerPositionParser();
                std::string NormaliseCallsign(std::string callsign) const;
                std::string ParseFacilityFromCallsign(std::string callsign) const;
                int IsMentoringPosition(std::string callsign) const;

                // Represents a position that is not affiliated with training
                const int positionNotMentoring = 0;

                // Represents a position that is a mentor
                const int positionMentor = 1;

                // Represents a position that is a trainee
                const int positionTrainee = 2;

            private:

                // The number of tokens we expect from a mentoring position when tokenised.
                const size_t expectedMentoringTokens = 3;

                // The middle character of a mentor callsign.
                const std::string mentorMidCharcter = "M";

                // The middle core of a trainee callsign.
                const std::string traineeMidCharacter = "T";
        };
    }  // namespace Controller
}  // namespace UKControllerPlugin
