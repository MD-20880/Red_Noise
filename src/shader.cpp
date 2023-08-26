#pragma once



#include <RayTriangleIntersection.h>

#include "shader.h"
#include "raytrace.h"

namespace Shader
{
        CanvasPoint calculateCanvasPos(int height,int width,glm::vec3 point,Camera::BasicCamera* camera){
        //first move camera to world position, and then rotate.
        // glm::vec3 newPoint = glm::inverse(camrot) * glm::vec3(point.x - campos.x,point.y-campos.y,point.z-campos.z);
        glm::vec3 newPoint = glm::inverse(camera->camrot_) * glm::vec3(camera->campos_.x - point.x,camera->campos_.y-point.y,camera->campos_.z-point.z);

        float x = camera->focal_ * ((newPoint.x)/(newPoint.z))*(-(float(height)*2.0/3.0)) +float(width)/2;
        float y = camera->focal_ * ((newPoint.y)/(newPoint.z))*(-(float(height)*2.0/3.0)) +float(width)/2;
        // printf("DBG:%f,%f POINT:%f,%f,%f\n",x,y,point.x,point.y,point.z);
        // return CanvasPoint(round(x),round(y),glm::length(camrot*glm::vec3(0,0,-1)*newPoint));
        return CanvasPoint(round(x),round(y),newPoint.z);
    }


    void LineShader(Scene::BasicScene* scene, Camera::BasicCamera* camera){
         for (ModelTriangle triangle : (*scene).triangles_){
                 std::array<glm::vec3, 3UL> vertices = triangle.vertices;

                //rotation matrix

                // glm::mat3 rot(1,0,0,0,0.7,0.7,0,-0.7,0.7);
                // glm::mat3 rot(1,0,0,0,1,0,0,0,1);
        

                CanvasTriangle triangle2d = CanvasTriangle(
                    calculateCanvasPos(camera->renderer_->getHeight(),
                                        camera->renderer_->getWidth(),
                                        vertices[0],camera),
                    calculateCanvasPos(camera->renderer_->getHeight(),
                                        camera->renderer_->getWidth(),
                                        vertices[1],camera),
                    calculateCanvasPos(camera->renderer_->getHeight(),
                                        camera->renderer_->getWidth(),
                                        vertices[2],camera)
                );
                
                // printf("top:\t(%f,%f)\nleft:\t(%f,%f)\nright:\t(%f,%f)\n",triangle2d.v0().x,triangle2d.v0().y,
                //                                                 triangle2d.v1().x,triangle2d.v1().y,
                //                                                 triangle2d.v2().x,triangle2d.v2().y);


                // drawFilledTriangle(triangle2d,triangle.colour,window);
                (*(*camera).renderer_).drawTriangle(triangle2d,Colour(123,123,123));

            }
    }



    void RasterizeShader(Scene::BasicScene* scene, Camera::BasicCamera* camera){
         for (ModelTriangle triangle : (*scene).triangles_){
                 std::array<glm::vec3, 3UL> vertices = triangle.vertices;

                //rotation matrix

                // glm::mat3 rot(1,0,0,0,0.7,0.7,0,-0.7,0.7);
                // glm::mat3 rot(1,0,0,0,1,0,0,0,1);
        

                CanvasTriangle triangle2d = CanvasTriangle(
                    calculateCanvasPos(camera->renderer_->getHeight(),
                                        camera->renderer_->getWidth(),
                                        vertices[0],camera),
                    calculateCanvasPos(camera->renderer_->getHeight(),
                                        camera->renderer_->getWidth(),
                                        vertices[1],camera),
                    calculateCanvasPos(camera->renderer_->getHeight(),
                                        camera->renderer_->getWidth(),
                                        vertices[2],camera)
                );
                
                // printf("top:\t(%f,%f)\nleft:\t(%f,%f)\nright:\t(%f,%f)\n",triangle2d.v0().x,triangle2d.v0().y,
                //                                                 triangle2d.v1().x,triangle2d.v1().y,
                //                                                 triangle2d.v2().x,triangle2d.v2().y);


                // drawFilledTriangle(triangle2d,triangle.colour,window);
                (*(*camera).renderer_).drawFilledTriangle(triangle2d,triangle.colour);

            }
    } 



    
    glm::vec3 calculateDirection(float x, float y, float height, float width, Camera::BasicCamera* camera){
        float focal = 2.0;
        float factor = 1.0/(height*2.0/3.0);

        // float xres = (x-WIDTH/2)*(campos.z + focal)/(factor*focal);
        // float yres = (y+HEIGHT/2)*(campos.z + focal) /(factor*focal);
        float xres = (x-width/2.0) * factor;
        float yres = (y-height/2.0) * factor;

        //assert(xres != 0 && yres != 0);
        glm::vec3 result(xres,yres,-focal);
        return camera->camrot_*result;

    }

