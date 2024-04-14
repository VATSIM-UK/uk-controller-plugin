#include "log/ApiLogger.h"
#include "test/ApiTestCase.h"

namespace UKControllerPluginUtilsTest::Api {
    class ApiLoggerTest : public UKControllerPluginTest::ApiTestCase
    {
        public:
        ApiLoggerTest() : ApiTestCase()
        {
        }

        UKControllerPluginUtils::Log::ApiLogger logger;
    };

    TEST_F(ApiLoggerTest, ItLogsSync)
    {
        const nlohmann::json expectedPayload = {
            {"type", "type"}, {"message", "message"}, {"plugin_version", "#VERSION_STRING#"}};

        this->ExpectApiRequest()->Post().To("plugin/logs").WithBody(expectedPayload).WillReturnCreated();
        logger.Log("type", "message");
    }

    TEST_F(ApiLoggerTest, ItLogsSyncWithMetadata)
    {
        const nlohmann::json metadata = {{"key", "value"}};
        const nlohmann::json expectedPayload = {
            {"type", "type"},
            {"message", "message"},
            {"metadata", metadata.dump()},
            {"plugin_version", "#VERSION_STRING#"}};

        this->ExpectApiRequest()->Post().To("plugin/logs").WithBody(expectedPayload).WillReturnCreated();
        logger.Log("type", "message", metadata);
    }

    TEST_F(ApiLoggerTest, ItLogsAsync)
    {
        const nlohmann::json expectedPayload = {
            {"type", "type"}, {"message", "message"}, {"plugin_version", "#VERSION_STRING#"}};

        this->ExpectApiRequest()->Post().To("plugin/logs").WithBody(expectedPayload).WillReturnCreated();
        logger.LogAsync("type", "message");
    }

    TEST_F(ApiLoggerTest, ItLogsAsyncWithMetadata)
    {
        const nlohmann::json metadata = {{"key", "value"}};
        const nlohmann::json expectedPayload = {
            {"type", "type"},
            {"message", "message"},
            {"metadata", metadata.dump()},
            {"plugin_version", "#VERSION_STRING#"}};

        this->ExpectApiRequest()->Post().To("plugin/logs").WithBody(expectedPayload).WillReturnCreated();
        logger.LogAsync("type", "message", metadata);
    }
} // namespace UKControllerPluginUtilsTest::Api
