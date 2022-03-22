#pragma once

namespace UKControllerPlugin::Graphics {
    class GdiGraphicsInterface;
} // namespace UKControllerPlugin::Graphics

namespace UKControllerPlugin::Mapping {

    /**
     * A class that knows how to draw a mapping element.
     */
    class MappingElementDrawer
    {
        public:
        virtual ~MappingElementDrawer() = default;

        /**
         * Draw the mapping element. The area is centered on the centre of the element.
         */
        virtual void Draw(Graphics::GdiGraphicsInterface& graphics, const Gdiplus::Rect& area) = 0;
    };
} // namespace UKControllerPlugin::Mapping
