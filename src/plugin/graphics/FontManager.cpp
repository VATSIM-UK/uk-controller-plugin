#include "FontManager.h"

namespace UKControllerPlugin::Graphics {

    auto FontManager::Instance() -> FontManager&
    {
        if (FontManager::instance == nullptr) {
            auto euroscopeFontFamily = std::make_unique<Gdiplus::FontFamily>(L"EuroScope");
            FontManager::instance = std::unique_ptr<FontManager>(new FontManager(
                euroscopeFontFamily->IsAvailable() ? std::move(euroscopeFontFamily)
                                                   : std::make_unique<Gdiplus::FontFamily>(L"Segoe UI")));
        }

        return *FontManager::instance;
    }

    auto FontManager::GetDefault() -> const Gdiplus::Font&
    {
        return this->Get(9);
    }

    auto FontManager::Get(int size) -> const Gdiplus::Font&
    {
        if (this->fonts.count(size) == 0) {
            this->fonts[size] = std::make_shared<Gdiplus::Font>(family.get(), size);
        }

        return *this->fonts.at(size);
    }

    FontManager::FontManager(std::unique_ptr<Gdiplus::FontFamily> family) : family(std::move(family))
    {
    }

    void FontManager::UnsetInstance()
    {
        FontManager::instance = nullptr;
    }
} // namespace UKControllerPlugin::Graphics
