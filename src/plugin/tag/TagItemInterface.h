#pragma once

namespace UKControllerPlugin::Tag {
    class TagData;

    /*
        Interface for TagItems to follow. Provides facilities
        for calling the tag item and also providing a human readable description.
    */
    class TagItemInterface
    {
        public:
        TagItemInterface();
        virtual ~TagItemInterface();
        TagItemInterface(const TagItemInterface& copyFrom);
        TagItemInterface(TagItemInterface&&) noexcept;
        auto operator=(const TagItemInterface&) -> TagItemInterface&;
        auto operator=(TagItemInterface&&) noexcept -> TagItemInterface&;
        [[nodiscard]] virtual auto GetTagItemDescription(int tagItemId) const -> std::string = 0;
        virtual void SetTagItemData(UKControllerPlugin::Tag::TagData& tagData) = 0;
    };
} // namespace UKControllerPlugin::Tag
