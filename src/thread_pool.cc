/*
 * @Author: Limer
 * @Date: 2022-06-07 13:15:47
 * @LastEditors: Limer
 * @LastEditTime: 2022-06-15 13:58:04
 * @Description:
 */
#include "thread_pool.h"
#include <mutex>
#include <stdexcept>

ThreadPool::ThreadPool(int size) {
    stop = false;
    for (int i = 0; i < size; ++i) {
        threads_.emplace_back(std::thread([this]() {
            while (true) {
                std::function<void()> task;
                {  // retrieve the task
                    std::unique_lock<std::mutex> lock(mtx_);
                    // if threadpool is stopped or the task queue isn't empty,
                    // this thread will not be blocked.
                    cv_.wait(lock,
                             [this]() { return stop || !tasks_que_.empty(); });
                    if (stop && tasks_que_.empty()) return;
                    task = tasks_que_.front();
                    tasks_que_.pop();
                }
                task();
            }
        }));
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(mtx_);
        stop = true;
    }
    cv_.notify_all();
    for (auto& th : threads_)
        if (th.joinable()) th.join();
}