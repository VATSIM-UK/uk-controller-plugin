#include "pch/pch.h"
#include "controller/TranslateFrequencyAbbreviation.h"

namespace UKControllerPlugin {
    namespace Controller {

        std::map <std::string, std::string> translations = {
            {"ESX", "ESSEX"},
            {"SOL", "SOLENT"},
            {"THA", "THAMES"},
            {"TMS", "THAMES"}
        };

        /*
            Given a recognised abbreviation for a facility, translate
            it to the full facility for use in matching controller positions.
        */
        std::string TranslateFrequencyAbbreviation(std::string facility)
        {
            return translations.count(facility) ? translations.at(facility) : facility;
        }
    }  // namespace Controller
}  // namespace UKControllerPlugin
