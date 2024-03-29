#pragma once

namespace UKControllerPlugin::Metar {
    class ParsedMetar;

    /*
        An interface to be implemented by classes that need to react to a new METAR coming in.
    */
    class MetarEventHandlerInterface
    {
        public:
        MetarEventHandlerInterface();
        virtual ~MetarEventHandlerInterface();
        MetarEventHandlerInterface(const MetarEventHandlerInterface&);
        MetarEventHandlerInterface(MetarEventHandlerInterface&&) noexcept;
        [[nodiscard]] auto operator=(const MetarEventHandlerInterface&) -> MetarEventHandlerInterface&;
        [[nodiscard]] auto operator=(MetarEventHandlerInterface&&) noexcept -> MetarEventHandlerInterface&;
        virtual void MetarUpdated(const ParsedMetar& metar) = 0;
    };
} // namespace UKControllerPlugin::Metar
