#ifndef PENROSE_RESOURCES_UPDATABLE_HPP
#define PENROSE_RESOURCES_UPDATABLE_HPP

namespace Penrose {

    /**
     * \brief Resource interface for types that requires constant update in main thread
     */
    class Updatable {
    public:
        virtual ~Updatable() = default;

        /**
         * \brief Update resource
         * \param delta Frame delta time in seconds
         */
        virtual void update(float delta) = 0;
    };
}

#endif // PENROSE_RESOURCES_UPDATABLE_HPP
