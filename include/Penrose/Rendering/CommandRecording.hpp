#ifndef PENROSE_RENDERING_COMMAND_RECORDING_HPP
#define PENROSE_RENDERING_COMMAND_RECORDING_HPP

#include <cstdint>

namespace Penrose {

    class Buffer;
    class Pipeline;
    class Descriptor;

    class CommandRecording {
    public:
        virtual ~CommandRecording() = default;

        virtual void bindGraphicsPipeline(Pipeline *pipeline) = 0;

        virtual void bindPushConstants(Pipeline *pipeline, std::uint32_t constantIdx, void *data) = 0;
        virtual void bindDescriptor(Pipeline *pipeline, Descriptor *descriptor) = 0;

        virtual void draw(Buffer *vertexBuffer, Buffer *indexBuffer) = 0;
    };
}

#endif // PENROSE_RENDERING_COMMAND_RECORDING_HPP
