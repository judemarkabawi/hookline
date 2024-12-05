#include "util.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "util/misc.hpp"

namespace {
const std::filesystem::path shader_path = "../../assets/shaders/";
}

namespace hookline {
ShaderSource load_shader_file(const std::string& vertex_name,
                              const std::string& fragment_name) {
    std::filesystem::path vertex_path = shader_path / vertex_name;
    std::filesystem::path fragment_path = shader_path / fragment_name;

    std::ifstream vertex_file(hookline::data_path(vertex_path.string()));
    std::ifstream fragment_file(hookline::data_path(fragment_path.string()));

    if (!vertex_file.is_open()) {
        std::stringstream err;
        err << "Failed to load shader " << vertex_path << "\n";
        throw std::runtime_error(err.str());
    }
    if (!fragment_file.is_open()) {
        std::stringstream err;
        err << "Failed to load shader " << vertex_path << "\n";
        throw std::runtime_error(err.str());
    }

    std::string vertex_source((std::istreambuf_iterator<char>(vertex_file)),
                              std::istreambuf_iterator<char>());
    std::string fragment_source((std::istreambuf_iterator<char>(fragment_file)),
                                std::istreambuf_iterator<char>());

    std::cout << "Loaded shader with vertex " << vertex_name << " and fragment "
              << fragment_name << ".\n";

    return ShaderSource{
        vertex_source,
        fragment_source,
    };
}
}  // namespace hookline