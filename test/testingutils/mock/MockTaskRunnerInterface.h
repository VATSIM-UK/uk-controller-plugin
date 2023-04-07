#pragma once
#include "task/TaskRunnerInterface.h"

namespace UKControllerPluginTest::TaskManager {
    class MockTaskRunnerInterface : public UKControllerPlugin::TaskManager::TaskRunnerInterface
    {
        public:
        explicit MockTaskRunnerInterface(bool runTask = true);
        virtual ~MockTaskRunnerInterface();
        [[nodiscard]] auto CountThreads() const -> size_t override;
        void QueueAsynchronousTask(std::function<void()> callback) override;

        private:
        // Whether we actually want to run the task.
        bool runTask;
    };
} // namespace UKControllerPluginTest::TaskManager
