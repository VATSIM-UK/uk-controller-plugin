#pragma once
#include "MappingElementDrawer.h"

namespace UKControllerPlugin::Mapping {
    /**
     * Draws a visual reference point.
     */
    class VisualReferencePoint : public MappingElementDrawer
    {
        public:
        VisualReferencePoint();
        void Draw(Windows::GdiGraphicsInterface& graphics, const Gdiplus::Rect& area) override;
        
        private:
         std::shared_ptr<Gdiplus::Pen> pen;
    };
} //
