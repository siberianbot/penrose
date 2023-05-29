#ifndef PENROSE_RENDERING_RENDER_CONTEXT_INL
#define PENROSE_RENDERING_RENDER_CONTEXT_INL

namespace Penrose {

    template<IsDefaultConstructableRenderOperator T>
    void RenderContext::addRenderOperator(const std::string_view &name) {
        this->addRenderOperator(name, std::make_unique<T>());
    }

    template<IsConstructableWithResourceSetRenderOperator T>
    void RenderContext::addRenderOperator(const std::string_view &name) {
        this->addRenderOperator(name, std::make_unique<T>(this->_resources));
    }
}

#endif // PENROSE_RENDERING_RENDER_CONTEXT_INL
