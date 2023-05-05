#pragma once
#include "api/ApiSettingsProviderInterface.h"

using UKControllerPluginUtils::Api::ApiSettings;
using UKControllerPluginUtils::Api::ApiSettingsProviderInterface;

namespace UKControllerPluginUtilsTest::Api {
    class MockApiSettingsProvider : public ApiSettingsProviderInterface
    {
        public:
        MockApiSettingsProvider();
        virtual ~MockApiSettingsProvider();
        MOCK_METHOD(ApiSettings&, Get, (), (override));
        MOCK_METHOD(bool, Has, (), (override));
    };
} // namespace UKControllerPluginUtilsTest::Api
