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
        void AddAirfield(std::shared_ptr<UKControllerPlugin::Airfield::AirfieldModel> airfield);
        [[nodiscard]] auto FetchAirfieldByIcao(const std::string& icao) const -> std::shared_ptr<AirfieldModel>;
        void ForEach(const std::function<void(const AirfieldModel& airfield)>& callback) const;
        [[nodiscard]] auto GetSize() const -> size_t;

        private:
        // A map of ICAO code to airfield.
        std::map<int, std::shared_ptr<AirfieldModel>> airfieldMap;
    };
} // namespace UKControllerPlugin::Airfield
