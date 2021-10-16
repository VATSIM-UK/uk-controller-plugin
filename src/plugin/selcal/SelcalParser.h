#pragma once

namespace UKControllerPlugin::Selcal {
    class ParsedSelcal;

    /**
     * Parses the SELCAL from a string such as the flightplan remarks.
     */
    class SelcalParser
    {
        public:
        SelcalParser();
        [[nodiscard]] auto ParseFromString(const std::string& string) const -> std::shared_ptr<ParsedSelcal>;

        private:
        [[nodiscard]] static auto DuplicateLetter(const std::string& code) -> bool;
        [[nodiscard]] static auto CharactersOutOfOrder(const std::string& code) -> bool;
        const std::regex selcalPattern;
    };
} // namespace UKControllerPlugin::Selcal
