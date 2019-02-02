#pragma once

namespace UKControllerPlugin {
    namespace Hold {

        /*
            A struct to conveniently represent
            the dimensions of an element in the
            hold display window
        */
        typedef struct HoldElementDimensions {
            const long topLeftX;
            const long topLeftY;
            const long width;
            const long height;

            bool operator==(const HoldElementDimensions & compare) const
            {
                return this->topLeftX == compare.topLeftX &&
                    this->topLeftY == compare.topLeftY &&
                    this->width == compare.width &&
                    this->height == compare.height;
            }

        } HoldElementDimensions;

    }  // namespace Hold
}  // namespace UKControllerPlugin
