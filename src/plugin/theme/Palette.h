#pragma once

#include "theme/PaletteEntry.h"
#include "theme/PaletteKey.h"

namespace UKControllerPlugin::Theme {

    /*
        Defines the colours used by a particular UI palette.
    */
    class Palette
    {
        public:
        [[nodiscard]] auto GetEntry(PaletteKey key) const -> PaletteEntry;
        [[nodiscard]] auto GetId() const -> const char*;
        [[nodiscard]] auto GetName() const -> const wchar_t*;

        [[nodiscard]] static auto GetPalettes() -> const std::vector<const Palette*>&;
        [[nodiscard]] static auto GetPalette(const char* id) -> const Palette*;

        constexpr Palette(
            const char* id, const wchar_t* name, std::initializer_list<std::pair<PaletteKey, const char*>> entries);
        constexpr Palette(
            const char* id,
            const wchar_t* name,
            const Palette& base,
            std::initializer_list<std::pair<PaletteKey, const char*>> overrides);

        constexpr static const char* DEFAULT = "default";

        private:
        const char* id;
        const wchar_t* name;
        std::array<PaletteEntry, PALETTE_KEY_COUNT> entries;

        static const std::vector<const Palette*> PALETTES;
    };

} // namespace UKControllerPlugin::Theme
