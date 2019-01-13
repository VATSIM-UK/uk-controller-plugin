#pragma once
#include "task/TaskRunnerInterface.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/null_sink.h"

namespace UKControllerPluginTest {
    namespace TaskManager {
        class MockTaskRunnerInterface : public UKControllerPlugin::TaskManager::TaskRunnerInterface
        {
            public:

                explicit MockTaskRunnerInterface(bool runTask = true)
                {
                    this->runTask = runTask;
                };

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
