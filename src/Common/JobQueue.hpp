#ifndef PENROSE_COMMON_JOB_QUEUE_HPP
#define PENROSE_COMMON_JOB_QUEUE_HPP

#include <functional>
#include <optional>
#include <queue>
#include <thread>

namespace Penrose {

    template <typename T>
    concept Job = requires(T t) {
        { t.operator()() } -> std::same_as<void>;
    };

    class JobQueue {
    public:
        void start();
        void stop();

        void enqueue(Job auto &&job) {
            this->_jobs.emplace([job] { job(); });
        }

        void clear();

    private:
        std::optional<std::jthread> _thread;
        std::queue<std::function<void()>> _jobs;
    };
}

#endif // PENROSE_COMMON_JOB_QUEUE_HPP
