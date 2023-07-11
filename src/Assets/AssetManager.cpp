#include <Penrose/Assets/AssetManager.hpp>

#include <fmt/core.h>
#include <stb/stb_image.h>
#include <tiny_obj_loader.h>

#include <Penrose/Assets/AssetDictionary.hpp>
#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Common/Vertex.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Utils/DestructibleObject.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

#include "src/Assets/AssetReader.hpp"
#include "src/Rendering/DeviceContext.hpp"
#include "src/Builtin/Assets/VkImageAsset.hpp"
#include "src/Builtin/Assets/VkMeshAsset.hpp"
#include "src/Builtin/Assets/VkShaderAsset.hpp"

namespace Penrose {

    AssetManager::AssetManager(ResourceSet *resources)
            : _assetDictionary(resources->get<AssetDictionary>()),
              _deviceContext(resources->get<DeviceContext>()) {
        //
    }

    void AssetManager::init() {
        this->_loadingThread = std::jthread([this](const std::stop_token &stopToken) {
            while (!stopToken.stop_requested()) {
                auto lock = std::lock_guard<std::mutex>(this->_loadingMutex);

                while (!this->_loadingQueue.empty()) {
                    auto asset = this->_loadingQueue.front();
                    this->_loadingQueue.pop();

                    this->tryLoadAsset(asset);
                }
            }
        });
    }

    void AssetManager::destroy() {
        if (this->_loadingThread.has_value()) {
            auto &thread = this->_loadingThread.value();

            thread.request_stop();

            if (thread.joinable()) {
                thread.join();
            }

            this->_loadingThread = std::nullopt;
        }

        this->_assets.clear();

        while (!this->_loadingQueue.empty()) {
            this->_loadingQueue.pop();
        }
    }

    void AssetManager::queueImageLoading(const std::string &asset) {
        auto lock = std::lock_guard<std::mutex>(this->_loadingMutex);

        auto entry = AssetManager::Entry{
                .type = AssetType::Image,
                .state = LoadingState::Pending,
                .ptr = nullptr
        };

        this->_assets.emplace(asset, entry);
        this->_loadingQueue.push(asset);
    }

    void AssetManager::queueMeshLoading(const std::string &asset) {
        auto lock = std::lock_guard<std::mutex>(this->_loadingMutex);

        auto entry = AssetManager::Entry{
                .type = AssetType::Mesh,
                .state = LoadingState::Pending,
                .ptr = nullptr
        };

        this->_assets.emplace(asset, entry);
        this->_loadingQueue.push(asset);
    }

    void AssetManager::queueShaderLoading(const std::string &asset) {
        auto lock = std::lock_guard<std::mutex>(this->_loadingMutex);

        auto entry = AssetManager::Entry{
                .type = AssetType::Shader,
                .state = LoadingState::Pending,
                .ptr = nullptr
        };

        this->_assets.emplace(asset, entry);
        this->_loadingQueue.push(asset);
    }

    bool AssetManager::isLoaded(const std::string &asset) const {
        auto it = this->_assets.find(asset);

        if (it == this->_assets.end()) {
            return false;
        }

        return it->second.state == LoadingState::Loaded;
    }

    void AssetManager::unload(const std::string &asset) {
        this->_assets.erase(asset);
    }

    void AssetManager::tryLoadAsset(const std::string &asset) {
        if (this->isLoaded(asset)) {
            return;
        }

        auto maybePath = this->_assetDictionary->tryGetPath(asset);
        if (!maybePath.has_value()) {
            // TODO: log
            return;
        }

        const auto &it = this->_assets.find(asset);
        if (it == this->_assets.end()) {
            // TODO: log
            return;
        }

        try {
            switch (it->second.type) {
                case AssetType::Shader:
                    it->second.ptr = this->loadShaderByPath(std::move(*maybePath));
                    break;

                case AssetType::Mesh:
                    it->second.ptr = this->loadMeshByPath(std::move(*maybePath));
                    break;

                case AssetType::Image:
                    it->second.ptr = this->loadImageByPath(std::move(*maybePath));
                    break;

                default:
                    throw EngineError("Not supported type");
            }

            it->second.state = LoadingState::Loaded;
        } catch (const std::exception &error) {
            it->second.state = LoadingState::Failed;

            // TODO: log
            return;
        }
    }

