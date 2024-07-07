#pragma once

#include <optional>

#include <tiny_gltf.h>

namespace eng::gltf {

std::optional<tinygltf::Model> load(const std::string &path);

}
