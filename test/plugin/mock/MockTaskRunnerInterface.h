#pragma once
#include "task/TaskRunnerInterface.h"

namespace UKControllerPluginTest {
    namespace TaskManager {
        class MockTaskRunnerInterface : public UKControllerPlugin::TaskManager::TaskRunnerInterface
        {
            public:

                explicit MockTaskRunnerInterface(bool runTask = true)
                {
                    this->runTask = runTask;
                };

                size_t CountThreads(void) const override
                {
                    return 0;
                }

                /*
                    Run the task only if required.
                */
                void QueueAsynchronousTask(std::function<void()> callback)
                {
                    if (this->runTask) callback();
                };

            private:
                // Whether we actually want to run the task.
                bool runTask;
        };
    }  // namespace TaskManager
}  // namespace UKControllerPluginTest
