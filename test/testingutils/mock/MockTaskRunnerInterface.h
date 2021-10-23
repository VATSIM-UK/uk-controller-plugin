#pragma once
#include "task/TaskRunnerInterface.h"

namespace UKControllerPluginTest::TaskManager {
    class MockTaskRunnerInterface : public UKControllerPlugin::TaskManager::TaskRunnerInterface
    {
        public:
        explicit MockTaskRunnerInterface(bool runTask = true)
            : runTask(runTask){

              };

        [[nodiscard]] auto CountThreads() const -> size_t override
        {
            return 0;
        }

        /*
            Run the task only if required.
        */
        void QueueAsynchronousTask(std::function<void()> callback) override
        {
            if (this->runTask) {
                callback();
            };
        };

        private:
        // Whether we actually want to run the task.
        bool runTask;
    };
} // namespace UKControllerPluginTest::TaskManager
