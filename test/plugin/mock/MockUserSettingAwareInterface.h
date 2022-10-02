#pragma once
#include "euroscope/UserSettingAwareInterface.h"
#include "euroscope/UserSetting.h"

namespace UKControllerPluginTest {
    namespace Euroscope {

        class MockUserSettingAwareInterface : public UKControllerPlugin::Euroscope::UserSettingAwareInterface
        {
            public:
                MOCK_METHOD1(UserSettingsUpdated, void(UKControllerPlugin::Euroscope::UserSetting &));
        };
    }  // namespace Euroscope
}  // namespace UKControllerPluginTest
