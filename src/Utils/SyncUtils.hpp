#ifndef PENROSE_UTILS_SYNC_UTILS_HPP
#define PENROSE_UTILS_SYNC_UTILS_HPP

#include <semaphore>

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
}

#endif // PENROSE_UTILS_SYNC_UTILS_HPP
