
#include "common.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <algorithm>

#include <glm/vec4.hpp>
#include <glm/exponential.hpp>
#include <glm/geometric.hpp>

#include <cpplocate/cpplocate.h>


namespace
{


std::string determineDataPath()
{
    std::string path = cpplocate::locatePath("data/shaders/visualization.frag", "share/treemap-demo/data/shaders/visualization.frag", reinterpret_cast<void *>(&dataPath));
    if (path.empty()) path = "./data";
    else              path = path + "data";

    return path;
}


} // namespace


const std::string & dataPath()
{
    static const auto path = determineDataPath();

    return path;
}

std::vector<char> rawFromFile(const std::string & filePath)
{
    auto stream = std::ifstream(filePath, std::ios::in | std::ios::binary | std::ios::ate);

    if (!stream)
    {
        std::cerr << "Reading from file '" << filePath << "' failed." << std::endl;
        return std::vector<char>();
    }

    stream.seekg(0, std::ios::end);

    const auto size = stream.tellg();
    auto raw = std::vector<char>(size);

    stream.seekg(0, std::ios::beg);
    stream.read(raw.data(), size);

    return raw;
}

std::vector<float> rawFromFileF(const std::string & filePath)
{
    auto stream = std::ifstream(filePath.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

    if (!stream)
    {
        std::cerr << "Reading from file '" << filePath << "' failed." << std::endl;
        return std::vector<float>();
    }

    stream.seekg(0, std::ios::end);

    const auto size = stream.tellg();
    auto raw = std::vector<float>(size / sizeof(float));

    stream.seekg(0, std::ios::beg);
    stream.read(reinterpret_cast<char *>(raw.data()), (size / sizeof(float)) * sizeof(float));

    return raw;
}

std::string textFromFile(const std::string & filePath)
{
    const auto text = rawFromFile(filePath);
    return std::string(text.begin(), text.end());
}

bool rawToFile(const char * filePath, const std::vector<char> & raw)
{
    auto stream = std::ofstream(filePath, std::ios::out | std::ios::binary);

    if (!stream)
    {
        std::cerr << "Writing to file '" << filePath << "' failed." << std::endl;
        return false;
    }

    stream.write(raw.data(), raw.size());

    return true;
}
