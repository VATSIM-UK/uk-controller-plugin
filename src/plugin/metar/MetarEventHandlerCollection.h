#pragma once

namespace UKControllerPlugin::Metar {
    class MetarEventHandlerInterface;
    class ParsedMetar;

    /*
        A class that stores pointers to classes that want to know
        when a new METAR comes in.
    */
    class MetarEventHandlerCollection
    {
        public:
        [[nodiscard]] auto CountHandlers() const -> size_t;
        void UpdatedMetarEvent(const ParsedMetar& metar) const;
        void RegisterHandler(std::shared_ptr<UKControllerPlugin::Metar::MetarEventHandlerInterface> handler);

        private:
        std::set<std::shared_ptr<UKControllerPlugin::Metar::MetarEventHandlerInterface>> handlers;
    };
} // namespace UKControllerPlugin::Metar
