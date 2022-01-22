#pragma once

namespace UKControllerPlugin::Graphics {
    /**
     * Stores string formats
     */
    class StringFormatManager
    {
        public:
        [[nodiscard]] static auto Instance() -> StringFormatManager&;
        [[nodiscard]] auto GetCentreAlign() -> const Gdiplus::StringFormat&;
        [[nodiscard]] auto GetLeftAlign() -> const Gdiplus::StringFormat&;

        protected:
        StringFormatManager();
        auto GetAligned(int id, Gdiplus::StringAlignment alignment) -> const Gdiplus::StringFormat&;
        static std::unique_ptr<StringFormatManager> instance;

        private:
        // The fonts
        std::map<int, std::shared_ptr<Gdiplus::StringFormat>> formats;
    };
} // namespace UKControllerPlugin::Graphics
