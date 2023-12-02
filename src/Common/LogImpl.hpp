#ifndef PENROSE_COMMON_LOG_IMPL_HPP
#define PENROSE_COMMON_LOG_IMPL_HPP

#include <Penrose/Common/Log.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    class LogImpl final: public Resource<LogImpl, ResourceGroup::Engine>,
                         public Log {
    public:
        explicit LogImpl(ResourceSet *resources);
        ~LogImpl() override = default;

        void write(LogLevel level, std::string_view tag, std::string_view msg) override;

        void addSink(std::type_index &&index) override;

    private:
        ResourceSet *_resources;
        ResourceProxy<LogSink> _sinks;
    };
}

#endif // PENROSE_COMMON_LOG_IMPL_HPP
