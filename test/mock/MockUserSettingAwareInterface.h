#pragma once
#include "pch/pch.h"
#include "euroscope/UserSettingAwareInterface.h"
#include "euroscope/UserSetting.h"

namespace UKControllerPluginTest {
    namespace Euroscope {

        class MockUserSettingAwareInterface : public UKControllerPlugin::Euroscope::UserSettingAwareInterface
        {
            public:
                MOCK_METHOD1(UserSettingsUpdated, void(UKControllerPlugin::Euroscope::UserSetting &));
        };
    }  // namespace RadarScreen
}  // namespace UKControllerPluginTest
