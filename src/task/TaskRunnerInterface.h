#pragma once

namespace UKControllerPlugin {
    namespace TaskManager {

        /*
            A public interface for a class that runs tasks on separate threads, usually
            to avoid long HTTP calls stalling the main EuroScope thread. This interface delibarately
            avoids the setup and teardown methods - so that the task runner can not be shut down
            outside the setup and teardown code.
        */
        class TaskRunnerInterface
        {
            public:
                virtual void QueueAsynchronousTask(std::function<void(void)> task) = 0;
        };
    }  // namespace TaskManager
}  // namespace UKControllerPlugin
