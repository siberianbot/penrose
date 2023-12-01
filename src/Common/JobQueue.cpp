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

                auto &job = this->_jobs.front();

                job();

                this->_jobs.pop();
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

    void JobQueue::clear() {
        while (!this->_jobs.empty()) {
            this->_jobs.pop();
        }
    }
}
