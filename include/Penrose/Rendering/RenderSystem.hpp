#ifndef PENROSE_RENDERING_RENDER_SYSTEM_HPP
#define PENROSE_RENDERING_RENDER_SYSTEM_HPP

#include <string>

#include <Penrose/Rendering/RenderContext.hpp>

namespace Penrose {

    /**
     * \brief Rendering system interface
     * \details Rendering system provides methods over some 3D rendering library (Vulkan, DirectX, etc). It also
     * responsible for its initialization/deinitialization, management, etc. Rendering system must not be Initializable
     * because RenderManager is responsible for strict initialization of render system.
     */
    class PENROSE_API RenderSystem {
    public:
        virtual ~RenderSystem() = default;

        /**
         * \brief Get name of rendering system
         * \return Name of rendering system
         */
        [[nodiscard]] virtual std::string getName() const = 0;

        /**
         * \brief Initialize rendering system
         */
        virtual void init() = 0;

        /**
         * \brief Deinitialize rendering system
         */
        virtual void destroy() = 0;

        /**
         * \brief Create render context for this rendering system
         * \return Instance of render context
         */
        [[nodiscard]] virtual RenderContext *makeRenderContext() = 0;
    };
}

#endif // PENROSE_RENDERING_RENDER_SYSTEM_HPP
