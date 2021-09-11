#pragma once

namespace UKControllerPlugin::Push {

    /*
        Class that represents a connection to the web API that receives "push" events.
        This may be achieved either via direct websocket connections, or regular polling.
    */
    class PushEventConnectionInterface
    {
        public:
        PushEventConnectionInterface();
        virtual ~PushEventConnectionInterface();
        PushEventConnectionInterface(const PushEventConnectionInterface&);
        PushEventConnectionInterface(PushEventConnectionInterface&&) noexcept;
        [[nodiscard]] auto operator=(const PushEventConnectionInterface&) -> PushEventConnectionInterface& = delete;
        [[nodiscard]] auto operator=(PushEventConnectionInterface&&) noexcept -> PushEventConnectionInterface& = delete;
        virtual void WriteMessage(std::string message) = 0;
        [[nodiscard]] virtual auto GetNextMessage() -> std::string = 0;

        // Returned when there are no messages in the queue to be processed
        const std::string noMessage;
    };
} // namespace UKControllerPlugin::Push
