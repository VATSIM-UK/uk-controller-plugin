#include "pch/pch.h"
#include "squawk/SquawkRequest.h"

namespace UKControllerPlugin {
    namespace Squawk {

        /*
            Start the squawk request. Return true if ok to proceed, false if
            one is already in progress.
        */
        bool SquawkRequest::Start(std::string callsign)
        {
            std::lock_guard<std::mutex> lock(this->requestlock);
            return this->requests.insert(callsign).second;
        }

        /*
            Finish the squawk request.
        */
        void SquawkRequest::End(std::string callsign)
        {
            std::lock_guard<std::mutex> lock(this->requestlock);
            this->requests.erase(callsign);
        }
    }  // namespace Squawk
}  // namespace UKControllerPlugin
