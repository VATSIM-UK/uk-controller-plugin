#pragma once

namespace UKControllerPlugin::Selcal {
    /**
     * Represents a SELCAL that has been parsed and generated
     * from a flightplan or similar
     */
    class ParsedSelcal
    {
        public:
        ParsedSelcal(std::string code);
        [[nodiscard]] auto GetRaw() const -> const std::string&;
        [[nodiscard]] auto GetWithSeparator() const -> std::string;

        private:
        std::string code;
    };
} // namespace UKControllerPlugin::Selcal
