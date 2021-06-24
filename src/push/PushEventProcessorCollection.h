#pragma once

namespace UKControllerPlugin {
    namespace Push {

        class PushEventProcessorInterface;
        struct PushEvent;

        /*
            An interface to be implemented by classes that
            want to process events pushed from the API.
        */
        class PushEventProcessorCollection
        {
            public:

                void AddProcessor(
                    std::shared_ptr<PushEventProcessorInterface> processor
                );
                size_t CountProcessorsForChannel(std::string event) const;
                size_t CountProcessorsForEvent(std::string event) const;
                size_t CountProcessorsForAll() const;
                std::set<std::string> GetChannelSubscriptions(void) const;
                void ProcessEvent(const PushEvent& message) const;

            private:

                std::map<
                    std::string,
                    std::set<std::shared_ptr<PushEventProcessorInterface>>
                > channelMap;

                // Maps protocol events to their processors
                std::map<
                    std::string,
                    std::set<std::shared_ptr<PushEventProcessorInterface>>
                > eventMap;

                // Processors that care about every event
                std::set<std::shared_ptr<PushEventProcessorInterface>> allEventProcessors;
        };
    } // namespace Push
}  // namespace UKControllerPlugin
