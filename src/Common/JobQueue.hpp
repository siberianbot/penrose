#ifndef PENROSE_COMMON_JOB_QUEUE_HPP
#define PENROSE_COMMON_JOB_QUEUE_HPP

#include <functional>
#include <memory>
#include <optional>
#include <thread>

#include "src/Common/OrderedQueue.hpp"

namespace Penrose {

    class Job {
    public:
        virtual ~Job() = default;

        [[nodiscard]] virtual int order() const = 0;

        [[nodiscard]] virtual bool remove() const = 0;

        virtual void exec() = 0;
    };

    class FuncJob final: public Job {
    public:
        explicit FuncJob(std::function<void()> &&func)
            : _func(std::forward<decltype(func)>(func)) {
            //
        }

        ~FuncJob() override = default;

        [[nodiscard]] int order() const override { return 0; }

        [[nodiscard]] bool remove() const override { return true; }

        void exec() override { this->_func(); }

    private:
        std::function<void()> _func;
    };

    class JobQueue {
    public:
        void start();
        void stop();

        void enqueue(std::unique_ptr<Job> &&job);

        void enqueue(std::function<void()> &&func);

        template <typename T, typename... Args>
        requires std::is_base_of_v<Job, T> && std::is_constructible_v<T, Args...>
        void enqueue(Args &&...args) {
            this->enqueue(std::make_unique<T>(std::forward<Args>(args)...));
        }

        void clear();

    private:
        std::optional<std::jthread> _thread;
        OrderedQueue<std::unique_ptr<Job>> _jobs;
    };
}

template <>
struct std::less<Penrose::Job> {
    bool operator()(const Penrose::Job &l, const Penrose::Job &r) const { return l.order() < r.order(); }
};

template <>
struct std::less<std::unique_ptr<Penrose::Job>> {
    bool operator()(const std::unique_ptr<Penrose::Job> &l, const std::unique_ptr<Penrose::Job> &r) const {
        return l->order() < r->order();
    }
};

template <>
struct std::equal_to<Penrose::Job> {
    bool operator()(const Penrose::Job &l, const Penrose::Job &r) const { return l.order() == r.order(); }
};

template <>
struct std::equal_to<std::unique_ptr<Penrose::Job>> {
    bool operator()(const std::unique_ptr<Penrose::Job> &l, const std::unique_ptr<Penrose::Job> &r) const {
        return l->order() == r->order();
    }
};

#endif // PENROSE_COMMON_JOB_QUEUE_HPP
