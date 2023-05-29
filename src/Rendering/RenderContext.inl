#ifndef PENROSE_RENDERING_RENDER_CONTEXT_INL
#define PENROSE_RENDERING_RENDER_CONTEXT_INL

namespace Penrose {

    template<IsRenderOperatorProducer T>
    void RenderContext::addRenderOperatorProducer(const std::string_view &name) {
        this->addRenderOperatorProducer(name, std::make_unique<T>());
    }
}

#endif // PENROSE_RENDERING_RENDER_CONTEXT_INL
