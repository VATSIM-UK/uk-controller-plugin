#pragma once
#include "api/RemoteFileManifest.h"

namespace UKControllerPlugin {
    namespace Api {

        /**
         * An abstract class for the web API.
         */
        class ApiInterface
        {
            public:
                virtual std::string CreateGeneralSquawkAssignment(
                    std::string callsign,
                    std::string origin,
                    std::string destination
                ) const = 0;
                virtual std::string CreateLocalSquawkAssignment(
                    std::string callsign,
                    std::string unit,
                    std::string flightRules
                ) const = 0;
                virtual bool CheckApiAuthorisation(void) const = 0;
                virtual void DeleteSquawkAssignment(std::string callsign) const = 0;
                virtual UKControllerPlugin::Api::RemoteFileManifest FetchDependencyManifest(void) const = 0;
                virtual std::string FetchRemoteFile(std::string uri) const = 0;
                virtual std::string GetAssignedSquawk(std::string callsign) const = 0;
                virtual std::string GetApiDomain(void) const = 0;
                virtual std::string GetApiKey(void) const = 0;
                virtual int UpdateCheck(std::string version) const = 0;

                // Codes returned after an update check
                static const int UPDATE_UP_TO_DATE = 0;
                static const int UPDATE_VERSION_DISABLED = 1;
                static const int UPDATE_VERSION_NEEDS_UPDATE = 2;
        };
    }  // namespace Api
}  // namespace UKControllerPlugin
