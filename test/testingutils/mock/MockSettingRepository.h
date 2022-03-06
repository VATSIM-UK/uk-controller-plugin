#include "setting/SettingRepositoryInterface.h"

using UKControllerPlugin::Setting::SettingRepositoryInterface;

namespace UKControllerPluginTest::Setting {
    class MockSettingRepository : public SettingRepositoryInterface
    {
        public:
        MOCK_METHOD(bool, HasSetting, (const std::string&), (const, override));
        MOCK_METHOD(void, ReloadSetting, (const std::string&), (override));
        MOCK_METHOD(void, UpdateSetting, (const std::string&, const std::string&), (override));
        MOCK_METHOD(std::string, GetSetting, (const std::string&, const std::string&), (const, override));
    };
} // namespace UKControllerPluginTest::Setting
