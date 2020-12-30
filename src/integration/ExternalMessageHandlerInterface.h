#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin {
    namespace Integration {
        /*
            Interfaces for classes that want to process messages from external sources.
        */
        class ExternalMessageHandlerInterface
        {
            public:

                /*
                    Process a message, return true if processed (propogration should stop)
                    or false otherwise.
                */
                virtual bool ProcessMessage(std::string message) = 0;
        };
    }  // namespace Integration
}  // namespace UKControllerPlugin
