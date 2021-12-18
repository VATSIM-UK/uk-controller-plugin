#pragma once

namespace UKControllerPlugin::Metar {
    class ParsedMetar;

    /**
     * Stores parsed METAR information
     */
    class ParsedMetarCollection
    {
        public:
        [[nodiscard]] auto Count() const -> size_t;
        [[nodiscard]] auto GetForAirfield(const std::string& airfield) const -> std::shared_ptr<ParsedMetar>;
        void UpdateMetar(const std::shared_ptr<ParsedMetar>& metar);

        private:
        std::map<std::string, std::shared_ptr<ParsedMetar>> metars;
        
        // Locks the container
        mutable std::mutex containerLock;
    };
} // namespace UKControllerPlugin::Metar
