#pragma once

namespace UKControllerPlugin::FlightRules {
    struct FlightRule;

    /**
     * Contains all the flight rules.
     */
    class FlightRuleCollection
    {
        public:
        void Add(const std::shared_ptr<FlightRule>& rule);
        [[nodiscard]] auto Count() const -> size_t;
        [[nodiscard]] auto Get(int id) const -> std::shared_ptr<FlightRule>;

        private:
        std::map<int, std::shared_ptr<FlightRule>> flightRules;
    };
} // namespace UKControllerPlugin::FlightRules
