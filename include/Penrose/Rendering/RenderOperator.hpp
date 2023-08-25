#ifndef PENROSE_RENDERING_RENDER_OPERATOR_HPP
#define PENROSE_RENDERING_RENDER_OPERATOR_HPP

#include <cstdint>
#include <string>

#include <Penrose/Common/ParamsCollection.hpp>
#include <Penrose/Common/Size.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class CommandRecording;
    class RenderSubgraph;

    class RenderOperator : public InitializableResource {
    public:
        struct Context {
            RenderSubgraph *subgraph;
            std::uint32_t subgraphPassIdx;
            Size renderArea;
            ParamsCollection param;
        };

        ~RenderOperator() override = default;

        [[nodiscard]] virtual std::string getName() const = 0;

        [[nodiscard]] virtual ParamsCollection getDefaults() const = 0;

        virtual void execute(CommandRecording *commandRecording, const RenderOperator::Context &context) = 0;
    };
}

#endif // PENROSE_RENDERING_RENDER_OPERATOR_HPP
