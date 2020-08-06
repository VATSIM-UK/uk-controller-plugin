#pragma once
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"

namespace UKControllerPlugin {
    namespace Tag {
        /*
            A new class
        */
        class TagData
        {
            public:
                TagData(
                    const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    const UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget,
                    int itemCode,
                    int dataAvailable,
                    char itemString[16],
                    int * euroscopeColourCode,
                    COLORREF * tagColour,
                    double * fontSize
                );

                std::string GetItemString(void) const;
                void SetItemString(std::string itemString);
                void SetEuroscopeColourCode(int code);
                int GetEuroscopeColourCode(void) const;
                void SetTagColour(COLORREF colour);
                COLORREF GetTagColour(void) const;
                void SetFontSize(double fontSize);
                double GetFontSize(void) const;

                
                // The tag item text is too long
                const std::string invalidItemText = "INVALID";

                // Max length we can have on TAG items, 15 characters + 1 null terminator
                const size_t maxItemSize = 16;

                // The flightplan
                const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan;

                // The radar target
                const UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget;

                // The code for the tag item
                const int itemCode;

                // What data is available - e.g. correlated track, uncorrelated etcx
                const int dataAvailable;

            private:

                // The string to put into the tag
                char * itemString;

                // The colour code for Euroscope;
                int* euroscopeColourCode;

                // The custom colour to use for the tag
                COLORREF* tagColour;

                // The font size to use
                double* fontSize;
        };
    }  // namespace Tag
}  // namespace UKControllerPlugin
