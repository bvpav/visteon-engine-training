#include "gltf.hh"

#include <iostream>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>

namespace eng::gltf {

std::optional<tinygltf::Model> load(const std::string &path)
{
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool success = loader.LoadASCIIFromFile(&model, &err, &warn, path);
    if (!warn.empty())
        std::cerr << "warning loading gltf: \n"
                  << warn << '\n';
    if (!err.empty())
        std::cerr << "error loading gltf: \n"
                  << err << '\n';
    if (!success)
        return std::nullopt;

    return std::move(model);
}

}