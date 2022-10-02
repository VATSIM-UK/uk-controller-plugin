#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            Represents a function that EuroScope will call through its
            OnFunctionCall interface.
        */
        typedef struct CallbackFunction
        {
            CallbackFunction(
                int functionId, std::string description, std::function<void(int, std::string, RECT)> function);
            void operator=(const CallbackFunction& copy);
            bool operator==(const CallbackFunction& compare) const;

            int functionId;
            std::string description;
            std::function<void(int, std::string, RECT)> function;
        } CallbackFunction;
    } // namespace Euroscope
} // namespace UKControllerPlugin
