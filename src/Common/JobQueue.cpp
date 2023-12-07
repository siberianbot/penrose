#include "JobQueue.hpp"

namespace Penrose {

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

                const auto &job = this->_jobs.front();

                job->exec();

                if (job->remove()) {
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

    void JobQueue::enqueue(std::unique_ptr<Job> &&job) {
        this->_jobs.push(std::forward<decltype(job)>(job));
    }

    void JobQueue::enqueue(std::function<void()> &&func) {
        this->enqueue<FuncJob>(std::forward<decltype(func)>(func));
    }

    void JobQueue::clear() {
        this->_jobs.clear();
    }
}
