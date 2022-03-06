#include "RunAsyncTask.h"
#include "TaskRunnerInterface.h"

using UKControllerPlugin::TaskManager::TaskRunnerInterface;

std::shared_ptr<TaskRunnerInterface> taskRunner; // NOLINT

void Async(const std::function<void(void)>& function)
{
    if (!taskRunner) {
        return;
    }

    taskRunner->QueueAsynchronousTask(function);
}

void SetTaskRunner(std::shared_ptr<TaskRunnerInterface> runner)
{
    if (taskRunner) {
        return;
    }

    taskRunner = std::move(runner);
}

void UnsetTaskRunner()
{
    taskRunner = nullptr;
}
