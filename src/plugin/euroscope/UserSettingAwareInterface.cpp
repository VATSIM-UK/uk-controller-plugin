#include "UserSettingAwareInterface.h"

namespace UKControllerPlugin::Euroscope {
    UserSettingAwareInterface::UserSettingAwareInterface() = default;
    UserSettingAwareInterface::~UserSettingAwareInterface() = default;
    UserSettingAwareInterface::UserSettingAwareInterface(const UserSettingAwareInterface&) = default;
    UserSettingAwareInterface::UserSettingAwareInterface(UserSettingAwareInterface&&) noexcept = default;
    [[nodiscard]] auto UserSettingAwareInterface::operator=(const UserSettingAwareInterface&)
        -> UserSettingAwareInterface& = default;
    [[nodiscard]] auto UserSettingAwareInterface::operator=(UserSettingAwareInterface&&) noexcept
        -> UserSettingAwareInterface& = default;
} // namespace UKControllerPlugin::Euroscope
