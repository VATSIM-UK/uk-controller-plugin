#pragma once

namespace UKControllerPlugin::Euroscope {
    class EuroscopeExtractedRouteInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::IntentionCode {
    /*
        Abstract class that represents a group of airfields. Internally, it contains a list
        of all airfields within the group, as well as functions to check whether an airfield is
        known and to return an intention code for the group.
    */
    class AirfieldGroup
    {
        public:
        AirfieldGroup();
        virtual ~AirfieldGroup();
        AirfieldGroup(const AirfieldGroup&);
        AirfieldGroup(AirfieldGroup&&);
        [[nodiscard]] auto operator=(const AirfieldGroup&) -> AirfieldGroup& = delete;
        [[nodiscard]] auto operator=(const AirfieldGroup&&) -> AirfieldGroup& = delete;
        virtual auto AppliesToController(
            const std::string& callsign, UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route) const
            -> bool;
        virtual auto HasAirfield(
            const std::string& airfield, UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route) const
            -> bool = 0;
        virtual auto GetIntentionCodeForGroup(
            const std::string& airfield, UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route) const
            -> std::string = 0;

        protected:
        void AddAirfieldToList(const std::string& airfield);
        [[nodiscard]] auto AirfieldInList(const std::string& airfield) const -> bool;
        [[nodiscard]] auto ControllerIsScottish(const std::string& callsign) const -> bool;
        virtual auto Initialise() -> bool = 0;
        static auto
        IsViaPoint(const std::string& point, UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route)
            -> bool;

        private:
        std::vector<std::string> airfields;

        const std::string SCAC_CALLSIGN_PREFIX = "SCO";
        const std::string SCTC_CALLSIGN_PREFIX = "STC";
        const std::string EGPX_CALLSIGN_PREFIX = "EGP";
        const std::string EGPX_MIL_CALLSIGN_PREFIX = "EGQ";
        const std::string EGPX_NI_CALLSIGN_PREFIX = "EGA";
    };
} // namespace UKControllerPlugin::IntentionCode
