#ifndef PENROSE_RENDERING_RENDER_CONTEXT_INL
#define PENROSE_RENDERING_RENDER_CONTEXT_INL

namespace Penrose {

    template<IsRenderOperator T>
    void RenderContext::registerRenderOperator() {
        this->registerRenderOperator(T::name(), []() { return ParamsCollection::empty(); }, T::create);
    }

    template<IsRenderOperatorWithDefaults T>
    void RenderContext::registerRenderOperator() {
        this->registerRenderOperator(T::name(), T::defaults, T::create);
    }
}

#endif // PENROSE_RENDERING_RENDER_CONTEXT_INL
