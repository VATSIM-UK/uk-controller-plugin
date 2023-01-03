#include "MockTaskRunnerInterface.h"

namespace UKControllerPluginTest::TaskManager {
    MockTaskRunnerInterface::MockTaskRunnerInterface(bool runTask) : runTask(runTask){};

    MockTaskRunnerInterface::~MockTaskRunnerInterface() = default;

    auto MockTaskRunnerInterface::CountThreads() const -> size_t
    {
        return 0;
    }

    /*
        Run the task only if required.
    */
    void MockTaskRunnerInterface::QueueAsynchronousTask(std::function<void()> callback)
    {
        if (this->runTask) {
            callback();
        };
    };
} // namespace UKControllerPluginTest::TaskManager
