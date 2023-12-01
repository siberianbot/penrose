#ifndef PENROSE_RESOURCES_RUNNABLE_HPP
#define PENROSE_RESOURCES_RUNNABLE_HPP

namespace Penrose {

    class [[deprecated]] Runnable {
    public:
        virtual ~Runnable() = default;

        virtual void run() = 0;
        virtual void stop() = 0;
    };
}

#endif // PENROSE_RESOURCES_RUNNABLE_HPP
