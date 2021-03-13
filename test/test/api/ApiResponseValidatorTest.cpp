#include "pch/utilstestpch.h"
#include "api/ApiResponseValidator.h"

using UKControllerPlugin::Api::ApiResponseValidator;

namespace UKControllerPluginUtilsTest {
    namespace Api {

        TEST(ApiResponseValidator, ValidateSchemaFailsDataNotObject)
        {
            EXPECT_FALSE(ApiResponseValidator::ValidateSchema("test", { { "test", -999 } }));
        }

        TEST(ApiResponseValidator, ValidateSchemaFailsIfUnknownType)
        {
            EXPECT_FALSE(ApiResponseValidator::ValidateSchema("{\"test\": true}"_json, { {"test", -999} }));
        }

        TEST(ApiResponseValidator, ValidateSchemaFailsIfBooleanNoMatch)
        {
            EXPECT_FALSE(
                ApiResponseValidator::ValidateSchema(
                    "{\"test\": 12}"_json, { { "test", ApiResponseValidator::boolean } }
                )
            );
        }

        TEST(ApiResponseValidator, ValidateSchemaFailsIfObjectNoMatch)
        {
            EXPECT_FALSE(
                ApiResponseValidator::ValidateSchema(
                    "{\"test\": 12}"_json, { { "test", ApiResponseValidator::object } }
                )
            );
        }

        TEST(ApiResponseValidator, ValidateSchemaFailsIfFloatNoMatch)
        {
            EXPECT_FALSE(
                ApiResponseValidator::ValidateSchema(
                    "{\"test\": 12}"_json, { { "test", ApiResponseValidator::floatingpoint } }
                )
            );
        }

        TEST(ApiResponseValidator, ValidateSchemaFailsIfStringNoMatch)
        {
            EXPECT_FALSE(
                ApiResponseValidator::ValidateSchema(
                    "{\"test\": 12}"_json, { { "test", ApiResponseValidator::string } }
                )
            );
        }

        TEST(ApiResponseValidator, ValidateSchemaFailsIfIntegerNoMatch)
        {
            EXPECT_FALSE(
                ApiResponseValidator::ValidateSchema(
                    "{\"test\": false}"_json, { { "test", ApiResponseValidator::integer } }
                )
            );
        }

        TEST(ApiResponseValidator, ValidateSchemaPassesIfMatch)
        {
            EXPECT_TRUE(
                ApiResponseValidator::ValidateSchema(
                    "{\"test\": false}"_json, { { "test", ApiResponseValidator::boolean } }
                )
            );
        }

        TEST(ApiResponseValidator, ValidateSchemaHandlesMultipleFields)
        {
            EXPECT_TRUE(
                ApiResponseValidator::ValidateSchema(
                    "{\"test\": false, \"test2\": 1234}"_json, { { "test",
                    ApiResponseValidator::boolean },
                    { "test2", ApiResponseValidator::integer } }
                )
            );
        }

        TEST(ApiResponseValidator, ValidateSchemaCanHandleMissingKeys)
        {
            EXPECT_FALSE(
                ApiResponseValidator::ValidateSchema(
                    "{\"test\": false, \"test2\": 1234}"_json, { { "test",
                    ApiResponseValidator::boolean },
                    { "test2", ApiResponseValidator::integer },
                    {"missing", ApiResponseValidator::integer } }
                )
            );
        }
    }  // namespace Api
}  // namespace UKControllerPluginUtilsTest
