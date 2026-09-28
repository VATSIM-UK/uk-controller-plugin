#include "ThemeManager.h"

namespace UKControllerPlugin::Theme {

    auto ThemeManager::Instance() -> ThemeManager&
    {
        if (!instance) {
            instance = std::unique_ptr<ThemeManager>(new ThemeManager());
        }

        return *instance;
    }

    void ThemeManager::UnsetInstance()
    {
        instance = nullptr;
    }

    auto ThemeManager::Colour(PaletteKey key) -> Gdiplus::Color
    {
        return Instance().GetColour(key);
    }

    auto ThemeManager::Brush(PaletteKey key) -> const Gdiplus::SolidBrush&
    {
        return Instance().GetBrush(key);
    }

    auto ThemeManager::Pen(PaletteKey key) -> const Gdiplus::Pen&
    {
        return Instance().GetPen(key);
    }

    auto ThemeManager::GetColour(PaletteKey key) const -> Gdiplus::Color
    {
        return colours[static_cast<std::size_t>(key)];
    }

    auto ThemeManager::GetBrush(PaletteKey key) const -> const Gdiplus::SolidBrush&
    {
        return *brushes[static_cast<std::size_t>(key)];
    }

    auto ThemeManager::GetPen(PaletteKey key) const -> const Gdiplus::Pen&
    {
        return *pens[static_cast<std::size_t>(key)];
    }

    void ThemeManager::ApplyPalette(const Palette* palette)
    {
        for (std::size_t key = 0; key < PALETTE_KEY_COUNT; ++key) {
            auto entry = palette->GetEntry(static_cast<PaletteKey>(key));
            auto colour = colours[key] = Gdiplus::Color(entry.value);
            brushes[key] = std::make_unique<Gdiplus::SolidBrush>(colour);
            pens[key] = std::make_unique<Gdiplus::Pen>(colour);
        }
    }

    ThemeManager::ThemeManager()
    {
        // Apply default palette
        ApplyPalette(Palette::GetPalette(""));
    }

} // namespace UKControllerPlugin::Theme
