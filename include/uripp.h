#ifndef H_URIPP_H
#define H_URIPP_H

#include <future>
#include <vector>
#include <functional>
#include <queue>
#include <condition_variable>
#include <type_traits>

namespace uripp::runtime::scheduler {
namespace detail {

template<typename TRes>
using defunct_future = std::future<TRes>;

using defunct_task = std::function<void()>;
using threadpool_error = std::runtime_error;
}

class ThreadPool {
public:
    using task_type = detail::defunct_task;
private:
    bool stop_ = false;
    std::vector<std::thread> workers_;
    std::queue<task_type> tasks_;
    std::mutex task_mutex_;
    std::condition_variable task_cv_;
public:
    explicit ThreadPool(size_t nthreads) {
        for (size_t i = 0; i < nthreads; ++i) {

            workers_.emplace_back(
                    [this] {
                        for (;;) {
                            using std::unique_lock;
                            task_type tsk;
                            {
                                unique_lock<std::mutex> lk(this->task_mutex_);
                                this->task_cv_.wait(lk,
                                                    [this] {
                                                        return this->stop_ || !this->tasks_.empty();
                                                    });
                                if (this->tasks_.empty() && this->stop_) return;

                                tsk = std::move(this->tasks_.front());
                                this->tasks_.pop();
                            }
                            tsk();
                        }
                    }
            );
        }
    }

    template<typename TFunc, class... TArgs>
    auto post(TFunc &&f, TArgs &&... args)
    -> detail::defunct_future<typename std::result_of_t<TFunc(TArgs...)>> {

        using return_t = typename std::result_of_t<TFunc(TArgs...)>;
        using std::unique_lock;

        auto tsk = std::make_shared<std::packaged_task<return_t()>>(
                std::bind(std::forward<TFunc>(f), std::forward<TArgs>(args)...)
        );
        std::future<return_t> return_v = tsk->get_future();
        {
            unique_lock<std::mutex> lk(this->task_mutex_);
            if (stop_) throw detail::threadpool_error("posted on stopped ThreadPool");

            this->tasks_.emplace([tsk] { (*tsk)(); });
        }
        this->task_cv_.notify_one();
        return return_v;
    }

    ~ThreadPool() {
        using std::unique_lock;
        {
            unique_lock<std::mutex> lk(task_mutex_);
            stop_ = true;
        }
        task_cv_.notify_all();
        for (std::thread &w : workers_) { w.join(); }
    }

};
}
#endif