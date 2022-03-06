#pragma once

namespace UKControllerPlugin::TaskManager {
    class TaskRunnerInterface;
} // namespace UKControllerPlugin::TaskManager

void Async(const std::function<void(void)>& function);
void SetTaskRunner(std::shared_ptr<UKControllerPlugin::TaskManager::TaskRunnerInterface> taskRunner);
void UnsetTaskRunner();
