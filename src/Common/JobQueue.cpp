#include "JobQueue.hpp"

namespace Penrose {

    class SemaphoreGuard {
    public:
        explicit SemaphoreGuard(std::binary_semaphore &semaphore)
            : _semaphore(semaphore) {
            this->_semaphore.acquire();
        }

        ~SemaphoreGuard() { this->_semaphore.release(); }

    private:
        std::binary_semaphore &_semaphore;
    };

    JobQueue::JobQueue()
        : _semaphore(1),
          _jobs({}) {
        //
    }

    void JobQueue::start() {
        if (this->_thread.has_value()) {
            this->stop();
        }

        this->_thread = std::jthread([this](const std::stop_token &token) {
            while (!token.stop_requested()) {
                if (this->_jobs.empty()) {
                    std::this_thread::yield();
                    continue;
                }

                const auto job = this->acquire();

                job.func();

                if (job.remove) {
                    auto guard = SemaphoreGuard(this->_semaphore);

                    this->_jobs.pop();
                }
            }
        });
    }

    void JobQueue::stop() {
        if (!this->_thread.has_value()) {
            return;
        }

        this->_thread->request_stop();

        if (this->_thread->joinable()) {
            this->_thread->join();
        }

        this->_thread = std::nullopt;
    }

    void JobQueue::enqueue(Func &&func, Params &&params) {
        const auto guard = SemaphoreGuard(this->_semaphore);

        this->_jobs.push(
            Job {
                .func = std::move(func),
                .order = params.order,
                .remove = params.remove,
            },
            params.override
        );
    }

    void JobQueue::clear() {
        const auto guard = SemaphoreGuard(this->_semaphore);

        this->_jobs.clear();
    }

    JobQueue::Job JobQueue::acquire() {
        const auto guard = SemaphoreGuard(this->_semaphore);

        return this->_jobs.front();
    }
}
