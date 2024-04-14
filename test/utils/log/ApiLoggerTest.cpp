#include "log/ApiLogger.h"
#include "test/ApiTestCase.h"
#include "update/PluginVersion.h"

namespace UKControllerPluginUtilsTest::Api {
    class ApiLoggerTest : public UKControllerPluginTest::ApiTestCase
    {
        public:
        ApiLoggerTest() : ApiTestCase()
        {
        }

        const nlohmann::json expectedPluginVersionMetadata = {
            {"plugin_version", UKControllerPlugin::Plugin::PluginVersion::version}};

        UKControllerPluginUtils::Log::ApiLogger logger;
    };

    TEST_F(ApiLoggerTest, ItLogsSync)
    {
        const nlohmann::json expectedPayload = {
            {"type", "type"}, {"message", "message"}, {"metadata", expectedPluginVersionMetadata.dump()}};

        this->ExpectApiRequest()->Post().To("plugin/logs").WithBody(expectedPayload).WillReturnCreated();
        logger.Log("type", "message");
    }

    TEST_F(ApiLoggerTest, ItLogsSyncWithMetadata)
    {
        nlohmann::json metadata = {{"key", "value"}};
        metadata.update(expectedPluginVersionMetadata);
        const nlohmann::json expectedPayload = {
            {"type", "type"}, {"message", "message"}, {"metadata", metadata.dump()}};

        this->ExpectApiRequest()->Post().To("plugin/logs").WithBody(expectedPayload).WillReturnCreated();
        logger.Log("type", "message", metadata);
    }

    TEST_F(ApiLoggerTest, ItLogsAsync)
    {
        const nlohmann::json expectedPayload = {
            {"type", "type"}, {"message", "message"}, {"metadata", expectedPluginVersionMetadata.dump()}};

        this->ExpectApiRequest()->Post().To("plugin/logs").WithBody(expectedPayload).WillReturnCreated();
        logger.LogAsync("type", "message");
    }

    TEST_F(ApiLoggerTest, ItLogsAsyncWithMetadata)
    {
        nlohmann::json metadata = {{"key", "value"}};
        metadata.update(expectedPluginVersionMetadata);
        const nlohmann::json expectedPayload = {
            {"type", "type"}, {"message", "message"}, {"metadata", metadata.dump()}};

        this->ExpectApiRequest()->Post().To("plugin/logs").WithBody(expectedPayload).WillReturnCreated();
        logger.LogAsync("type", "message", metadata);
    }
} // namespace UKControllerPluginUtilsTest::Api
