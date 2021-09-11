#include "TagData.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;

namespace UKControllerPlugin::Tag {

    TagData::TagData(
        const EuroScopeCFlightPlanInterface& flightPlan,
        const EuroScopeCRadarTargetInterface& radarTarget,
        const int itemCode,
        const int dataAvailable,
        char* itemString,
        int* euroscopeColourCode,
        COLORREF* tagColour,
        double* fontSize)
        : flightPlan(flightPlan), radarTarget(radarTarget), itemCode(itemCode), dataAvailable(dataAvailable),
          itemString(itemString), euroscopeColourCode(euroscopeColourCode), tagColour(tagColour), fontSize(fontSize)
    {
    }

    auto TagData::GetItemString() const -> std::string
    {
        return {this->itemString};
    }

    void TagData::SetItemString(std::string itemString)
    {
        // We only allow data of length maxlength - 1 because of null char on the end.
        if (itemString.size() > maxItemSize - 1) {
            itemString = this->invalidItemText;
        }

        // Copy into place
        strcpy_s(this->itemString, maxItemSize, itemString.c_str());
    }

    void TagData::SetEuroscopeColourCode(int code)
    {
        *this->euroscopeColourCode = code;
    }

    auto TagData::GetEuroscopeColourCode() const -> int
    {
        return *this->euroscopeColourCode;
    }

    void TagData::SetTagColour(COLORREF colour)
    {
        *this->euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_RGB_DEFINED;
        *this->tagColour = colour;
    }

    auto TagData::GetTagColour() const -> COLORREF
    {
        return *this->tagColour;
    }

    void TagData::SetFontSize(double fontSize)
    {
        *this->fontSize = fontSize;
    }

    auto TagData::GetFontSize() const -> double
    {
        return *this->fontSize;
    }
    
    auto TagData::GetItemCode() const -> int
    {
        return this->itemCode;
    }
    
    auto TagData::GetFlightplan() const -> Euroscope::EuroScopeCFlightPlanInterface&
    {
        return this->flightPlan;
    }
    
    auto TagData::GetRadarTarget() const -> Euroscope::EuroScopeCRadarTargetInterface&
    {
        return this->radarTarget;
    }
} // namespace UKControllerPlugin::Tag
