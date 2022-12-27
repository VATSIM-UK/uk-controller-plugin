#pragma once

namespace UKControllerPlugin::IntentionCode {
    class ExitDetermination;

    class FirExitPoint {
        public:
            FirExitPoint(int id, std::string identifier, bool internal, std::shared_ptr<ExitDetermination> exitDetermination);
            ~FirExitPoint() = default;

        private:
        [[nodiscard]] auto Id() const -> int;
        [[nodiscard]] auto Identifier() const -> const std::string&;
        [[nodiscard]] auto IsInternal() const -> bool;
        [[nodiscard]] auto ExitDetermination() const -> const ExitDetermination&;

        // The id of the exit point
        int id;

        // Identifier
        std::string identifier;

        // Is it internal (ie between EGTT and EGPX)
        bool internal;

        // Determining whether an aircraft is exiting the FIR here
        std::shared_ptr<ExitDetermination> exitDetermination;
    };
} // namespace UKControllerPlugin::IntentionCode