#ifndef PENROSE_RESOURCES_RESOURCE_HPP
#define PENROSE_RESOURCES_RESOURCE_HPP

#include <typeindex>
#include <typeinfo>

namespace Penrose {

    /**
     * \brief Base type of Resource<> for pointers
     */
    class PENROSE_API ResourceBase {
    public:
        virtual ~ResourceBase() = default;

        /**
         * \brief Get type of instanced resource
         * \return Type index of resource
         */
        [[nodiscard]] virtual std::type_index getType() const = 0;
    };

    /**
     * \brief Base type of engine resource
     * \details Resources implements some sort of dependency injection pattern. This allows us to make engine more
     * flexible and modular. Resource instances cannot be copied or movied. They are instanced only once by dependency
     * container. Any specific behavior which requires resource initialization/deinitialization or update can be
     * achivied by implementing Initializable or Updatable interfaces by inheritor class.
     * \tparam Self Type of Resource<> inheritor
     */
    template <typename Self>
    class PENROSE_API Resource: public ResourceBase {
    public:
        Resource(const Resource &) = delete;
        Resource(Resource &&) = delete;
        Resource &operator=(const Resource &) = delete;
        Resource &operator=(Resource &&) = delete;

        ~Resource() override = default;

        /**
         * \brief Get type of resource
         * \return Type index of resource
         */
        [[nodiscard]] static std::type_index type() { return typeid(Self); }

        //! \copydoc ResourceBase::getType
        [[nodiscard]] std::type_index getType() const final { return type(); }

        /**
         * \brief Create instance of resource through parameterless default constructor
         * \return Instance of resource
         */
        [[nodiscard]] constexpr static Self *create() {
            static_assert(std::is_default_constructible_v<Self>);

            return new Self();
        }

        /**
         * \brief  Create instance of resource through constructor with parameters
         * \tparam Args Type of constructor parameters
         * \param args Constructor parameters
         * \return Instance of resource
         */
        template <typename... Args>
        [[nodiscard]] constexpr static Self *create(Args &&...args) {
            static_assert(std::is_constructible_v<Self, Args...>);

            return new Self(std::forward<decltype(args)>(args)...);
        }

    protected:
        Resource() = default;
    };

    /**
     * \brief Resource concept: ensures that Target implements Resource<>
     */
    template <typename Target>
    concept IsResource = std::is_base_of_v<Resource<Target>, Target>;
}

#endif // PENROSE_RESOURCES_RESOURCE_HPP
