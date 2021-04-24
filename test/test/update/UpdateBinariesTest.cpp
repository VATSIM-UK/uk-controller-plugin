#include "pch/utilstestpch.h"
#include "api/ApiException.h"
#include "update/UpdateBinaries.h"
#include "mock/MockWinApi.h"
#include "mock/MockApiInterface.h"
#include "mock/MockCurlApi.h"
#include "curl/CurlRequest.h"
#include "curl/CurlResponse.h"

using testing::Test;
using testing::NiceMock;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Curl::MockCurlApi;
using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Curl::CurlResponse;

namespace UKControllerPluginUtilsTest {
    namespace Update {
        class UpdateBinariesTest : public Test
        {
            public:
                NiceMock<MockWinApi> mockWindows;
                NiceMock<MockApiInterface> mockApi;
                NiceMock<MockCurlApi> mockCurl;
        };

        TEST_F(UpdateBinariesTest, UpdateDataIsValidIfAllPresent)
        {
            nlohmann::json updateData{
                {"version", "3.0.0"},
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            EXPECT_TRUE(UKControllerPlugin::UpdateDataValid(updateData));
        }

        TEST_F(UpdateBinariesTest, UpdateDataIsNotValidIfVersionMissing)
        {
            nlohmann::json updateData{
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            EXPECT_FALSE(UKControllerPlugin::UpdateDataValid(updateData));
        }

        TEST_F(UpdateBinariesTest, UpdateDataIsNotValidIfVersionNotString)
        {
            nlohmann::json updateData{
                {"version", 123},
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            EXPECT_FALSE(UKControllerPlugin::UpdateDataValid(updateData));
        }

        TEST_F(UpdateBinariesTest, UpdateDataIsNotValidIfUpdaterUrlMissing)
        {
            nlohmann::json updateData{
                {"version", "3.0.0"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            EXPECT_FALSE(UKControllerPlugin::UpdateDataValid(updateData));
        }

        TEST_F(UpdateBinariesTest, UpdateDataIsNotValidIfUpdaterUrlNotString)
        {
            nlohmann::json updateData{
                {"version", "3.0.0"},
                {"updater_download_url", 123},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            EXPECT_FALSE(UKControllerPlugin::UpdateDataValid(updateData));
        }

        TEST_F(UpdateBinariesTest, UpdateDataIsNotValidIfCoreUrlMissing)
        {
            nlohmann::json updateData{
                {"version", "3.0.0"},
                {"updater_download_url", "foo"},
                {"loader_download_url", "baz"},
            };

            EXPECT_FALSE(UKControllerPlugin::UpdateDataValid(updateData));
        }

        TEST_F(UpdateBinariesTest, UpdateDataIsNotValidIfCoreUrlNotString)
        {
            nlohmann::json updateData{
                {"version", "3.0.0"},
                {"updater_download_url", "foo"},
                {"core_download_url", 123},
                {"loader_download_url", "baz"},
            };

            EXPECT_FALSE(UKControllerPlugin::UpdateDataValid(updateData));
        }

        TEST_F(UpdateBinariesTest, UpdateDataIsNotValidIfLoaderUrlMissing)
        {
            nlohmann::json updateData{
                {"version", "3.0.0"},
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
            };

            EXPECT_FALSE(UKControllerPlugin::UpdateDataValid(updateData));
        }

        TEST_F(UpdateBinariesTest, UpdateDataIsNotValidIfCoreLoaderUrlNotString)
        {
            nlohmann::json updateData{
                {"version", "3.0.0"},
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", 123},
            };

            EXPECT_FALSE(UKControllerPlugin::UpdateDataValid(updateData));
        }

        TEST_F(UpdateBinariesTest, GetUpdateDataReturnsUpdateData)
        {
            nlohmann::json updateData{
                {"version", "3.0.0"},
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            ON_CALL(this->mockApi, GetUpdateDetails())
                .WillByDefault(testing::Return(updateData));

            EXPECT_EQ(updateData, UKControllerPlugin::GetUpdateData(this->mockApi));
        }

        TEST_F(UpdateBinariesTest, GetUpdateDataThrowsExceptionIfUpdateDataInvalid)
        {
            nlohmann::json updateData{
                {"version", 1234}, // Invalid
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            ON_CALL(this->mockApi, GetUpdateDetails())
                .WillByDefault(testing::Return(updateData));

            EXPECT_THROW(UKControllerPlugin::GetUpdateData(this->mockApi), std::exception);
        }

        TEST_F(UpdateBinariesTest, GetUpdateDataThrowsApiExceptionIfThrownByApi)
        {
            nlohmann::json updateData{
                {"version", 1234}, // Invalid
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            ON_CALL(this->mockApi, GetUpdateDetails())
                .WillByDefault(testing::Throw(UKControllerPlugin::Api::ApiException("foo")));

            EXPECT_THROW(UKControllerPlugin::GetUpdateData(this->mockApi), UKControllerPlugin::Api::ApiException);
        }

        TEST_F(UpdateBinariesTest, ItUpdatesTheCoreLibrary)
        {
            nlohmann::json updateData{
                {"version", "3.0.1"},
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            ON_CALL(this->mockApi, GetUpdateDetails())
                .WillByDefault(testing::Return(updateData));

            CurlRequest expectedRequest(
                "bar",
                CurlRequest::METHOD_GET
            );
            expectedRequest.SetMaxRequestTime(0);

            CurlResponse response("3.0.1.core", false, 200);

            EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedRequest))
                .Times(1)
                .WillOnce(testing::Return(response));


            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"bin/UKControllerPluginCore.dll"), "3.0.1.core", true)
            )
                .Times(1);

            EXPECT_TRUE(UKControllerPlugin::DownloadCoreLibrary(updateData, this->mockWindows, this->mockCurl));
        }

        TEST_F(UpdateBinariesTest, ItHandlesCurlErrorsUpdatingTheCoreLibrary)
        {
            nlohmann::json updateData{
                {"version", "3.0.1"},
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            ON_CALL(this->mockApi, GetUpdateDetails())
                .WillByDefault(testing::Return(updateData));

            CurlRequest expectedRequest(
                "bar",
                CurlRequest::METHOD_GET
            );
            expectedRequest.SetMaxRequestTime(0);

            CurlResponse response("3.0.1.core", true, 200);

            EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedRequest))
                .Times(1)
                .WillOnce(testing::Return(response));


            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"bin/UKControllerPluginCore.dll"), "3.0.1.core", true)
            )
                .Times(0);

            EXPECT_FALSE(UKControllerPlugin::DownloadCoreLibrary(updateData, this->mockWindows, this->mockCurl));
        }

        TEST_F(UpdateBinariesTest, ItHandlesHttpErrorsUpdatingTheCoreLibrary)
        {
            nlohmann::json updateData{
                {"version", "3.0.1"},
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            ON_CALL(this->mockApi, GetUpdateDetails())
                .WillByDefault(testing::Return(updateData));

            CurlRequest expectedRequest(
                "bar",
                CurlRequest::METHOD_GET
            );
            expectedRequest.SetMaxRequestTime(0);

            CurlResponse response("3.0.1.core", false, 500);

            EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedRequest))
                .Times(1)
                .WillOnce(testing::Return(response));


            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"bin/UKControllerPluginCore.dll"), "3.0.1.core", true)
            )
                .Times(0);

            EXPECT_FALSE(UKControllerPlugin::DownloadCoreLibrary(updateData, this->mockWindows, this->mockCurl));
        }

        TEST_F(UpdateBinariesTest, ItUpdatesTheUpdaterLibrary)
        {
            nlohmann::json updateData{
                {"version", "3.0.1"},
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            ON_CALL(this->mockApi, GetUpdateDetails())
                .WillByDefault(testing::Return(updateData));

            CurlRequest expectedRequest(
                "foo",
                CurlRequest::METHOD_GET
            );
            expectedRequest.SetMaxRequestTime(0);

            CurlResponse response("3.0.1.updater", false, 200);

            EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedRequest))
                .Times(1)
                .WillOnce(testing::Return(response));


            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"bin/UKControllerPluginUpdater.dll"), "3.0.1.updater", true)
            )
                .Times(1);

            EXPECT_TRUE(UKControllerPlugin::DownloadUpdater(updateData, this->mockWindows, this->mockCurl));
        }

        TEST_F(UpdateBinariesTest, ItHandlesCurlErrorsUpdatingTheUpdaterLibrary)
        {
            nlohmann::json updateData{
                {"version", "3.0.1"},
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            ON_CALL(this->mockApi, GetUpdateDetails())
                .WillByDefault(testing::Return(updateData));

            CurlRequest expectedRequest(
                "foo",
                CurlRequest::METHOD_GET
            );
            expectedRequest.SetMaxRequestTime(0);

            CurlResponse response("3.0.1.updater", true, 200);

            EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedRequest))
                .Times(1)
                .WillOnce(testing::Return(response));


            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"bin/UKControllerPluginCore.dll"), "3.0.1.updater", true)
            )
                .Times(0);

            EXPECT_FALSE(UKControllerPlugin::DownloadUpdater(updateData, this->mockWindows, this->mockCurl));
        }

        TEST_F(UpdateBinariesTest, ItHandlesHttpErrorsUpdatingTheUpdaterLibrary)
        {
            nlohmann::json updateData{
                {"version", "3.0.1"},
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            ON_CALL(this->mockApi, GetUpdateDetails())
                .WillByDefault(testing::Return(updateData));

            CurlRequest expectedRequest(
                "foo",
                CurlRequest::METHOD_GET
            );
            expectedRequest.SetMaxRequestTime(0);

            CurlResponse response("3.0.1.updater", false, 500);

            EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedRequest))
                .Times(1)
                .WillOnce(testing::Return(response));


            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"bin/UKControllerPluginCore.dll"), "3.0.1.updater", true)
            )
                .Times(0);

            EXPECT_FALSE(UKControllerPlugin::DownloadUpdater(updateData, this->mockWindows, this->mockCurl));
        }
    } // namespace Updater
} // namespace UKControllerPluginUpdaterTest
