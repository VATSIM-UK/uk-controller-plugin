#pragma once
#include "pch/pch.h"
#include "plugin/PopupMenuItem.h"
#include "radarscreen/ConfigurableDisplayInterface.h"

namespace UKControllerPluginTest {
    namespace RadarScreen {

        /*
            Just a test class to test with
        */
        class MockConfigurableDisplay : public UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface
        {
            public:

            MockConfigurableDisplay(std::string firstValue, std::string secondValue)
                : firstValue(firstValue), secondValue(secondValue)
            {

            }

            MockConfigurableDisplay(void)
                : firstValue("noValue1"), secondValue("noValue2")
            {

            }

            void Configure(int, std::string, RECT) override
            {
                this->configured++;
            }

            UKControllerPlugin::Plugin::PopupMenuItem GetConfigurationMenuItem(void) const override
            {
                UKControllerPlugin::Plugin::PopupMenuItem returnVal;
                returnVal.firstValue = this->firstValue;
                returnVal.secondValue = this->secondValue;
                returnVal.callbackFunctionId = 1234;
                returnVal.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
                returnVal.disabled = false;
                returnVal.fixedPosition = false;
                return returnVal;
            }

            // Values to differentiate
            int configured = 0;
            const std::string firstValue;
            const std::string secondValue;
        };
    }  // namespace RadarScreen
}  // namespace UKControllerPluginTest
