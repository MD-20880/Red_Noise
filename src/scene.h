#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <TextureMap.h>
#include <ModelTriangle.h>
#include <set>


namespace Scene{

class BasicScene{
public:
    std::vector<ModelTriangle> triangles_;
    std::vector<glm::vec3> pointLights_;
    std::unordered_map<glm::vec3,glm::vec3> vertexNormalMap_;
    std::unordered_map<glm::vec3,std::vector<ModelTriangle>> vertexTriangleMap_;
    std::set<int> reflectiveTriangles_;
    
    BasicScene(char* objFileName);
    BasicScene(char* objFileName, char* mtlFileName);
    BasicScene(char* objFileName, char* mtlFileName,float scaling);



};

void calculateNormal(std::vector<ModelTriangle> &list);

//storing Mapping relationship between vertex and connected triangles
std::unordered_map<glm::vec3,std::vector<ModelTriangle>> calculateVertexTriangleMap(std::vector<ModelTriangle> triangles);
std::unordered_map<glm::vec3, glm::vec3> calculateVertexNormalMap(std::unordered_map<glm::vec3,std::vector<ModelTriangle>> vertexTriangleMap);



}