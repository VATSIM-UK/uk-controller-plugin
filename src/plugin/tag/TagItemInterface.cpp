#include "TagData.h"
#include "TagItemInterface.h"

namespace UKControllerPlugin::Tag {
    TagItemInterface::TagItemInterface() = default;
    TagItemInterface::~TagItemInterface() = default;
    TagItemInterface::TagItemInterface(const TagItemInterface& copyFrom) = default;
    TagItemInterface::TagItemInterface(TagItemInterface&&) noexcept = default;
    auto TagItemInterface::operator=(const TagItemInterface&) -> TagItemInterface& = default;
    auto TagItemInterface::operator=(TagItemInterface&&) noexcept -> TagItemInterface& = default;
} // namespace UKControllerPlugin::Tag
