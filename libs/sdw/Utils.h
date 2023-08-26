#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

std::vector<std::string> split(const std::string &line, char delimiter);

bool equalPoint(glm::vec3 point1, glm::vec3 point2, float diff);