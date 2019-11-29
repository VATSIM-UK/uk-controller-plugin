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
                MOCK_CONST_METHOD2(AuthoriseWebsocketChannel, std::string(std::string socketId, std::string channel));
                MOCK_CONST_METHOD0(CheckApiAuthorisation, bool(void));
                MOCK_CONST_METHOD1(DeleteSquawkAssignment, void(std::string));
                MOCK_CONST_METHOD0(FetchDependencyManifest, UKControllerPlugin::Api::RemoteFileManifest(void));
                MOCK_CONST_METHOD0(GetDependencyList, nlohmann::json(void));
                MOCK_CONST_METHOD1(FetchRemoteFile, std::string(std::string));
                MOCK_CONST_METHOD1(GetAssignedSquawk, UKControllerPlugin::Squawk::ApiSquawkAllocation(std::string));
                MOCK_CONST_METHOD0(GetApiDomain, std::string(void));
                MOCK_CONST_METHOD0(GetApiKey, std::string(void));
                MOCK_CONST_METHOD0(GetHoldDependency, nlohmann::json(void));
                MOCK_CONST_METHOD0(GetGenericHoldProfiles, nlohmann::json(void));
                MOCK_CONST_METHOD0(GetMinStackLevels, nlohmann::json(void));
                MOCK_CONST_METHOD0(GetUserHoldProfiles, nlohmann::json(void));
                MOCK_CONST_METHOD1(GetDependency, nlohmann::json(UKControllerPlugin::Dependency::DependencyData));
                MOCK_CONST_METHOD1(GetUri, nlohmann::json(std::string uri));
                MOCK_CONST_METHOD1(DeleteUserHoldProfile, void(unsigned int profileId));
                MOCK_CONST_METHOD2(
                    CreateUserHoldProfile, unsigned int(std::string name, std::set<unsigned int> holds)
                );
                MOCK_CONST_METHOD3(
                    UpdateUserHoldProfile,
                    void(unsigned int id, std::string name, std::set<unsigned int> holds)
                );
                MOCK_CONST_METHOD1(UpdateCheck, int(std::string));
                MOCK_METHOD1(SetApiDomain, void(std::string));
                MOCK_METHOD1(SetApiKey, void(std::string));
        };
    }  // namespace Api
}  // namespace UKControllerPluginTest
