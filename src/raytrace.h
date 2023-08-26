#pragma once

#include <RayTriangleIntersection.h>



namespace RayTracing{
    RayTriangleIntersection getClosestIntersection(glm::vec3 campos, glm::vec3 direction, std::vector<ModelTriangle> triangles);
}