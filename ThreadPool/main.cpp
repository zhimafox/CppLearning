#include <iostream>
#include "ThreadPool.h"

int main() {
//    std::cout << "Hello, World!" << std::endl;
    ThreadPool pool(6);
//    for (int i = 0; i < 15; ++i) {
//        if (i < 5)
//        {
//            auto task = std::make_shared<Task>(TaskPriority::Low, "Low");
//            pool.addTask(task);
//        }
//        else if (i < 10)
//        {
//            auto task = std::make_shared<Task>(TaskPriority::Normal, "Normal");
//            pool.addTask(task);
//        }
//        else
//        {
//            auto task = std::make_shared<Task>(TaskPriority::High, "High");
//            pool.addTask(task);
//        }
//    }
    auto task = std::make_shared<Task>(TaskPriority::High, "High");
    pool.addTask(task);
    while (true) {

    }
    return 0;
}
