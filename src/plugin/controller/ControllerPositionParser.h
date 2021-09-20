#pragma once

namespace UKControllerPlugin::Controller {

    class ControllerPositionParser
    {
        public:
        ControllerPositionParser();
        [[nodiscard]] auto NormaliseCallsign(std::string callsign) const -> std::string;
        [[nodiscard]] auto ParseFacilityFromCallsign(std::string callsign) const -> std::string;
        [[nodiscard]] auto ParseTypeFromCallsign(std::string callsign) const -> std::string;
        [[nodiscard]] auto IsMentoringPosition(std::string callsign) const -> int;

        // Represents a position that is not affiliated with training
        inline static const int positionNotMentoring = 0;

        // Represents a position that is a mentor
        inline static const int positionMentor = 1;

        // Represents a position that is a trainee
        inline static const int positionTrainee = 2;

        private:
        // The middle character of a mentor callsign.
        const std::string mentorMidCharcter = "M";

        // The middle character of a trainee callsign.
        const std::string traineeMidCharacter = "T";
    };
} // namespace UKControllerPlugin::Controller
