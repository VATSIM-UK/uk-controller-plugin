#pragma once
#include "pch/pch.h"
#include "api/ApiInterface.h"

namespace UKControllerPluginTest {
    namespace Api {

        class MockApiInterface : public UKControllerPlugin::Api::ApiInterface
        {
            public:
                MOCK_CONST_METHOD3(
                    CreateGeneralSquawkAssignment,
                    UKControllerPlugin::Squawk::ApiSquawkAllocation (
                        std::string,
                        std::string,
                        std::string
                    )
                );
                MOCK_CONST_METHOD3(
                    CreateLocalSquawkAssignment,
                    UKControllerPlugin::Squawk::ApiSquawkAllocation (
                        std::string,
                        std::string,
                        std::string
                    )
                );
                MOCK_CONST_METHOD0(CheckApiAuthorisation, bool(void));
                MOCK_CONST_METHOD1(DeleteSquawkAssignment, void(std::string));
                MOCK_CONST_METHOD0(FetchDependencyManifest, UKControllerPlugin::Api::RemoteFileManifest(void));
                MOCK_CONST_METHOD1(FetchRemoteFile, std::string(std::string));
                MOCK_CONST_METHOD1(GetAssignedSquawk, UKControllerPlugin::Squawk::ApiSquawkAllocation(std::string));
                MOCK_CONST_METHOD0(GetApiDomain, std::string(void));
                MOCK_CONST_METHOD0(GetApiKey, std::string(void));
                MOCK_CONST_METHOD0(GetHoldDependency, nlohmann::json(void));
                MOCK_CONST_METHOD1(UpdateCheck, int(std::string));
        };
    }  // namespace Api
}  // namespace UKControllerPluginTest
