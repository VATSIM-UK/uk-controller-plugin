#pragma once
#include "tag/TagData.h"

namespace UKControllerPlugin {
    namespace Tag {

        /*
            Interface for TagItems to follow. Provides facilities
            for calling the tag item and also providing a human readable description.
        */
        class TagItemInterface
        {
            public:
                virtual std::string GetTagItemDescription(int tagItemId) const = 0;
                virtual void SetTagItemData(UKControllerPlugin::Tag::TagData & tagData) = 0;
        };
    }  // namespace Tag
}  // namespace UKControllerPlugin
