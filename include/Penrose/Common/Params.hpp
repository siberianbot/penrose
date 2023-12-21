#ifndef PENROSE_COMMON_PARAMS_HPP
#define PENROSE_COMMON_PARAMS_HPP

#include <map>
#include <optional>
#include <string>
#include <variant>

namespace Penrose {

    /**
     * \brief Container for parameters of fourth types: boolean, integer, floating point number or string
     */
    class PENROSE_API Params {
    public:
        /**
         * \brief Value variant type
         */
        using Value = std::variant<bool, int, float, std::string>;

        /**
         * \brief Value container type
         */
        using Container = std::map<std::string, Value>;

        /**
         * \brief Emplaces new key-value into container
         * \param key Key
         * \param value Value
         * \return This instance
         */
        [[nodiscard]] Params &add(std::string_view &&key, Value &&value) {
            this->_data.emplace(key, std::forward<decltype(value)>(value));

            return *this;
        }

        /**
         * \brief Replaces existing or emplaces new key-value into container
         * \param key Key
         * \param value Value
         * \return This instance
         */
        [[nodiscard]] Params &set(std::string_view &&key, Value &&value) {
            this->_data.insert_or_assign(std::string(key), std::forward<decltype(value)>(value));

            return *this;
        }

        /**
         * \brief Try to get value from container or return nothing
         * \param key Key
         * \return Value or nothing
         */
        [[nodiscard]] std::optional<Value> tryGet(std::string_view &&key) const {
            const auto it = this->_data.find(std::string(key));

            if (it == this->_data.end()) {
                return std::nullopt;
            }

            return it->second;
        }

        /**
         * \brief Try to get value of concrete type from container or return nothing
         * \tparam T Type of value
         * \param key Key
         * \return Value of concrete type or nothing
         */
        template <typename T>
        [[nodiscard]] std::optional<T> tryGet(std::string_view &&key) const {
            const auto value = this->tryGet(std::forward<decltype(key)>(key));

            if (!value.has_value()) {
                return std::nullopt;
            }

            return std::get<T>(*value);
        }

        /**
         * \brief Get value from container
         * \param key Key
         * \return Value
         */
        [[nodiscard]] Value get(std::string_view &&key) const {
            return this->tryGet(std::forward<decltype(key)>(key)).value();
        }

        /**
         * \brief Get value of concrete type from container
         * \tparam T Type of value
         * \param key Key
         * \return Value of concrete type
         */
        template <typename T>
        [[nodiscard]] T get(std::string_view &&key) const {
            return this->tryGet<T>(std::forward<decltype(key)>(key)).value();
        }

        /**
         * \brief Merge another container into this
         * \param params Another parameters container
         * \return This instance
         */
        [[nodiscard]] Params &merge(const Params &params) {
            auto dataCopy = std::move(this->_data);

            this->_data = params._data;
            this->_data.merge(std::move(dataCopy));

            return *this;
        }

        /**
         * \brief Check container for emptiness
         * \return Container emptiness
         */
        [[nodiscard]] bool empty() const { return this->_data.empty(); }

    private:
        Container _data;
    };
}

#endif // PENROSE_COMMON_PARAMS_HPP