    std::shared_ptr<ShaderAsset> AssetManager::loadShaderByPath(std::filesystem::path &&path) {
        auto assetReader = AssetReader::read(std::move(path));

        return std::shared_ptr<ShaderAsset>(makeVkShaderAsset(this->_deviceContext,
                                                              reinterpret_cast<const std::uint32_t *>(assetReader.data()),
                                                              assetReader.size()));
    }

    std::shared_ptr<MeshAsset> AssetManager::loadMeshByPath(std::filesystem::path &&path) {
        auto assetReader = AssetReader::read(std::move(path));
        auto assetText = std::string(reinterpret_cast<const char *>(assetReader.data()), assetReader.size());

        auto objReader = tinyobj::ObjReader();
        if (!objReader.ParseFromString(assetText, "") || !objReader.Valid()) {
            throw EngineError(fmt::format("Failed to read mesh: {}", objReader.Error()));
        }

        // TODO: pass warning

        auto &attrib = objReader.GetAttrib();
        auto &shapes = objReader.GetShapes();
        std::vector<Vertex> vertices;
        std::vector<std::uint32_t> indices;

        for (const auto &shape: shapes) {
            for (const auto &index: shape.mesh.indices) {
                indices.push_back(vertices.size());

                auto vertex = Vertex{
                        .pos = {
                                attrib.vertices.at(3 * index.vertex_index + 0),
                                attrib.vertices.at(3 * index.vertex_index + 1),
                                attrib.vertices.at(3 * index.vertex_index + 2)
                        },
                        .normal = glm::vec3(0),
                        .color = glm::vec3(1),
                        .uv = glm::vec2(0)
                };

                if (index.normal_index >= 0) {
                    vertex.normal = {
                            attrib.normals.at(3 * index.normal_index + 0),
                            attrib.normals.at(3 * index.normal_index + 1),
                            attrib.normals.at(3 * index.normal_index + 2)
                    };
                }

                if (index.texcoord_index >= 0) {
                    vertex.uv = {
                            attrib.texcoords.at(2 * index.texcoord_index + 0),
                            attrib.texcoords.at(2 * index.texcoord_index + 1)
                    };
                }

                if (!attrib.colors.empty()) {
                    vertex.color = {
                            attrib.colors.at(3 * index.vertex_index + 0),
                            attrib.colors.at(3 * index.vertex_index + 1),
                            attrib.colors.at(3 * index.vertex_index + 2)
                    };
                }

                vertices.push_back(vertex);
            }
        }

        return std::shared_ptr<MeshAsset>(makeVkMeshAsset(this->_deviceContext,
                                                          std::move(vertices),
                                                          std::move(indices)));
    }

    std::shared_ptr<ImageAsset> AssetManager::loadImageByPath(std::filesystem::path &&path) {
        auto assetReader = AssetReader::read(std::move(path));

        int width, height, channels;
        auto data = stbi_load_from_memory(reinterpret_cast<const stbi_uc *>(assetReader.data()),
                                          static_cast<int>(assetReader.size()),
                                          &width, &height, &channels, STBI_rgb_alpha);

        if (data == nullptr) {
            throw EngineError("Image file is invalid");
        }

        auto destructibleData = DestructibleObject<stbi_uc *>(data, [](stbi_uc *instance) {
            stbi_image_free(instance);
        });

        return std::shared_ptr<ImageAsset>(makeVkImageAsset(this->_deviceContext, width, height, 4, data));
    }

    template<>
    std::optional<std::shared_ptr<Asset>> AssetManager::tryGetAsset<Asset>(const std::string &asset) const {
        auto it = this->_assets.find(asset);

        if (it == this->_assets.end()) {
            return std::nullopt;
        }

        if (it->second.state != LoadingState::Loaded && it->second.state != LoadingState::Failed) {
            while (it->second.state != LoadingState::Loaded && it->second.state != LoadingState::Failed) {
                // wait
            }
        }

        if (it->second.state == LoadingState::Failed) {
            return std::nullopt;
        }

        return it->second.ptr;
    }
}
