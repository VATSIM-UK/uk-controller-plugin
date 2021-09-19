#include "components/BrushSwitcher.h"
#include "time/SystemClock.h"

namespace UKControllerPlugin::Components {

    std::shared_ptr<BrushSwitcher>
    BrushSwitcher::Create(std::shared_ptr<Gdiplus::Brush> baseBrush, const std::chrono::seconds phase)
    {
        return std::shared_ptr<BrushSwitcher>(new BrushSwitcher(baseBrush, phase));
    }

    std::shared_ptr<BrushSwitcher> BrushSwitcher::AdditionalBrush(std::shared_ptr<Gdiplus::Brush> brush)
    {
        this->brushes.push_back(brush);
        return shared_from_this();
    }

    std::shared_ptr<Gdiplus::Brush> BrushSwitcher::Base() const
    {
        return this->baseBrush;
    }

    std::shared_ptr<Gdiplus::Brush> BrushSwitcher::Next()
    {
        if (this->brushes.empty()) {
            return this->Base();
        }

        if (this->nextSwitch < Time::TimeNow()) {
            this->nextSwitch = Time::TimeNow() + this->phase;

            if (this->brushIndex == this->brushes.size() - 1) {
                this->brushIndex = 0;
            } else {
                this->brushIndex++;
            }
        }

        return this->brushes.at(this->brushIndex);
    }

    BrushSwitcher::BrushSwitcher(std::shared_ptr<Gdiplus::Brush> baseBrush, const std::chrono::seconds phase)
        : baseBrush(std::move(baseBrush)), phase(phase)
    {
        this->brushes.push_back(this->baseBrush);
    }
} // namespace UKControllerPlugin::Components
