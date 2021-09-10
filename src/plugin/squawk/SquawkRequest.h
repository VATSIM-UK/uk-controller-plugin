#pragma once

namespace UKControllerPlugin::Squawk {

    /*
        Class for providing a thread-safe way of checking whether or not
        a squawk request is in progress.
    */
    class SquawkRequest
    {
        public:
        [[nodiscard]] auto Start(std::string callsign) -> bool;
        void End(std::string callsign);

        private:
        // Mutex that gets locked when requests are being processed.
        std::mutex requestlock;

        // Requests that are in progress.
        std::unordered_set<std::string> requests;
    };
} // namespace UKControllerPlugin::Squawk
