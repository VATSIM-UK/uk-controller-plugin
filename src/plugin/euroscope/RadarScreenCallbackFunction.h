#pragma once

namespace UKControllerPlugin::Euroscope {

    class EuroscopeRadarLoopbackInterface;

    /*
        Represents a function that EuroScope will call through its
        OnFunctionCall interface at a radar screen level.
    */
    typedef struct RadarScreenCallbackFunction
    {
        RadarScreenCallbackFunction() = default;
        RadarScreenCallbackFunction(
            int functionId,
            std::string description,
            std::function<void(int, EuroscopeRadarLoopbackInterface&, const std::string&, const POINT&, const RECT&)>
                function);
        void operator=(const RadarScreenCallbackFunction& copy);
        bool operator==(const RadarScreenCallbackFunction& compare) const;

        int functionId;
        std::string description;
        std::function<void(int, EuroscopeRadarLoopbackInterface&, const std::string&, const POINT&, const RECT&)>
            function;
    } RadarScreenCallbackFunction;
} // namespace UKControllerPlugin::Euroscope
