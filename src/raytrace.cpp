

#include "raytrace.h"


namespace RayTracing{
    RayTriangleIntersection getClosestIntersection(glm::vec3 campos, glm::vec3 direction, std::vector<ModelTriangle> triangles){
	RayTriangleIntersection result(glm::vec3(0,0,0), FLT_MAX, ModelTriangle(glm::vec3(0,0,0),glm::vec3(0,0,0),glm::vec3(0,0,0),Colour(0,0,0)),-1);
	for (int i = 0; i < triangles.size(); i++){
		glm::vec3 e0 = triangles[i].vertices[1] - triangles[i].vertices[0];
		glm::vec3 e1 = triangles[i].vertices[2] - triangles[i].vertices[0];
		glm::vec3 SPVector = campos - triangles[i].vertices[0];
		glm::mat3 DEMatrix(-direction, e0, e1);
		glm::vec3 possibleSolution = glm::inverse(DEMatrix) * SPVector;
		if (possibleSolution[1] >= 0.0 && possibleSolution[1] <= 1.0 &&
			possibleSolution[2] >= 0.0 && possibleSolution[2] <= 1.0 &&
			possibleSolution[1] + possibleSolution[2] <= 1.0 &&
			possibleSolution[0] >= 0.0){

			if (result.distanceFromCamera>possibleSolution.x){
			// std::cout << "get three " << std::endl;
			result.distanceFromCamera = possibleSolution.x;
			glm::vec3 intersectionPoint = triangles[i].vertices[0] + e0*possibleSolution.y + e1*possibleSolution.z;
			// glm::vec3 intersectionPiont2 = campos + direction*possibleSolution.x;
			// assert(intersectionPoint1 == intersectionPiont2);
			result.intersectionPoint = intersectionPoint;
			result.intersectedTriangle = triangles[i];
			result.triangleIndex = i;
			result.possibleSolution = possibleSolution;
		}

	}
			//std::cout << possibleSolution.x <<" " << possibleSolution.y <<" "  << possibleSolution.z <<" " <<std::endl;
			
		}

		
	return result;
}
}