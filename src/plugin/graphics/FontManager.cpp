#include "FontManager.h"

namespace UKControllerPlugin::Graphics {

    auto FontManager::Instance() -> FontManager&
    {
        static std::unique_ptr<FontManager> instance;
        if (instance == nullptr) {
            instance =
                std::unique_ptr<FontManager>(new FontManager(std::make_unique<Gdiplus::FontFamily>(L"EuroScope")));
        }

        return *instance;
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
} // namespace UKControllerPlugin::Graphics
