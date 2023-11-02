//
// Created by zhimafox on 2023/9/20.
//

#ifndef UNTITLED_THREADPOOL_H
#define UNTITLED_THREADPOOL_H

#include <vector>
#include <mutex>
#include <atomic>
#include <thread>
#include <chrono>
#include <iostream>
#include <queue>
#include <map>
#include <condition_variable>

using namespace std;
const int MAX_THREADS = 100; //最大线程数目
enum class TaskPriority {
    High,
    Normal,
    Low
};


class Task  {
public:
    Task(TaskPriority priority, const string &identify) : priority(priority), mTaskIdentify(identify) {}

    virtual void process() {
        std::cout << "--------------  run task start =---------------= indentify:" << mTaskIdentify << "threadId:"<< this_thread::get_id()<< std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "==============  run task end ================= indentify:" << mTaskIdentify << "threadId:"<< this_thread::get_id()<< std::endl;
    };

    TaskPriority getPriority() {
        return priority;
    }

    string getIdentify() {
        return mTaskIdentify;
    }

private:
    TaskPriority priority{TaskPriority::Low};
    string mTaskIdentify;
};

using TaskPtr = std::shared_ptr<Task>;

class ThreadPool : public std::enable_shared_from_this<ThreadPool> {
public:
    explicit ThreadPool(int threadNumber = 6);

    ~ThreadPool();

    void addTask(const TaskPtr &task);

    void run();

private:
    TaskPtr popTask();

    bool isTaskComming();

private:
    vector<thread> mThreadList;
    atomic<bool> mIsRunning;

    map<TaskPriority, queue<TaskPtr>> taskQueue;
    std::mutex queueMtx;
    condition_variable condition;
};

#endif //UNTITLED_THREADPOOL_H
