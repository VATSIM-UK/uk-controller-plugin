#pragma once

namespace UKControllerPlugin::Approach {
    /**
     * Options for the approach sequencer display.
     */
    class ApproachSequencerDisplayOptions
    {
        public:
        [[nodiscard]] auto Position() const -> const POINT&;
        void Position(const POINT& position);
        [[nodiscard]] auto Airfield() const -> const std::string&;
        void Airfield(const std::string& airfield);
        [[nodiscard]] auto ContentCollapsed() const -> bool;
        void ToggleCollapsed();
        void SetCollapsed(bool collapsed);
        [[nodiscard]] auto IsVisible() const -> bool;
        void ToggleVisible();
        void SetVisible(bool visible);

        private:
        // Where we are displaying
        POINT position{200, 200};

        // The airfield we are displaying for
        std::string airfield;

        // Is the content collapsed
        bool contentCollapsed = false;

        // Are we visible
        bool isVisible = false;
    };
} // namespace UKControllerPlugin::Approach
