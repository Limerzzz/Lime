/*
 * @Author: Limer
 * @Date: 2022-06-07 13:07:21
 * @LastEditors: Limer
 * @LastEditTime: 2022-06-14 13:49:36
 * @Description:
 */
#pragma once
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

class ThreadPool {
   public:
    ThreadPool(int size = std::thread::hardware_concurrency());
    ~ThreadPool();
    template <class F, class... Args>
    auto add(F&&, Args&&...)
        -> std::future<typename std::result_of<F(Args...)>::type>;

   private:
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_que_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool stop;
};

template <class F, class... Args>
auto ThreadPool::add(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type> {
    // result_of_t<F(Args...)> =result_of<F(Args...)>::type
    using return_type = typename std::result_of_t<F(Args...)>;
    // task is a shared pointer
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(mtx_);
        if (stop)
            throw std::runtime_error("enqueue  on  stopped ThreadPool.\n");
        tasks_que_.emplace([task]() { (*task)(); });
    }
    cv_.notify_one();
    return res;
}
