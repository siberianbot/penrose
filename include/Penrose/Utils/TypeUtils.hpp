#ifndef PENROSE_UTILS_TYPE_UTILS_HPP
#define PENROSE_UTILS_TYPE_UTILS_HPP

#include <string>
#include <typeindex>

#if defined(__GNUC__)
#    include <cxxabi.h>
#endif

namespace Penrose {

    /* TODO: constexpr */ static std::string demangle(const char *name) {
#if defined(__GNUC__)
        int status;
        char *realname = abi::__cxa_demangle(name, nullptr, nullptr, &status);

        if (status != 0) {
            return name;
        }

        return realname;
#else
        return name;
#endif
    }

    [[nodiscard]] inline std::string getTypeName(const std::type_index type) {
        return demangle(type.name());
    }

    template <typename T>
    [[nodiscard]] std::string getTypeName() {
        return demangle(typeid(T).name());
    }
}

#endif // PENROSE_UTILS_TYPE_UTILS_HPP
