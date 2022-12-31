#pragma once

namespace UKControllerPlugin::IntentionCode {

    class AircraftIntentionCode;

    /**
     * Interface for classes that want to know about intention code updates
     */
    class IntentionCodeEventHandlerInterface
    {
        public:
        virtual ~IntentionCodeEventHandlerInterface() = default;
        /**
         * Will be called when an intention code is updated.
         */
        virtual void IntentionCodeUpdated(const AircraftIntentionCode& intentionCode) = 0;
    };
} // namespace UKControllerPlugin::IntentionCode
