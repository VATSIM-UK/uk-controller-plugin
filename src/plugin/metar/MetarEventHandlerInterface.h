#pragma once

namespace UKControllerPlugin {
    namespace Metar {

        /*
            An interface to be implemented by classes that need to react to a new METAR coming in.
        */
        class MetarEventHandlerInterface
        {
            public:
                virtual void NewMetar(std::string station, std::string metar) = 0;
        };
    }  // namespace Metar
}  // namespace UKControllerPlugin
