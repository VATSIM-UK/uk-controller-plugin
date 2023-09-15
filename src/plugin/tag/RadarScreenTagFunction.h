#pragma once

namespace UKControllerPlugin::Euroscope {
    class EuroScopeCFlightPlanInterface;
    class EuroScopeCRadarTargetInterface;
    class EuroscopeRadarLoopbackInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Tag {

    /*
        Represents a function that EuroScope will call through its
        OnFunctionCall interface, but specific to radar screen instances.
    */
    typedef struct RadarScreenTagFunction
    {

        RadarScreenTagFunction();
        RadarScreenTagFunction(
            int functionId,
            std::string description,
            std::function<void(
                Euroscope::EuroscopeRadarLoopbackInterface&,
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface&,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface&,
                std::string context,
                const POINT& mousePos,
                const RECT& area)> function);
        RadarScreenTagFunction(const RadarScreenTagFunction& copy);
        void operator=(const RadarScreenTagFunction& copy);
        bool operator==(const RadarScreenTagFunction& compare) const;

        int functionId;
        std::string description;
        std::function<void(
            Euroscope::EuroscopeRadarLoopbackInterface&,
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface&,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface&,
            std::string,
            const POINT&,
            const RECT&)>
            function;
    } RadarScreenTagFunction;
} // namespace UKControllerPlugin::Tag
