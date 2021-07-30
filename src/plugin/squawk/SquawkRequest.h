#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin {
    namespace Squawk {

        /*
            Class for providing a thread-safe way of checking whether or not
            a squawk request is in progress.
        */
        class SquawkRequest
        {
            public:

                 bool Start(std::string callsign);
                 void End(std::string callsign);

            private:

                // Mutex that gets locked when requests are being processed.
                std::mutex requestlock;

                // Requests that are in progress.
                std::unordered_set<std::string> requests;
        };
    }  // namespace Squawk
}  // namespace UKControllerPlugin
