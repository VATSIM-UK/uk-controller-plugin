#pragma once
#include "task/TaskRunnerInterface.h"

namespace UKControllerPlugin {
    namespace Curl {
        class HttpHelper;
    }  // namespace Curl
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace TaskManager {

        /*
            A class that runs Tasks on a separate thread to the main EuroScope instance.

            The primary use of this class is to run tasks that involve HTTP requests, as waiting
            for CURL on the ES thread would lock up the entire application.
        */
        class TaskRunner : public UKControllerPlugin::TaskManager::TaskRunnerInterface
        {
            public:
                TaskRunner(
                    int numAsynchronousTaskThreads,
                    int numInlineTaskThreads
                );
                ~TaskRunner(void);
                void QueueAsynchronousTask(std::function<void(void)> task) override;
                void QueueInlineTask(std::function<void(void)> task);

            private:
                void ProcessInlineTasks(void);
                void ProcessAsynchronousTasks(void);

                // Are the threads running
                bool threadsRunning = true;

                // A vector for all the threads.
                std::vector<std::thread> threads;

                // A lock for the asynchronous queue when picking off tasks.
                std::mutex asynchronousQueueLock;

                // The master queue for asynchronous tasks - will be taken off in order.
                std::deque <std::function<void(void)>> asynchronousTaskQueue;

                // A condition variable for the asynchronous queue.
                std::condition_variable asynchronousQueueCondVar;

                // A lock for the inline queue when picking off tasks.
                std::mutex inlineQueueLock;

                // The master queue for inline tasks - will be taken off in order.
                std::deque <std::function<void()>> inlineTaskQueue;

                // A condition variable for the inline queue.
                std::condition_variable inlineQueueCondVar;
        };
    }  // namespace TaskManager
}  // namespace UKControllerPlugin
