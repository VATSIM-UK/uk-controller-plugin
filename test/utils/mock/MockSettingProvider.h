#pragma once
#include "setting/SettingProviderInterface.h"

using UKControllerPlugin::Setting::SettingProviderInterface;

namespace UKControllerPluginTest::Setting {
    class MockSettingProvider : public SettingProviderInterface
    {
        public:
        MockSettingProvider();
        virtual ~MockSettingProvider();
        MOCK_METHOD(std::string, Get, (const std::string&), (override));
        MOCK_METHOD(void, Save, (const std::string&, const std::string&), (override));
        MOCK_METHOD(const std::set<std::string>&, Provides, (), (override));
        MOCK_METHOD(void, Reload, (), (override));
    };
} // namespace UKControllerPluginTest::Setting
