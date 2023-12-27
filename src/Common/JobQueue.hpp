#ifndef PENROSE_COMMON_JOB_QUEUE_HPP
#define PENROSE_COMMON_JOB_QUEUE_HPP

#include <functional>
#include <optional>
#include <semaphore>
#include <thread>

#include "src/Common/OrderedQueue.hpp"

namespace Penrose {

    class JobQueue {
    public:
        using Func = std::function<void()>;

        struct Params {
            int order = 0;

            // Should remove job after execution
            bool remove = true;

            // New job replaces old job with same order
            bool override = false;
        };

        JobQueue();

        void start();
        void stop();

        void enqueue(Func &&func, Params &&params = {});

        void clear();

        [[nodiscard]] bool running() const { return this->_running; }

    private:
        struct Job {
            Func func;
            int order;
            bool remove;
        };

        struct JobEqualTo {
            bool operator()(const Job &lhs, const Job &rhs) const { return lhs.order == rhs.order; }
        };

        struct JobLess {
            bool operator()(const Job &lhs, const Job &rhs) const { return lhs.order < rhs.order; }
        };

        std::optional<std::jthread> _thread;
        std::atomic_bool _running;
        std::binary_semaphore _semaphore;
        OrderedQueue<Job, JobEqualTo, JobLess> _jobs;

        Job acquire();
    };
}

#endif // PENROSE_COMMON_JOB_QUEUE_HPP
