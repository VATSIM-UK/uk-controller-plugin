#pragma once

namespace UKControllerPlugin {
    namespace Controller {
        class ControllerPositionHierarchy;
    } // namespace Controller
    namespace Prenote {
        struct PairedAirfieldPrenote;
    } // namespace Prenote
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Airfield {

    /*
        A class representing a UK airfield. Contains details such as the top-down
        order, as well as who currently owns it.
    */
    class AirfieldModel
    {
        public:
        AirfieldModel(
            int id,
            std::string icao,
            std::unique_ptr<Controller::ControllerPositionHierarchy> topDownOrder,
            std::vector<std::shared_ptr<Prenote::PairedAirfieldPrenote>> airfieldPairingPrenotes = {},
            int defaultHandoffId = NO_HANDOFF_ID);
        ~AirfieldModel();
        AirfieldModel(const AirfieldModel&) = delete;
        AirfieldModel(AirfieldModel&&) noexcept;
        auto operator=(const AirfieldModel&) -> AirfieldModel& = delete;
        auto operator=(AirfieldModel&&) noexcept -> AirfieldModel&;
        [[nodiscard]] auto Id() const -> int;
        [[nodiscard]] auto Icao() const -> std::string;
        [[nodiscard]] auto TopDownOrder() const -> const Controller::ControllerPositionHierarchy&;
        [[nodiscard]] auto AirfieldPairingPrenotes() const
            -> const std::vector<std::shared_ptr<Prenote::PairedAirfieldPrenote>>&;
        [[nodiscard]] auto HasDefaultHandoff() const -> bool;
        [[nodiscard]] auto DefaultHandoff() const -> int;
        auto operator==(const AirfieldModel& compare) const -> bool;

        private:
        // The default handoff id if there is none
        inline static const int NO_HANDOFF_ID = 0;

        // The id of the airfield
        int id;

        // The airfield ICAO
        std::string icao;

        // The order of who owns this airfield.
        std::unique_ptr<Controller::ControllerPositionHierarchy> topDownOrder;

        // Prenotes for an airfield pairing
        std::vector<std::shared_ptr<Prenote::PairedAirfieldPrenote>> airfieldPairingPrenotes;

        // The default handoff order to use for this airfield
        int defaultHandoffId;
    };
} // namespace UKControllerPlugin::Airfield
