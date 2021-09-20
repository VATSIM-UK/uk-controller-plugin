#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopeRadarLoopbackInterface;
    } // namespace Euroscope
    namespace Windows {
        class GdiGraphicsInterface;
    } // namespace Windows
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Components {

    /*
     * A class that switches brushes on each tick, used to implement
     * flashing colours.
     */
    class BrushSwitcher : public std::enable_shared_from_this<BrushSwitcher>
    {
        public:
        static std::shared_ptr<BrushSwitcher>
        Create(std::shared_ptr<Gdiplus::Brush> baseBrush, std::chrono::seconds phase);
        std::shared_ptr<BrushSwitcher> AdditionalBrush(std::shared_ptr<Gdiplus::Brush> baseBrush);
        std::shared_ptr<Gdiplus::Brush> Base() const;
        std::shared_ptr<Gdiplus::Brush> Next();

        protected:
        explicit BrushSwitcher(std::shared_ptr<Gdiplus::Brush> baseBrush, std::chrono::seconds phase);

        private:
        // The base colour
        std::shared_ptr<Gdiplus::Brush> baseBrush;

        // The colours to cycle
        std::vector<std::shared_ptr<Gdiplus::Brush>> brushes;

        // The current colour index
        unsigned int brushIndex = 0;

        // Last time colours were switched
        std::chrono::system_clock::time_point nextSwitch = (std::chrono::system_clock::time_point::min)();

        // How often to switch colours
        const std::chrono::seconds phase;
    };
} // namespace UKControllerPlugin::Components
