#pragma once

namespace UKControllerPlugin::IntentionCode
{
    class FirExitPoint;

    /*
        Represents the point(s) at which an aircraft leaves the FIR(s).
    */
    class AircraftFirExit
    {
        // The callsign to which the exit belongs
        std::string callsign = "";

        // The point at which an internal exit between EGTT and EGPX occurs (if at all)
        std::shared_ptr<FirExitPoint> internalExitPoint = nullptr;

        // The point at which the aircraft will leave the UK FIRs
        std::shared_ptr<FirExitPoint> ukExitPoint = nullptr;
    }
} // namespace UKControllerPlugin::IntentionCode