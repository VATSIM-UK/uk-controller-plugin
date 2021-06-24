#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin {
    namespace Push {

        /*
            Class that represents a connection to the web API that receives "push" events.
            This may be achieved either via direct websocket connections, or regular polling.
        */
        class PushEventConnectionInterface
        {
            public:
                virtual ~PushEventConnectionInterface(void) {}
                virtual void WriteMessage(std::string message) = 0;
                virtual std::string GetNextMessage(void) = 0;
                virtual void SetIdleTimeout(std::chrono::seconds timeout) = 0;

                // Returned when there are no messages in the queue to be processed
                const std::string noMessage = "";
        };
    } // namespace Push
}  // namespace UKControllerPlugin
