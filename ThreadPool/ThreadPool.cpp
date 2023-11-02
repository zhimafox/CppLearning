//
// Created by zhimafox on 2023/9/21.
//
#include "ThreadPool.h"
#include <iostream>

static void threadWork(ThreadPool* pool)
{
    pool->run();
}

ThreadPool::ThreadPool(int threadNumber) : mIsRunning(false)
{
    if (threadNumber <= 0 || threadNumber > MAX_THREADS)
    {
        throw std::exception();
    }

    for (int i = 0; i < threadNumber; i++)
    {
        mThreadList.emplace_back(&ThreadPool::run, this);
    }
}


ThreadPool::~ThreadPool()
{
    mIsRunning = false;
    condition.notify_all();
    for (auto &thread: mThreadList)
    {
        thread.join();
    }
}

bool ThreadPool::isTaskComming()
{
    unique_lock<mutex> unique(queueMtx);
    if (!taskQueue[TaskPriority::High].empty()
        || !taskQueue[TaskPriority::Normal].empty()
        || !taskQueue[TaskPriority::Low].empty())
    {
        return true;
    }
    return false;
}

TaskPtr ThreadPool::popTask()
{
    unique_lock<mutex> unique(queueMtx);
    TaskPtr task;
    std::cout << "----to find task--" << std::endl;
    if (!taskQueue[TaskPriority::High].empty())
    {
        std::cout << "----found High task" << std::endl;
        task = taskQueue[TaskPriority::High].front();
        taskQueue[TaskPriority::High].pop();
    }
    else if (!taskQueue[TaskPriority::Normal].empty())
    {
        std::cout << "----found Normal task" << std::endl;
        task = taskQueue[TaskPriority::Normal].front();
        taskQueue[TaskPriority::Normal].pop();
    }
    else if (!taskQueue[TaskPriority::Low].empty())
    {
        std::cout << "----found Low task" << std::endl;
        task = taskQueue[TaskPriority::Low].front();
        taskQueue[TaskPriority::Low].pop();
    } else
    {
        std::cout << "----no found any task" << std::endl;
        return nullptr;
    }

    return task;
}

void ThreadPool::run()
{
    mIsRunning = true;

    std::cout << "start ThreadPool, threadId:" <<  this_thread::get_id() << std::endl;
    while (mIsRunning)
    {
        {
            unique_lock<mutex> unique(queueMtx);
            while (taskQueue[TaskPriority::High].empty()
                   && taskQueue[TaskPriority::Normal].empty()
                   && taskQueue[TaskPriority::Low].empty())
            {
                std::cout << "Wait signal====" << std::endl;
                condition.wait(unique);
            }
        }
        std::cout << "Received signal ---<<<<" << std::endl;
        if (auto task = popTask())
        {
            std::cout << "pop task, and run(call process())" << std::endl;
            task->process();
        }
        else
        {
            std::cout << "run--no task now" << std::endl;
            //No task now.
        }
    }
}


void ThreadPool::addTask(const TaskPtr &task)
{
    std::cout << "addTask, Identifer:" << task->getIdentify() << std::endl;
    {
        unique_lock<mutex> lockGuard(queueMtx);
        auto iter = taskQueue.find(task->getPriority());
        if (iter == taskQueue.end())
        {
            taskQueue[task->getPriority()] = {};
        }
        taskQueue[task->getPriority()].emplace(task);
        std::cout << "send signal---->>>" << std::endl;
    }
    condition.notify_all();

}
