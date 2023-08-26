#include "scene.h"
#include <map>
#include <string>

#include <Colour.h>

#include "utils.h"

namespace Scene
{

    BasicScene::BasicScene(char* objFileName){
        BasicScene(objFileName,{},0.35);
    }

    BasicScene::BasicScene(char* objFileName, char* mtlFileName){
        BasicScene(objFileName,mtlFileName,0.35);
    }
    
    BasicScene::BasicScene(char* objFileName, char* mtlFileName,float scaling){
        auto materials = Utils::readMtl(mtlFileName);
        triangles_ = Utils::readObj(objFileName,materials,scaling);
        calculateNormal(triangles_);
        vertexTriangleMap_ = calculateVertexTriangleMap(triangles_);
        vertexNormalMap_ = calculateVertexNormalMap(vertexTriangleMap_);
        pointLights_ = {glm::vec3(0.5,0.5,0),

        };
        reflectiveTriangles_ = std::set<int>({8,9});
    }
    




	void calculateNormal(std::vector<ModelTriangle> &list){
		for (int i = 0; i <list.size(); i++){
			ModelTriangle triangle = list[i];
			list[i].normal = glm::normalize(glm::cross(triangle.vertices[1] - triangle.vertices[0],triangle.vertices[2] - triangle.vertices[0]));
		}
	}

    std::unordered_map<glm::vec3,std::vector<ModelTriangle>> calculateVertexTriangleMap(std::vector<ModelTriangle> triangles){
        std::unordered_map<glm::vec3,std::vector<ModelTriangle>> vertexMap; 
        for (ModelTriangle triangle : triangles){
            for (glm::vec3 vertex : triangle.vertices){
                vertexMap[vertex].push_back(triangle);
            }
        }
        return vertexMap;
    }

    std::unordered_map<glm::vec3, glm::vec3> calculateVertexNormalMap(std::unordered_map<glm::vec3,std::vector<ModelTriangle>> vertexTriangleMap){
        std::unordered_map<glm::vec3, glm::vec3> result;
        for (auto element : vertexTriangleMap){
            glm::vec3 vertexNorm(0,0,0);
            for (ModelTriangle triangle: element.second){
                vertexNorm += triangle.normal;
            }
            result[element.first] = glm::normalize(vertexNorm);
        }
        return result;
    }


} // namespace Scene
