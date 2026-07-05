#include "lneng/AssetLoader.h"
#include <SDL3/SDL_log.h>
#include <stb_image.h>
#include <fastgltf/core.hpp>
#include <fastgltf/tools.hpp>
#include <stdexcept>

namespace lneng {

// clang-format off
// NOLINTNEXTLINE(readability-convert-member-functions-to-static, readability-function-cognitive-complexity)
auto AssetLoader::LoadGlb(const std::filesystem::path& path)
    // clang-format on
    -> ModelCreateInfo {
  // todo the model caching and stuff should probably live HERE not after ?
  fastgltf::Parser parser;

  auto data = fastgltf::GltfDataBuffer::FromPath(path);
  if (data.error() != fastgltf::Error::None) {
    throw std::runtime_error("Failed to load GLB: " + path.string());
  }

  auto asset = parser.loadGltfBinary(data.get(), path.parent_path(),
                                     fastgltf::Options::None);
  if (asset.error() != fastgltf::Error::None) {
    throw std::runtime_error("Failed to parse GLB: " + path.string());
  }

  ModelCreateInfo result;
  result.name = path.stem().string();

  for (auto& fgMesh : asset->meshes) {
    for (auto& primitive : fgMesh.primitives) {
      ModelCreateInfo::Mesh meshInfo;

      auto* posAttr = primitive.findAttribute("POSITION");
      if (posAttr == primitive.attributes.end()) {
        continue;
      }
      auto& posAccessor = asset->accessors[posAttr->accessorIndex];
      meshInfo.vertices.resize(posAccessor.count);

      fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(
          asset.get(), posAccessor, [&](fastgltf::math::fvec3 pos, size_t idx) {
            meshInfo.vertices[idx].pos = {pos.x(), pos.y(), pos.z()};
          });

      if (auto* normAttr = primitive.findAttribute("NORMAL");
          normAttr != primitive.attributes.end()) {
        fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(
            asset.get(), asset->accessors[normAttr->accessorIndex],
            [&](fastgltf::math::fvec3 n, size_t idx) {
              meshInfo.vertices[idx].normal = {n.x(), n.y(), n.z()};
            });
      }

      if (auto* uvAttr = primitive.findAttribute("TEXCOORD_0");
          uvAttr != primitive.attributes.end()) {
        fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec2>(
            asset.get(), asset->accessors[uvAttr->accessorIndex],
            [&](fastgltf::math::fvec2 uv, size_t idx) {
              meshInfo.vertices[idx].uv = {uv.x(), uv.y()};
            });
      }

      if (primitive.indicesAccessor.has_value()) {
        auto& indexAccessor =
            asset->accessors[primitive.indicesAccessor.value()];
        meshInfo.indices.reserve(indexAccessor.count);
        fastgltf::iterateAccessor<uint32_t>(
            asset.get(), indexAccessor,
            [&](uint32_t idx) { meshInfo.indices.push_back(idx); });
      }

      meshInfo.textureIndex = 0;
      if (primitive.materialIndex.has_value()) {
        auto& material = asset->materials[primitive.materialIndex.value()];
        if (material.pbrData.baseColorTexture.has_value()) {
          auto& tex =
              asset->textures[material.pbrData.baseColorTexture->textureIndex];
          if (tex.imageIndex.has_value()) {
            meshInfo.textureIndex =
                static_cast<uint32_t>(tex.imageIndex.value());
          }
        }
      }

      result.meshes.push_back(std::move(meshInfo));
    }
  }

  result.textures.reserve(asset->images.size());
  for (auto& fgImage : asset->images) {
    auto* inlineData =
        std::get_if<fastgltf::sources::BufferView>(&fgImage.data);
    if (inlineData == nullptr) {
      throw std::runtime_error("Only supports .glb files (for now)");
    }

    auto& bufferView = asset->bufferViews[inlineData->bufferViewIndex];
    auto& buffer = asset->buffers[bufferView.bufferIndex];

    std::visit(
        fastgltf::visitor{
            [](auto&) {
              throw std::runtime_error("Unsupported buffer source type");
            },
            [&](fastgltf::sources::Array& arr) {
              const std::byte* rawBytes =
                  arr.bytes.data() + bufferView.byteOffset;
              size_t byteLength = bufferView.byteLength;
              result.textures.push_back(decodeImageBytes(rawBytes, byteLength));
            },
            [&](fastgltf::sources::Vector& vec) {
              const std::byte* rawBytes =
                  vec.bytes.data() + bufferView.byteOffset;
              size_t byteLength = bufferView.byteLength;
              result.textures.push_back(decodeImageBytes(rawBytes, byteLength));
            },
        },
        buffer.data);
  }

  return result;
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
auto AssetLoader::LoadSkybox(const std::filesystem::path& xPosTexture,
                             const std::filesystem::path& xNegTexture,
                             const std::filesystem::path& yPosTexture,
                             const std::filesystem::path& yNegTexture,
                             const std::filesystem::path& zPosTexture,
                             const std::filesystem::path& zNegTexture)
    -> SkyboxCreateInfo {
  return {
      .textureXPos = decodeImageFile(xPosTexture),
      .textureXNeg = decodeImageFile(xNegTexture),
      .textureYPos = decodeImageFile(yPosTexture),
      .textureYNeg = decodeImageFile(yNegTexture),
      .textureZPos = decodeImageFile(zPosTexture),
      .textureZNeg = decodeImageFile(zNegTexture),
      .name = "",  // todo
  };
}

auto AssetLoader::decodeImageBytes(const std::byte* data, size_t size)
    -> Texture {
  int width;
  int height;
  int channels;
  unsigned char* pixels = stbi_load_from_memory(
      reinterpret_cast<const uint8_t*>(data), static_cast<int>(size), &width,
      &height, &channels, 4);
  if (pixels == nullptr) {
    throw std::runtime_error("stbi load failed");
  }

  Texture texture;
  texture.width = static_cast<uint32_t>(width);
  texture.height = static_cast<uint32_t>(height);

  size_t totalBytes = static_cast<size_t>(width) * height * 4;
  texture.pixelsRgba8.assign(pixels, pixels + totalBytes);

  stbi_image_free(pixels);

  return texture;
}

auto AssetLoader::decodeImageFile(const std::filesystem::path& path)
    -> Texture {
  int width;
  int height;
  int channels;
  unsigned char* pixels =
      stbi_load(path.string().c_str(), &width, &height, &channels, 4);
  if (pixels == nullptr) {
    throw std::runtime_error("stbi load failed: " + path.string());
  }

  Texture texture;
  texture.width = static_cast<uint32_t>(width);
  texture.height = static_cast<uint32_t>(height);
  size_t totalBytes = static_cast<size_t>(width) * height * 4;
  texture.pixelsRgba8.assign(pixels, pixels + totalBytes);

  stbi_image_free(pixels);
  return texture;
}

}  // namespace lneng
