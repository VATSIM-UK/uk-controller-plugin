#pragma once

namespace UKControllerPlugin::Graphics {
    /**
     * Manages font objects for the entire plugin so we dont
     * have many duplicates
     */
    class FontManager
    {
        public:
        [[nodiscard]] static auto Instance() -> FontManager&;
        [[nodiscard]] auto Get(int size) -> const Gdiplus::Font&;
        [[nodiscard]] auto GetDefault() -> const Gdiplus::Font&;

        protected:
        FontManager(std::unique_ptr<Gdiplus::FontFamily> family);
        static std::unique_ptr<FontManager> instance;

        private:
        // The font family
        std::unique_ptr<Gdiplus::FontFamily> family;

        // The fonts
        std::map<int, std::shared_ptr<Gdiplus::Font>> fonts;
    };
} // namespace UKControllerPlugin::Graphics
