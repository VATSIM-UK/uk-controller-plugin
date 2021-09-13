#pragma once

namespace UKControllerPlugin::Euroscope {
    class EuroScopeCFlightPlanInterface;
    class EuroScopeCRadarTargetInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Tag {
    /*
        Amalgamates all of the data EuroScope uses when requesting a tag function to make it
        nicer to pass around.
    */
    class TagData
    {
        public:
        TagData(
            const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            const UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
            int itemCode,
            int dataAvailable,
            char* itemString,
            int* euroscopeColourCode,
            COLORREF* tagColour,
            double* fontSize);

        [[nodiscard]] auto GetItemString() const -> std::string;
        void SetItemString(std::string itemString);
        void SetEuroscopeColourCode(int code);
        [[nodiscard]] auto GetEuroscopeColourCode() const -> int;
        void SetTagColour(COLORREF colour);
        [[nodiscard]] auto GetTagColour() const -> COLORREF;
        void SetFontSize(double fontSize);
        [[nodiscard]] auto GetFontSize() const -> double;
        [[nodiscard]] auto GetItemCode() const -> int;
        [[nodiscard]] auto GetDataAvailable() const -> int;
        [[nodiscard]] auto GetFlightplan() const -> const Euroscope::EuroScopeCFlightPlanInterface&;
        [[nodiscard]] auto GetRadarTarget() const -> const Euroscope::EuroScopeCRadarTargetInterface&;

        private:
        // The string to put into the tag
        char* itemString;

        // The colour code for Euroscope;
        int* euroscopeColourCode;

        // The custom colour to use for the tag
        COLORREF* tagColour;

        // The font size to use
        double* fontSize;

        // The tag item text is too long
        const std::string invalidItemText = "INVALID";

        // Max length we can have on TAG items, 15 characters + 1 null terminator
        static const size_t maxItemSize = 16;

        // The flightplan
        const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan;

        // The radar target
        const UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget;

        // The code for the tag item
        const int itemCode;

        // What data is available - e.g. correlated track, uncorrelated etc
        const int dataAvailable;
    };
} // namespace UKControllerPlugin::Tag
