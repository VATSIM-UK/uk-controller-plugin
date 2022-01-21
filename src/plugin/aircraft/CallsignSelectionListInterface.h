#pragma once

namespace UKControllerPlugin::Aircraft {
    /**
     * An interface for classes that trigger a callsign
     * selection list.
     */
    class CallsignSelectionListInterface
    {
        public:
        virtual ~CallsignSelectionListInterface() = default;
        virtual void TriggerList(const POINT& location) = 0;
    };
} // namespace UKControllerPlugin::Aircraft
