#pragma once

namespace UKControllerPlugin::Windows {
    class GdiGraphicsInterface;
} // namespace UKControllerPlugin::Windows

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
        virtual void Draw(Windows::GdiGraphicsInterface& graphics, const Gdiplus::Rect& area) = 0;
    };
} // namespace UKControllerPlugin::Mapping
