#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <conio.h>

enum Status {
    Initilized,
    Added,
    OnGoing,
    Done
};

struct Task {
    Task() { status = Initilized; }
    Task(std::function<void()> _work) {
        work = _work;
        status = Initilized;
    }
    std::function<void()> work;
    Status status;
};

class ThreadPool {
    std::mutex mQueueMutex;
    std::condition_variable mNewTaskNotifyCV;
    std::condition_variable mEndAllTasksNotifyCV;
    uint32_t busyWorkers = 0;
        
    std::vector<std::jthread> mWorkers;
    std::queue<Task*> mTasks;
    
public:
    /// <summary>
    /// Create a new Thread Pool
    /// </summary>
    /// <param name="threadNumber"> How many worker the pool needs to create</param>
    ThreadPool(uint32_t threadNumber = std::thread::hardware_concurrency()-1) {
        //Create as much workers as asked
        for (uint32_t i = 0; i < threadNumber; ++i) {
            //Create a new thread with while true loop as it have to live as long as it's not stopped
            mWorkers.emplace_back([this](std::stop_token stoken) {
                while (true) {
                    //Initializing the task variable
                    Task* task;
                    //Create a scope to lock the queue
                    {
                        //Lock
                        std::unique_lock<std::mutex> lock(mQueueMutex);
                        //Wait on predicate so unlock queue
                        mNewTaskNotifyCV.wait(lock, [this] {
                            return !mTasks.empty();
                        });
                        //Check if stop has been requested for the thread
                        if (stoken.stop_requested()) {
                            return;
                        }
                        //Increment the busy workers number
                        busyWorkers++;
                        //Get a task a the top of the queue
                        task = mTasks.front();
                        //Remove the gotten task from the queue
                        mTasks.pop();
                    }
                    //Update the task status
                    task->status = Status::OnGoing;
                    //Execute the task work
                    task->work();
                    //Update the task status
                    task->status = Status::Done;
                    //Decrement the busy workers number
                    busyWorkers--;
                    //Notify the Worker Pool that the current job is done
                    mEndAllTasksNotifyCV.notify_one();
                }
            });
        }
    }

    ~ThreadPool() {
        //For all workers, request them to stop
        for (auto& w : mWorkers) {
            w.request_stop();
        }
        //Notify all the workers so they wake up to stop
        mNewTaskNotifyCV.notify_all();
        //Clear the threads
        mWorkers.clear();
    }

    /// <summary>
    /// Try to add a task to the Worker Pool
    /// </summary>
    /// <param name="task">The task to add</param>
    /// <returns>If the task has successfully added</returns>
    bool AddTask(Task* task) {
    
        //Check if the task hasn't been already added
        if (task->status != Status::Initilized)
            return false;

        //Create a scope to lock the queue and add the new task
        {
            //Lock the queue mutex
            std::unique_lock<std::mutex> lock(mQueueMutex);
            //Add the task to the end of the queue
            mTasks.push(task);
            //Update the task status
            task->status = Status::Added;
        }   
        //Notify the workers that a new task has been added
        mNewTaskNotifyCV.notify_one();
        return true;
    }

    void WaitFinished() {
        //Lock the queue
        std::unique_lock<std::mutex> lock(mQueueMutex);
        //Check if task list is empty and if no worker has an ongoing task
        mEndAllTasksNotifyCV.wait(lock, [this] {return mTasks.empty() && busyWorkers == 0; });
    }
};

void Print() {
    std::cout << "print" << std::endl;
}

void TestA(ThreadPool& threadpool) {

    Task taskA = Task(&Print);
    Task taskB = Task(&Print);
    
    threadpool.AddTask(&taskA);
    threadpool.AddTask(&taskB);

    threadpool.WaitFinished();

    if (taskA.status == Status::Done && taskB.status == Status::Done) {
        std::cout << "All job done !" << std::endl;
    }
}

int main()
{
    ThreadPool Threadpool;

    std::cout << "Enter :" << std::endl << "a" << std::endl;
    auto value = _getch();
    switch (value) {
    case 97:
        TestA(Threadpool);
        break;
    }
    return 0;
}
