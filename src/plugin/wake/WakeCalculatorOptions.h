#pragma once

namespace UKControllerPlugin::Wake {
    class WakeCategoryMapperInterface;
    
    /**
     * Selected options in relation to the Wake Calculator Display
     */
    class WakeCalculatorOptions
    {
        public:
        [[nodiscard]] auto LeadAircraft() const -> const std::string&;
        void LeadAircraft(const std::string& leadAircraft);
        [[nodiscard]] auto FollowingAircraft() const -> const std::string&;
        void FollowingAircraft(const std::string& followingAircraft);
        [[nodiscard]] auto Scheme() const -> const std::string&;
        void Scheme(const std::string& scheme);
        [[nodiscard]] auto SchemeMapper() const -> std::shared_ptr<WakeCategoryMapperInterface>;
        void SchemeMapper(const std::shared_ptr<WakeCategoryMapperInterface> mapper);
        [[nodiscard]] auto Intermediate() const -> bool;
        void Intermediate(bool intermediate);

        private:
            // Who the lead aircraft is
            std::string leadAircraft;
            
            // Who the following aircraft is
            std::string followingAircraft;
            
            // Are we doing intermediate departures
            bool intermediate = false;
            
            // What scheme we're using
            std::string scheme;
            
            // For scheme mapping
            std::shared_ptr<WakeCategoryMapperInterface> schemeMapper;
    };
} // namespace UKControllerPlugin::Wake
