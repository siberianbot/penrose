#ifndef PENROSE_UTILS_TEMPLATE_UTILS_HPP
#define PENROSE_UTILS_TEMPLATE_UTILS_HPP

namespace Penrose {

    template<bool ...Values>
    concept All = (Values && ...);

    template<bool ...Values>
    concept Any = (Values || ...);
}

#endif // PENROSE_UTILS_TEMPLATE_UTILS_HPP
