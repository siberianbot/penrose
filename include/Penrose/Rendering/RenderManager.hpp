#ifndef PENROSE_RENDERING_RENDER_MANAGER_HPP
#define PENROSE_RENDERING_RENDER_MANAGER_HPP

#include <typeindex>

#include <Penrose/Rendering/Renderer.hpp>
#include <Penrose/Rendering/RenderSystem.hpp>

namespace Penrose {

    /**
     * \brief Render manager interface
     * \details This interface provides methods for working with rendering system.
     */
    class RenderManager {
    public:
        virtual ~RenderManager() = default;

        /**
         * \brief Set current rendering system
         * \param type Type of rendering system
         */
        virtual void setRenderSystem(std::type_index &&type) = 0;

        /**
         * \brief Set current rendering system
         * \tparam T Type of rendering system
         */
        template <typename T>
        requires std::is_base_of_v<RenderSystem, T>
        void setRenderSystem() {
            this->setRenderSystem(typeid(T));
        }

        /**
         * \brief Add renderer
         * \param type Type of renderer
         */
        virtual void addRenderer(std::type_index &&type) = 0;

        /**
         * \brief Add renderer
         * \tparam T Type of renderer
         */
        template <typename T>
        requires std::is_base_of_v<Renderer, T>
        void addRenderer() {
            this->addRenderer(typeid(T));
        }
    };
}

#endif // PENROSE_RENDERING_RENDER_MANAGER_HPP
