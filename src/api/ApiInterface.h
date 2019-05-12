#pragma once
#include "api/RemoteFileManifest.h"
#include "squawk/ApiSquawkAllocation.h"
#include "dependency/DependencyData.h"

namespace UKControllerPlugin {
    namespace Api {

        /**
         * An abstract class for the web API.
         */
        class ApiInterface
        {
            public:
                virtual UKControllerPlugin::Squawk::ApiSquawkAllocation CreateGeneralSquawkAssignment(
                    std::string callsign,
                    std::string origin,
                    std::string destination
                ) const = 0;
                virtual UKControllerPlugin::Squawk::ApiSquawkAllocation CreateLocalSquawkAssignment(
                    std::string callsign,
                    std::string unit,
                    std::string flightRules
                ) const = 0;
                virtual bool CheckApiAuthorisation(void) const = 0;
                virtual void DeleteSquawkAssignment(std::string callsign) const = 0;
                virtual UKControllerPlugin::Api::RemoteFileManifest FetchDependencyManifest(void) const = 0;
                virtual std::string FetchRemoteFile(std::string uri) const = 0;
                virtual UKControllerPlugin::Squawk::ApiSquawkAllocation GetAssignedSquawk(
                    std::string callsign
                ) const = 0;
                virtual std::string GetApiDomain(void) const = 0;
                virtual std::string GetApiKey(void) const = 0;
                virtual nlohmann::json GetHoldDependency(void) const = 0;
                virtual nlohmann::json GetGenericHoldProfiles(void) const = 0;
                virtual nlohmann::json GetUserHoldProfiles(void) const = 0;
                virtual nlohmann::json GetDependency(
                    UKControllerPlugin::Dependency::DependencyData dependency
                ) const = 0;
                virtual void DeleteUserHoldProfile(unsigned int profileId) const = 0;
                virtual unsigned int CreateUserHoldProfile(std::string name, std::set<unsigned int> holds) const = 0;
                virtual void UpdateUserHoldProfile(
                    unsigned int id,
                    std::string name,
                    std::set<unsigned int> holds
                ) const = 0;
                virtual int UpdateCheck(std::string version) const = 0;
                virtual void SetApiKey(std::string key) = 0;
                virtual void SetApiDomain(std::string domain) = 0;

                // Codes returned after an update check
                static const int UPDATE_UP_TO_DATE = 0;
                static const int UPDATE_VERSION_DISABLED = 1;
                static const int UPDATE_VERSION_NEEDS_UPDATE = 2;
        };
    }  // namespace Api
}  // namespace UKControllerPlugin
