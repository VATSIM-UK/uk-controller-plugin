#pragma once
#include "euroscope/PluginSettingsProviderInterface.h"
#include "euroscope/UserSetting.h"

using UKControllerPlugin::Euroscope::PluginSettingsProviderInterface;
using UKControllerPlugin::Euroscope::UserSetting;

namespace UKControllerPluginTest::Euroscope {
    class MockPluginSettingsProvider : public PluginSettingsProviderInterface
    {
        public:
            MOCK_METHOD(void, LoadSettings, (UserSetting&), (override));
            MOCK_METHOD(void, SaveSettings, (UserSetting&), (override));
    };
} // namespace UKControllerPluginTest::Euroscope
