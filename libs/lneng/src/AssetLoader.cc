#include "lneng/AssetLoader.h"
#include <fastgltf/core.hpp>
#include <fastgltf/tools.hpp>
#include <stdexcept>

namespace lneng {

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
auto AssetLoader::LoadGlb(const std::filesystem::path& path) -> GltfAsset {
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

  GltfAsset result;

  for (auto& fgMesh : asset->meshes) {
    MeshInfo meshInfo;
    meshInfo.cacheKey = std::string(fgMesh.name);

    for (auto& primitive : fgMesh.primitives) {
      size_t initialVtx = meshInfo.vertices.size();

      if (primitive.indicesAccessor.has_value()) {
        auto& indexAccessor =
            asset->accessors[primitive.indicesAccessor.value()];
        meshInfo.indices.reserve(meshInfo.indices.size() + indexAccessor.count);
        fastgltf::iterateAccessor<uint32_t>(
            asset.get(), indexAccessor, [&](uint32_t idx) {
              meshInfo.indices.push_back(idx + initialVtx);
            });
      }

      auto* posAttr = primitive.findAttribute("POSITION");
      if (posAttr == primitive.attributes.end()) {
        continue;
      }
      auto& posAccessor = asset->accessors[posAttr->accessorIndex];
      meshInfo.vertices.resize(initialVtx + posAccessor.count);
      fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(
          asset.get(), posAccessor, [&](fastgltf::math::fvec3 pos, size_t idx) {
            meshInfo.vertices[initialVtx + idx].pos = {pos.x(), pos.y(),
                                                       pos.z()};
          });

      auto* normAttr = primitive.findAttribute("NORMAL");
      if (normAttr != primitive.attributes.end()) {
        fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(
            asset.get(), asset->accessors[normAttr->accessorIndex],
            [&](fastgltf::math::fvec3 n, size_t idx) {
              meshInfo.vertices[initialVtx + idx].normal = {n.x(), n.y(),
                                                            n.z()};
            });
      }

      auto* uvAttr = primitive.findAttribute("TEXCOORD_0");
      if (uvAttr != primitive.attributes.end()) {
        fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec2>(
            asset.get(), asset->accessors[uvAttr->accessorIndex],
            // NOLINTNEXTLINE(readability-identifier-length)
            [&](fastgltf::math::fvec2 uv, size_t idx) {
              meshInfo.vertices[initialVtx + idx].uv = {uv.x(), uv.y()};
            });
      }
    }

    result.meshes.push_back(std::move(meshInfo));
  }

  return result;
}

}  // namespace lneng
