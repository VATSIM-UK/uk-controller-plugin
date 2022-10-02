#pragma once

namespace UKControllerPlugin {
    namespace RadarScreen {
        class ConfigurableDisplayInterface;
    } // namespace RadarScreen
} // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopeRadarLoopbackInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace RadarScreen {

        /*
            A collection of classes that render to the screen and can be configured through
            a configuration clickspot on the screen.
        */
        class ConfigurableDisplayCollection
        {
            public:
            size_t CountDisplays(void) const;
            void
            RegisterDisplay(std::shared_ptr<UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface> radarScreen);

            typedef std::list<std::shared_ptr<UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface>>
                DisplaySet;
            typedef DisplaySet::iterator iterator;
            typedef DisplaySet::const_iterator const_iterator;
            const_iterator cbegin() const
            {
                return displays.cbegin();
            } // namespace UKControllerPlugin
            const_iterator cend() const
            {
                return displays.cend();
            } // namespace Euroscope

            private:
            std::list<std::shared_ptr<UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface>> displays;
        };

    } // namespace RadarScreen
} // namespace UKControllerPlugin
