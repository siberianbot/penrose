#ifndef PENROSE_ASSETS_MESH_ASSET_HPP
#define PENROSE_ASSETS_MESH_ASSET_HPP

#include <memory>

#include <Penrose/Assets/Asset.hpp>
#include <Penrose/Rendering/Objects/Buffer.hpp>

namespace Penrose {

    /**
     * \brief Mesh asset
     */
    class PENROSE_API MeshAsset final: public Asset {
    public:
        MeshAsset(std::shared_ptr<Buffer> &&vertexBuffer, std::shared_ptr<Buffer> &&indexBuffer)
            : _vertexBuffer(std::forward<decltype(vertexBuffer)>(vertexBuffer)),
              _indexBuffer(std::forward<decltype(indexBuffer)>(indexBuffer)) {
            //
        }

        ~MeshAsset() override = default;

        //! \copydoc Asset::getType
        [[nodiscard]] AssetType getType() const override { return AssetType::Mesh; }

        /**
         * \brief Get pointer to instance of Buffer with vertex data
         * \return instance of Buffer with vertex data
         */
        [[nodiscard]] std::weak_ptr<Buffer> getVertexBuffer() const { return this->_vertexBuffer; }

        /**
         * \brief Get pointer to instance of Buffer with index data
         * \return instance of Buffer with index data
         */
        [[nodiscard]] std::weak_ptr<Buffer> getIndexBuffer() const { return this->_indexBuffer; }

    private:
        std::shared_ptr<Buffer> _vertexBuffer;
        std::shared_ptr<Buffer> _indexBuffer;
    };
}

#endif // PENROSE_ASSETS_MESH_ASSET_HPP
