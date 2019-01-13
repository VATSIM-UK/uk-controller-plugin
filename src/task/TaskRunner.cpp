#include "pch/stdafx.h"
#include "task/TaskRunner.h"

namespace UKControllerPlugin {
    namespace TaskManager {

        TaskRunner::TaskRunner(int numAsynchronousThreads, int numSynchronousThreads)
        {
            // Create the threads for asynchronous tasks.
            for (int i = 0; i < numAsynchronousThreads; i++) {
                this->threads.push_back(std::thread(std::bind(&TaskRunner::ProcessAsynchronousTasks, this)));
            }

            // Create the threads for inline tasks.
            for (int i = 0; i < numSynchronousThreads; i++) {
                this->threads.push_back(std::thread(std::bind(&TaskRunner::ProcessInlineTasks, this)));
            }

            LogInfo(
                "TaskRunner created " + std::to_string(numAsynchronousThreads) + " asynchonronous and " +
                    std::to_string(numSynchronousThreads) + " synchronous threads"
            );
        }

        /*
            Shut down all the threads.
        */
        TaskRunner::~TaskRunner(void)
        {
            std::unique_lock <std::mutex> uniqueLockInline(this->inlineQueueLock);
            std::unique_lock <std::mutex> uniqueLockAsynchronous(this->asynchronousQueueLock);
            this->threadsRunning = false;
            this->inlineQueueCondVar.notify_all();
            this->asynchronousQueueCondVar.notify_all();
            uniqueLockInline.unlock();
            uniqueLockAsynchronous.unlock();


            for (auto &thread : this->threads) {
                thread.join();
            }
            LogInfo("All TaskRunner threads shut down");
        }

        /*
            Queue an aysynchronous task and notify one thread. These kinds of tasks involve actions
            that may be blocking to the EuroScope instance for significant periods
            of time, for example, tasks that involve CURL.
        */
        void TaskRunner::QueueAsynchronousTask(std::function<void(void)> task)
        {
            std::unique_lock <std::mutex> uniqueLock(this->asynchronousQueueLock);
            this->asynchronousTaskQueue.push_back(std::move(task));
            this->asynchronousQueueCondVar.notify_one();
            uniqueLock.unlock();
        }

        /*
            Queue an inline task and notify one thread. These kind of tasks should only
            be ones that will not block EuroScope for any longer than they otherwise would
            if being run sequentially, the kind of tasks that occur regularly and thus would
            incur a large thread creation overhead - for example, rendering.
        */
        void TaskRunner::QueueInlineTask(std::function<void()> task)
        {
            std::unique_lock <std::mutex> uniqueLock(this->inlineQueueLock);
            this->inlineTaskQueue.push_back(std::move(task));
            this->inlineQueueCondVar.notify_one();
            uniqueLock.unlock();
        }

        /*
            A method for running tasks that run on separate threads, but inline
            with standard EuroScope execution. That is, somewhere, EuroScope will
            be waiting for these tasks to signal that they are complete before continuing.

            This should not be used to run tasks such as HTTP requests, which will slow down the process.
        */
        void TaskRunner::ProcessInlineTasks()
        {
            std::defer_lock_t def;
            std::function<void()> currentTask;
            std::unique_lock <std::mutex> uniqueLock(this->inlineQueueLock, def);
            while (true) {

                uniqueLock.lock();
                // We've been commanded to stop, lets go.
                if (!threadsRunning) {
                    break;
                }

                // If the queue is empty, we should wait for a job
                if (this->inlineTaskQueue.empty()) {
                    this->inlineQueueCondVar.wait(uniqueLock);

                    // Spurious wakeup, skip the loop
                    if (this->inlineTaskQueue.empty()) {
                        uniqueLock.unlock();
                        continue;
                    }
                }

                // Lock the queue and copy the task off
                currentTask = std::move(this->inlineTaskQueue.front());
                this->inlineTaskQueue.pop_front();
                uniqueLock.unlock();

                // Now we'll do the task and set the response
                try {
                    // Do the task
                    currentTask();
                }
                catch (...) {
                    // For the moment, if we hit an exception, just do nothing.
                }
            }
        }

        /*
            A method to process tasks that are asynchronous - running outside the normal
            loop of EuroScope execution. For example, tasks that require HTTP requests, which
            make take a significant amount of time.
        */
        void TaskRunner::ProcessAsynchronousTasks()
        {
            std::defer_lock_t def;
            std::function<void(void)> currentTask;
            std::unique_lock <std::mutex> uniqueLock(this->asynchronousQueueLock, def);
            while (true) {

                uniqueLock.lock();
                // We've been commanded to stop, lets go.
                if (!threadsRunning) {
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
                catch (...) {
                    // For the moment, if we hit an exception, just do nothing.
                }
            }
        }
    }  // namespace TaskManager
}  // namespace UKControllerPlugin
