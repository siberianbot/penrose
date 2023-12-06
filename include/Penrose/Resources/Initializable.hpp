#ifndef PENROSE_RESOURCES_INITIALIZABLE_HPP
#define PENROSE_RESOURCES_INITIALIZABLE_HPP

namespace Penrose {

    /**
     * \brief Resource interface for types that requires initialization or deinitialization
     */
    class Initializable {
    public:
        virtual ~Initializable() = default;

        /**
         * \brief Initialize resource
         */
        virtual void init() = 0;

        /**
         * \brief Deinitialize (or destroy) resource
         */
        virtual void destroy() = 0;
    };
}

#endif // PENROSE_RESOURCES_INITIALIZABLE_HPP
