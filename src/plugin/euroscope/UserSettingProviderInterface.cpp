#include "UserSettingProviderInterface.h"

namespace UKControllerPlugin::Euroscope {
    UserSettingProviderInterface::UserSettingProviderInterface() = default;
    UserSettingProviderInterface::~UserSettingProviderInterface() = default;
    UserSettingProviderInterface::UserSettingProviderInterface(const UserSettingProviderInterface&) = default;
    auto UserSettingProviderInterface::operator=(const UserSettingProviderInterface&)
        -> UserSettingProviderInterface& = default;
} // namespace UKControllerPlugin::Euroscope
