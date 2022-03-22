#pragma once

namespace UKControllerPlugin::Windows {
    class GdiGraphicsInterface;
} // namespace UKControllerPlugin::Windows

namespace UKControllerPlugin::Mapping {
    /**
     * An element that can be drawn or otherwise represented on the
     * map.
     */
    class MappingElementInterface
    {
        public:
        virtual ~MappingElementInterface() = default;

        /**
         * The id of the element.
         */
        [[nodiscard]] virtual auto Id() const -> int = 0;

        /**
         * The type of the element to be drawn.
         */
        [[nodiscard]] virtual auto Type() const -> const std::string& = 0;

        /**
         * The label associated with the element.
         */
        [[nodiscard]] virtual auto Label() const -> const std::string& = 0;

        /**
         * The label associated with the element.
         */
        [[nodiscard]] virtual auto Position() const -> const EuroScopePlugIn::CPosition& = 0;

        /**
         * Should we be displaying the mapping element
         */
        [[nodiscard]] virtual auto ShouldDisplay() const -> bool = 0;

        /**
         * Draw the mapping element. The area is centered on the centre of the element.
         */
        virtual void Draw(Windows::GdiGraphicsInterface& graphics, Gdiplus::Rect& area) = 0;
    };
} // namespace UKControllerPlugin::Mapping
