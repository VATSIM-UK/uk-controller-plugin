#include "MockConfigurableDisplay.h"

namespace UKControllerPluginTest::RadarScreen {
    MockConfigurableDisplay::MockConfigurableDisplay(std::string firstValue, std::string secondValue)
        : firstValue(firstValue), secondValue(secondValue)
    {
    }

    MockConfigurableDisplay::MockConfigurableDisplay() : firstValue("noValue1"), secondValue("noValue2")
    {
    }

    MockConfigurableDisplay::~MockConfigurableDisplay() = default;
} // namespace UKControllerPluginTest::RadarScreen
