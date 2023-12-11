#ifndef PENROSE_RENDERING_COMMAND_RECORDING_HPP
#define PENROSE_RENDERING_COMMAND_RECORDING_HPP

#include <cstdint>

#include <Penrose/Types/Rect.hpp>

namespace Penrose {

    class Buffer;
    class Pipeline;
    class Descriptor;

    class CommandRecording {
    public:
        virtual ~CommandRecording() = default;

        virtual void setViewport(FloatRect rect) = 0;
        virtual void setScissor(IntRect rect) = 0;

        virtual void bindGraphicsPipeline(Pipeline *pipeline) = 0;

        virtual void bindPushConstants(Pipeline *pipeline, std::uint32_t constantIdx, void *data) = 0;
        virtual void bindDescriptor(Pipeline *pipeline, Descriptor *descriptor) = 0;

        virtual void bindBuffer(std::uint32_t bindingIdx, Buffer *buffer, std::uint32_t offset) = 0;
        virtual void bindIndexBuffer(Buffer *buffer, std::uint32_t offset) = 0;

        virtual void draw(std::uint32_t indexCount, std::uint32_t instanceCount) = 0;
    };
}

#endif // PENROSE_RENDERING_COMMAND_RECORDING_HPP
