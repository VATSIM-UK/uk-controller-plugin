#pragma once

namespace UKControllerPlugin::List {
    /**
     * Interface for a class wrapping EuroScopes popup list.
     */
    class PopupListInterface
    {
        public:
        virtual ~PopupListInterface() = default;
        virtual void Trigger(const POINT& location) = 0;
    };
} // namespace UKControllerPlugin::List
