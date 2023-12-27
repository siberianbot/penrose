#ifndef PENROSE_ECS_SYSTEM_STATE_HPP
#define PENROSE_ECS_SYSTEM_STATE_HPP

namespace Penrose {

    /**
     * \brief Current system state
     */
    enum class SystemState {
        Stopped,
        Initializing,
        Running,
        Paused,
        Stopping,
        Failed
    };
}

#endif // PENROSE_ECS_SYSTEM_STATE_HPP
