#pragma once

namespace UKControllerPlugin::AircraftState {

    struct AircraftState
    {
        bool clearanceFlag = false;
        std::string groundState;

        auto operator==(const AircraftState& compare) const -> bool
        {
            return this->clearanceFlag == compare.clearanceFlag && this->groundState == compare.groundState;
        }
    };

    /*
        @see https://www.euroscope.hu/wp/non-standard-extensions/
    */
    const std::set<std::string> GROUND_STATES = {"NSTS", "STUP", "PUSH", "TAXI", "DEPA", "ARR", "TXIN", "PARK"};
} // namespace UKControllerPlugin::AircraftState
