#pragma once
#include "api/ApiRequestPerformerInterface.h"

using UKControllerPluginUtils::Api::ApiRequestData;
using UKControllerPluginUtils::Api::ApiRequestPerformerInterface;
using UKControllerPluginUtils::Api::Response;

namespace UKControllerPluginUtilsTest::Api {
    class MockApiRequestPerformer : public ApiRequestPerformerInterface
    {
        public:
        MockApiRequestPerformer();
        virtual ~MockApiRequestPerformer();
        MOCK_METHOD(Response, Perform, (const ApiRequestData&), (override));
    };
} // namespace UKControllerPluginUtilsTest::Api
