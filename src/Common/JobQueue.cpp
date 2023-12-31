#include "JobQueue.hpp"

#include "src/Utils/SyncUtils.hpp"

namespace Penrose {

    JobQueue::JobQueue()
        : _running(false),
          _semaphore(1),
          _jobs({}) {
        //
    }

    void JobQueue::start() {
        if (this->_thread.has_value()) {
            this->stop();
        }

        this->_thread = std::jthread([this](const std::stop_token &token) {
            this->_running = true;

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

            this->_running = false;
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
