#pragma once

namespace UKControllerPlugin::Controller {
    class ActiveCallsign;

    /*
        An interface to be implemented by classes that want to know
        when Active Callsigns come and go.
    */
    class ActiveCallsignEventHandlerInterface
    {
        public:
        ActiveCallsignEventHandlerInterface();
        virtual ~ActiveCallsignEventHandlerInterface();
        ActiveCallsignEventHandlerInterface(const ActiveCallsignEventHandlerInterface&);
        ActiveCallsignEventHandlerInterface(ActiveCallsignEventHandlerInterface&&) noexcept;
        [[nodiscard]] auto operator=(const ActiveCallsignEventHandlerInterface&)
            -> ActiveCallsignEventHandlerInterface&;
        [[nodiscard]] auto operator=(ActiveCallsignEventHandlerInterface&&) noexcept
            -> ActiveCallsignEventHandlerInterface&;

        virtual void ActiveCallsignAdded(const UKControllerPlugin::Controller::ActiveCallsign& callsign) = 0;
        virtual void ActiveCallsignRemoved(const UKControllerPlugin::Controller::ActiveCallsign& callsign) = 0;
        virtual void CallsignsFlushed();
    };
} // namespace UKControllerPlugin::Controller
