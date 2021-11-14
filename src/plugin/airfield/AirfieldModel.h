#pragma once

namespace UKControllerPlugin::Controller {
    class ControllerPositionHierarchy;
} // namespace UKControllerPlugin::Controller

namespace UKControllerPlugin::Airfield {

    /*
        A class representing a UK airfield. Contains details such as the top-down
        order, as well as who currently owns it.
    */
    class AirfieldModel
    {
        public:
        AirfieldModel(int id, std::string icao, std::unique_ptr<Controller::ControllerPositionHierarchy> topDownOrder);
        ~AirfieldModel();
        AirfieldModel(const AirfieldModel&) = delete;
        AirfieldModel(AirfieldModel&&) noexcept;
        auto operator=(const AirfieldModel&) -> AirfieldModel& = delete;
        auto operator=(AirfieldModel&&) noexcept -> AirfieldModel&;
        [[nodiscard]] auto Id() const -> int;
        [[nodiscard]] auto Icao() const -> std::string;
        [[nodiscard]] auto TopDownOrder() const -> const Controller::ControllerPositionHierarchy&;
        auto operator==(const AirfieldModel& compare) const -> bool;

        private:
        // The id of the airfield
        int id;

        // The airfield ICAO
        std::string icao;

        // The order of who owns this airfield.
        std::unique_ptr<Controller::ControllerPositionHierarchy> topDownOrder;
    };
} // namespace UKControllerPlugin::Airfield
