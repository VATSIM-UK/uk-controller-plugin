#pragma once

namespace UKControllerPlugin::Theme {

    /*
        Defines a colour as used in a palette.

        The default colour is a signaling value and should not appear in any
        actual palettes.
    */
    class PaletteEntry
    {
        public:
        constexpr PaletteEntry() = default;

        explicit constexpr PaletteEntry(Gdiplus::ARGB argb) : value(argb)
        {
        }

        explicit constexpr PaletteEntry(const char* hex)
        {
            value = 0xff;

            if (*hex == '#')
                ++hex;

            for (char c = *hex; c; c = *++hex) {
                value <<= 4;
                if ('0' <= *hex && *hex <= '9')
                    value += *hex - '0';
                else if ('A' <= *hex && *hex <= 'F')
                    value += *hex - 'A' + 0xa;
                else if ('a' <= *hex && *hex <= 'f')
                    value += *hex - 'a' + 0xa;
                else
                    throw std::invalid_argument("invalid hex literal");
            }
        }

        Gdiplus::ARGB value = 0x00ff00ff;
    };

} // namespace UKControllerPlugin::Theme
