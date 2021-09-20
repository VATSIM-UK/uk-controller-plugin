#pragma once

namespace UKControllerPlugin::Airfield {

    class AirfieldModel;

    /*
        A class representing a collection of airfields. Allows airfields to be
        retrieved by their ICAO code.
    */
    class AirfieldCollection
    {
        public:
        AirfieldCollection();
        ~AirfieldCollection();
        AirfieldCollection(const AirfieldCollection& copyFrom) = delete;
        AirfieldCollection(AirfieldCollection&&) noexcept;
        auto operator=(const AirfieldCollection&) -> AirfieldCollection& = delete;
        auto operator=(AirfieldCollection&&) noexcept -> AirfieldCollection&;
        void AddAirfield(std::unique_ptr<UKControllerPlugin::Airfield::AirfieldModel> airfield);
        [[nodiscard]] auto FetchAirfieldByIcao(const std::string& icao) const -> const AirfieldModel&;
        [[nodiscard]] auto GetSize() const -> size_t;

        private:
        static auto IsHomeAirfield(const std::string& icao) -> bool;

        // A map of ICAO code to airfield.
        std::map<std::string, std::unique_ptr<UKControllerPlugin::Airfield::AirfieldModel>> airfieldMap;
    };
} // namespace UKControllerPlugin::Airfield