    void RaytracingShader(Scene::BasicScene* scene, Camera::BasicCamera* camera){
	for( int y = 0 ; y < camera->renderer_->getHeight(); y++){
		for ( int x = 0; x < camera->renderer_->getWidth(); x++){
			//Send Ray
			glm::vec3 start = camera->campos_;
			glm::vec3 raydirection = glm::normalize(calculateDirection(x,y,camera->renderer_->getHeight(),camera->renderer_->getWidth(),camera));
			RayTriangleIntersection camintersect =  RayTracing::getClosestIntersection(start,raydirection,scene->triangles_);

			//If hit
			if (camintersect.triangleIndex != -1){
				
				Colour original = camintersect.intersectedTriangle.colour;
				glm::vec3 pNormal = camintersect.intersectedTriangle.normal;
				float ambient = 0.2;
				float lightIntensity = 0.5;
				float diffuseIntensity = 0.0;
                float proximityLighting = 0.0;
				float specularIntensity = 0.0;
				float visibility = 1.0;

				if (scene->reflectiveTriangles_.find(camintersect.triangleIndex) != scene->reflectiveTriangles_.end()){
					raydirection = camintersect.intersectionPoint-start;
					raydirection = raydirection - 2.0f * pNormal*(glm::dot(raydirection,pNormal));
					start = camintersect.intersectionPoint;
					camintersect = RayTracing::getClosestIntersection(camintersect.intersectionPoint + raydirection*0.01f,raydirection,scene->triangles_);
					pNormal = camintersect.intersectedTriangle.normal;
					original = camintersect.intersectedTriangle.colour;
				}

                std::vector<glm::vec3> lightList = scene->pointLights_;
				for(int i = 0; i < lightList.size(); i++){
                    
					glm::vec3 lights = lightList[i];
					RayTriangleIntersection lightintersect = RayTracing::getClosestIntersection(lights,camintersect.intersectionPoint-lights,scene->triangles_);
					
					// if(equalPoint(lightintersect.intersectionPoint, camintersect.intersectionPoint,0.001)){
                    if (lightintersect.triangleIndex == camintersect.triangleIndex && lightintersect.triangleIndex != -1){
                        ModelTriangle triangle = lightintersect.intersectedTriangle;
                        glm::vec3 vectorToLight = glm::normalize(lights - lightintersect.intersectionPoint);
                        glm::vec3 vectorFromLight = glm::normalize( lightintersect.intersectionPoint- lights);
                        glm::vec3 reflection = vectorFromLight - 2.0f * pNormal*(glm::dot(vectorFromLight,pNormal));
                        
                        
                        proximityLighting += (3/(4*3.14*pow(lightintersect.distanceFromCamera,2.0)))*lightIntensity;
                        diffuseIntensity += glm::dot(pNormal,vectorToLight)*lightIntensity;
                        specularIntensity += pow(std::max(glm::dot(reflection,glm::normalize(start-lightintersect.intersectionPoint)),0.0f),256)*lightIntensity;

                    }
                    // }
				}
				lightIntensity /= float(lightList.size());
				diffuseIntensity /= float(lightList.size());
				specularIntensity /= float(lightList.size());




				
				glm::vec3 colour(original.red,original.green,original.blue);
				// glm::vec3 resultColor = colour*(diffuseIntensity+ambient+specularIntensity)*visibility;
                glm::vec3 resultColor = colour*(proximityLighting+diffuseIntensity+ambient+specularIntensity)*visibility;
				// Colour pixel = Colour(std::min(resultColor.x,255.0f),std::min(resultColor.y,255.0f),std::min(resultColor.z,255.0f));
				// Colour pixel = Colour(std::min(colour.x,255.0f),std::min(colour.y,255.0f),std::min(colour.z,255.0f));
				Colour pixel = Colour(std::min(resultColor.x,255.0f),std::min(resultColor.y,255.0f),std::min(resultColor.z,255.0f));


				// pbuffer[y][x] = fromColour(camintersect.intersectedTriangle.colour);
				(*camera->renderer_->getpbuffer())[y][x] = Utils::fromColour(pixel);
			}
			
		}
	}

}


std::vector<glm::vec3> gs_calculate_vertex_value(ModelTriangle triangle, Scene::BasicScene* scene, Camera::BasicCamera* camera){
	std::vector<glm::vec3> result;
	std::array<glm::vec3,3UL> vertices = triangle.vertices;
	for(int v = 0; v < triangle.vertices.size(); v++){
		glm::vec3 colour(0,0,0);
		Colour original = triangle.colour;
		colour = glm::vec3(original.red,original.green,original.blue);
		glm::vec3 pNormal = scene->vertexNormalMap_[vertices[v]];

		// std::cout << glm::to_string(pNormal) << std::endl;
		float ambient = 0.2;
		float lightIntensity = 1.0;
		float diffuseIntensity = 0.0;
		float specularIntensity = 0.0;
		float visibility = 1.0;
		for(int i = 0; i < scene->pointLights_.size(); i++){
				glm::vec3 lights = scene->pointLights_[i];
				RayTriangleIntersection lightintersect = RayTracing::getClosestIntersection(lights,vertices[v]-lights,scene->triangles_);
			
				
					
				glm::vec3 vectorToLight = glm::normalize(lights - vertices[v]);
				glm::vec3 vectorFromLight = glm::normalize( vertices[v]- lights);
				glm::vec3 reflection = vectorFromLight- 2.0f * pNormal*(glm::dot(vectorFromLight,pNormal));

				lightIntensity += (1/(4*3.14*pow(lightintersect.distanceFromCamera,2.0)));

				diffuseIntensity += std::max(glm::dot(pNormal,vectorToLight),0.0f);

				specularIntensity += pow(std::max(glm::dot(reflection,glm::normalize(camera->campos_-lightintersect.intersectionPoint)),0.0f),256);
					
			}
		lightIntensity /= float(scene->pointLights_.size());
		diffuseIntensity /= float(scene->pointLights_.size());
		specularIntensity /= float(scene->pointLights_.size());
		colour = colour*(diffuseIntensity+ambient+specularIntensity)*visibility;

		colour = glm::vec3(std::min(colour.x,255.0f),std::min(colour.y,255.0f),std::min(colour.z,255.0f));
		result.push_back(colour);
	}
	return result;
}

void GourandShader(Scene::BasicScene* scene, Camera::BasicCamera* camera){

	std::unordered_map<int,std::vector<glm::vec3>> triangleVertexColourMap;
	for( int y = 0 ; y < camera->renderer_->getHeight(); y++){
		for ( int x = 0; x < camera->renderer_->getWidth(); x++){
			//Send Ray
			glm::vec3 raydirection = glm::normalize(calculateDirection(x,y,camera->renderer_->getHeight(),camera->renderer_->getWidth(),camera));
			RayTriangleIntersection camintersect =  RayTracing::getClosestIntersection(camera->campos_,raydirection,scene->triangles_);

			//If hit
			if (camintersect.triangleIndex != -1){
				std::vector<glm::vec3> vertexValue;
				vertexValue = gs_calculate_vertex_value(camintersect.intersectedTriangle,scene,camera);

				// if (triangleVertexColourMap.find(camintersect.triangleIndex) != triangleVertexColourMap.end()){
				// 	vertexValue = triangleVertexColourMap[camintersect.triangleIndex];
				// }else{
				// 	vertexValue = gs_calculate_vertex_value(camintersect.intersectedTriangle,campos,list,lightList,vertexNormalMap);
				// 	triangleVertexColourMap[camintersect.triangleIndex] = vertexValue;
				// }

				glm::vec3 colour(0,0,0);
				glm::vec3 colourE0 = vertexValue[1] - vertexValue[0];
				glm::vec3 colourE1 = vertexValue[2] - vertexValue[0];

				colour = vertexValue[0] + colourE0*camintersect.possibleSolution.y + colourE1*camintersect.possibleSolution.z;
				glm::vec3 resultColor = colour;

				Colour pixel = Colour(std::min(resultColor.x,255.0f),std::min(resultColor.y,255.0f),std::min(resultColor.z,255.0f));
				(*camera->renderer_->getpbuffer())[y][x] = Utils::fromColour(pixel);
			}
			
		}
	}

}

glm::vec3 pointNormal(RayTriangleIntersection intersection, Scene::BasicScene* scene){
    auto vertexNormalMap = scene->vertexNormalMap_;
	glm::vec3 vertexNormal(0.0,0.0,0.0);
	std::array<glm::vec3,3UL> vertices = intersection.intersectedTriangle.vertices;
	glm::vec3 e0 = vertexNormalMap[vertices[1]] - vertexNormalMap[vertices[0]];
	glm::vec3 e1 = vertexNormalMap[vertices[2]] - vertexNormalMap[vertices[0]];
	vertexNormal = vertexNormalMap[vertices[0]] + e0*intersection.possibleSolution.y + e1*intersection.possibleSolution.z;

	return glm::normalize(vertexNormal);

}	

void PhongShader(Scene::BasicScene* scene, Camera::BasicCamera* camera){
	for( int y = 0 ; y < camera->renderer_->getHeight(); y++){
		for ( int x = 0; x < camera->renderer_->getWidth(); x++){
			//Send Ray
			glm::vec3 raydirection = glm::normalize(calculateDirection(x,y,camera->renderer_->getHeight(),camera->renderer_->getWidth(),camera));
			RayTriangleIntersection camintersect =  RayTracing::getClosestIntersection(camera->campos_,raydirection,scene->triangles_);

			//If hit
			if (camintersect.triangleIndex != -1){
				Colour original = camintersect.intersectedTriangle.colour;
				glm::vec3 pNormal = pointNormal(camintersect,scene);
				float ambient = 0.2;
				float lightIntensity = 0.0;
				float diffuseIntensity = 0.0;
				float specularIntensity = 0.0;
				float visibility = 1.0;
				for(int i = 0; i < scene->pointLights_.size(); i++){
					glm::vec3 lights = scene->pointLights_[i];
					RayTriangleIntersection lightintersect = RayTracing::getClosestIntersection(lights,camintersect.intersectionPoint-lights,scene->triangles_);
					// if(equalPoint(lightintersect.intersectionPoint, camintersect.intersectionPoint,0.001)){
						ModelTriangle triangle = lightintersect.intersectedTriangle;
						glm::vec3 vectorToLight = glm::normalize(lights - lightintersect.intersectionPoint);
						glm::vec3 vectorFromLight = glm::normalize( lightintersect.intersectionPoint- lights);
						glm::vec3 reflection = vectorFromLight- 2.0f * pNormal*(glm::dot(vectorFromLight,pNormal));
						
						
						lightIntensity += (1/(4*3.14*pow(lightintersect.distanceFromCamera,2.0)));
						diffuseIntensity += std::max(glm::dot(pNormal,vectorToLight),0.0f);
						specularIntensity += pow(std::max(glm::dot(reflection,glm::normalize(camera->campos_-lightintersect.intersectionPoint)),0.0f),256);
						// std::cout << diffuseIntensity << std::endl;

					
					// }	
				}
				lightIntensity /= float(scene->pointLights_.size());
				diffuseIntensity /= float(scene->pointLights_.size());
				specularIntensity /= float(scene->pointLights_.size());




				glm::vec3 colour = glm::vec3(original.red,original.green,original.blue);
					
				glm::vec3 resultColor = colour*(diffuseIntensity+ambient+specularIntensity)*visibility;

				Colour pixel = Colour(std::min(resultColor.x,255.0f),std::min(resultColor.y,255.0f),std::min(resultColor.z,255.0f));

				// pbuffer[y][x] = fromColour(camintersect.intersectedTriangle.colour);
				(*camera->renderer_->getpbuffer())[y][x] = Utils::fromColour(pixel);
			}
			
		}
	}

}






} // namespace Shader
