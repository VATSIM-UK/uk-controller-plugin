#pragma once

// Forward declarations
namespace UKControllerPlugin {
    namespace Metar {
        class MetarEventHandlerInterface;
    }  // namespace Metar
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Metar {

        /*
            A class that stores pointers to classes that want to know
            when a new METAR comes in.
        */
        class MetarEventHandlerCollection
        {
            public:
                int CountHandlers(void) const;
                void NewMetarEvent(std::string station, std::string metar) const;
                void RegisterHandler(std::shared_ptr<UKControllerPlugin::Metar::MetarEventHandlerInterface> handler);

            private:
                std::set<std::shared_ptr<UKControllerPlugin::Metar::MetarEventHandlerInterface>> handlers;
        };
    }  // namespace Metar
}  // namespace UKControllerPlugin
