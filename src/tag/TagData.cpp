#include "pch/stdafx.h"
#include "tag/TagData.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;

namespace UKControllerPlugin {
    namespace Tag {

        TagData::TagData(
            const EuroScopeCFlightPlanInterface& flightPlan,
            const EuroScopeCRadarTargetInterface& radarTarget,
            const int itemCode,
            const int dataAvailable,
            char * itemString,
            int* euroscopeColourCode,
            COLORREF* tagColour,
            double* fontSize
        ) 
            : flightPlan(flightPlan), radarTarget(radarTarget), itemCode(itemCode), dataAvailable(dataAvailable),
            itemString(itemString), euroscopeColourCode(euroscopeColourCode), tagColour(tagColour), fontSize(fontSize)
        {
        }


        std::string TagData::GetItemString(void) const
        {
            return std::string(this->itemString);
        }

        void TagData::SetItemString(std::string itemString)
        {
            // We only allow data of length maxlength - 1 because of null char on the end.
            if (itemString.size() > this->maxItemSize - 1) {
                itemString = this->invalidItemText;
            }

            // Copy into place
            strcpy_s(this->itemString, 16, itemString.c_str());
        }

        void TagData::SetEuroscopeColourCode(int code)
        {
            *this->euroscopeColourCode = code;
        }

        int TagData::GetEuroscopeColourCode(void) const
        {
            return *this->euroscopeColourCode;
        }

        void TagData::SetTagColour(COLORREF colour)
        {
            *this->euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_RGB_DEFINED;
            *this->tagColour = colour;
        }

        COLORREF TagData::GetTagColour(void) const
        {
            return *this->tagColour;
        }

        void TagData::SetFontSize(double fontSize)
        {
            *this->fontSize = fontSize;
        }

        double TagData::GetFontSize(void) const
        {
            return *this->fontSize;
        }

    }  // namespace Tag
}  // namespace UKControllerPlugin
