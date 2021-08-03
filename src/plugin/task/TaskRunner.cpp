#include "pch/pch.h"
#include "task/TaskRunner.h"

namespace UKControllerPlugin {
    namespace TaskManager {

        TaskRunner::TaskRunner(int numThreads)
        {
            // Create the threads for asynchronous tasks.
            std::unique_lock<std::mutex> uniqueLock(this->asynchronousQueueLock);
            for (int i = 0; i < numThreads; i++) {
                this->threads.push_back(std::thread(&TaskRunner::ProcessAsynchronousTasks, this, i));
            }

            LogInfo("TaskRunner created with " + std::to_string(numThreads) + " threads");
        }

        /*
            Shut down all the threads.
        */
        TaskRunner::~TaskRunner(void)
        {
            std::unique_lock<std::mutex> uniqueLock(this->asynchronousQueueLock);
            this->threadsRunning = false;
            this->asynchronousQueueCondVar.notify_all();
            uniqueLock.unlock();


            for (auto &thread : this->threads) {
                if (thread.joinable()) {
                    thread.join();
                }
            }
            LogInfo("All TaskRunner threads shut down");
        }

        size_t TaskRunner::CountThreads(void) const
        {
            return this->threads.size();
        }

        /*
            Queue an aysynchronous task and notify one thread. These kinds of tasks involve actions
            that may be blocking to the EuroScope instance for significant periods
            of time, for example, tasks that involve CURL.
        */
        void TaskRunner::QueueAsynchronousTask(std::function<void(void)> task)
        {
            std::unique_lock<std::mutex> uniqueLock(this->asynchronousQueueLock);
            this->asynchronousTaskQueue.push_back(std::move(task));
            this->asynchronousQueueCondVar.notify_one();
        }

        /*
            A method to process tasks that are asynchronous - running outside the normal
            loop of EuroScope execution. For example, tasks that require HTTP requests, which
            make take a significant amount of time.
        */
        void TaskRunner::ProcessAsynchronousTasks(int threadNumber)
        {
            std::unique_lock<std::mutex> uniqueLock(this->asynchronousQueueLock, std::defer_lock_t());
            std::function<void(void)> currentTask;
            while (true) {

                uniqueLock.lock();

                // We've been commanded to stop, lets go.
                if (!this->threadsRunning) {
                    break;
                }

                // If the queue is empty, we should wait for a job
                if (this->asynchronousTaskQueue.empty()) {
                    this->asynchronousQueueCondVar.wait(uniqueLock);

                    // Spurious wakeup, skip the loop
                    if (this->asynchronousTaskQueue.empty()) {
                        uniqueLock.unlock();
                        continue;
                    }
                }

                // Lock the queue and copy the task off
                currentTask = std::move(this->asynchronousTaskQueue.front());
                this->asynchronousTaskQueue.pop_front();
                uniqueLock.unlock();

                // Do the task
                try {
                    currentTask();
                }
                catch (std::exception exception) {
                    LogError("Unhandled exception in task runner " + std::string(exception.what()));
                }
            }

            LogInfo("Task runner thread " + std::to_string(threadNumber) + " stopped");
        }
    }  // namespace TaskManager
}  // namespace UKControllerPlugin
