#ifndef PENROSE_RENDERING_RENDER_OPERATOR_INFO_HPP
#define PENROSE_RENDERING_RENDER_OPERATOR_INFO_HPP

#include <string>

#include <Penrose/Common/ParamsCollection.hpp>

namespace Penrose {

    class RenderOperatorInfo {
    public:
        explicit RenderOperatorInfo(std::string &&name,
                                    ParamsCollection &&params = {})
                : _name(name),
                  _params(params) {
            //
        }

        [[nodiscard]] RenderOperatorInfo &setName(const std::string &name) {
            this->_name = name;

            return *this;
        }

        [[nodiscard]] RenderOperatorInfo &setName(std::string &&name) {
            this->_name = name;

            return *this;
        }

        [[nodiscard]] RenderOperatorInfo &setParams(const ParamsCollection &params) {
            this->_params = params;

            return *this;
        }

        [[nodiscard]] RenderOperatorInfo &setParams(ParamsCollection &&params) {
            this->_params = params;

            return *this;
        }

        [[nodiscard]] RenderOperatorInfo &withParameter(const ParamsCollection::KeyView &key,
                                                        const ParamsCollection::Value &value) {
            this->_params.set(key, value);

            return *this;
        }

        [[nodiscard]] const std::string &getName() const { return this->_name; }

        [[nodiscard]] const ParamsCollection &getParams() const { return this->_params; }

        [[nodiscard]] bool operator==(const RenderOperatorInfo &rhs) const = default;

    private:
        std::string _name;
        ParamsCollection _params;
    };
}

#endif // PENROSE_RENDERING_RENDER_OPERATOR_INFO_HPP
