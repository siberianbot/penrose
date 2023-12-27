#ifndef PENROSE_ECS_SYSTEM_PARAMS_HPP
#define PENROSE_ECS_SYSTEM_PARAMS_HPP

namespace Penrose {

    /**
     * \brief Values for SystemParams::autostart
     */
    enum class SystemAutoStartParam {
        None,
        Once,
        Always
    };

    /**
     * \brief System parameters
     */
    struct PENROSE_API SystemParams {

        /**
         * \brief Require system autostart
         */
        SystemAutoStartParam autostart = SystemAutoStartParam::Once;

        /**
         * \brief Try to recover system after failure
         */
        bool recover = true;
    };
}

#endif // PENROSE_ECS_SYSTEM_PARAMS_HPP
