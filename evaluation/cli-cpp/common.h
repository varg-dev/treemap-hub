
#pragma once


#include <vector>
#include <string>

#include <glm/common.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


const std::string & dataPath();

// Read raw binary file into a char vector (probably the fastest way).
std::vector<char> rawFromFile(const std::string & filePath);
std::vector<float> rawFromFileF(const std::string & filePath);

bool rawToFile(const std::string & filePath, const std::vector<char> & raw);

std::string textFromFile(const std::string & filePath);
