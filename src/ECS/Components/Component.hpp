#ifndef PENROSE_ECS_COMPONENTS_COMPONENT_HPP
#define PENROSE_ECS_COMPONENTS_COMPONENT_HPP

namespace Penrose {

    class Component {
    protected:
        bool _dirty = false;

    public:
        virtual ~Component() = default;

        [[nodiscard]] bool isDirty() const { return this->_dirty; }

        void setDirty(bool dirty) {
            this->_dirty = dirty;
        }
    };
}

#endif // PENROSE_ECS_COMPONENTS_COMPONENT_HPP
