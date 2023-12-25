#ifndef PENROSE_UTILS_TEMPLATE_UTILS_HPP
#define PENROSE_UTILS_TEMPLATE_UTILS_HPP

namespace Penrose {

    /**
     * \brief Ensures all values are true
     */
    template <bool... Values>
    concept All = (Values && ...);

    /**
     * \brief Ensures any value is true
     */
    template <bool... Values>
    concept Any = (Values || ...);
}

#endif // PENROSE_UTILS_TEMPLATE_UTILS_HPP
