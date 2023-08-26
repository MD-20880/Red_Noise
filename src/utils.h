#pragma once

#include <vector>
#include <map>

#include <Colour.h>
#include <ModelTriangle.h>
#include <glm/glm.hpp>
#include <CanvasPoint.h>


namespace Utils{
/**
 * @brief Interpolate from start to end
 * 
 * @param start Start of the interpolation
 * @param end End of the interpolation
 * @param steps Number of values need to be calculated
 * 
 */
std::vector<float> interpolate(float start, float end, int steps);


/**
 * @brief Interpolate from start to end
 * 
 * @param start Start of the interpolation
 * @param end End of the interpolation
 * @param steps Number of values need to be calculated
 */
std::vector<glm::vec3> interpolate(glm::vec3 start, glm::vec3 end, int steps);



glm::vec3 GetBarycentricCoord(CanvasPoint P1, CanvasPoint P2, CanvasPoint P3, CanvasPoint P);

/*Global Function*/
uint32_t fromColour(Colour colour);
Colour getColour(uint32_t colour);

std::map<std::string,Colour> readMtl(std::string filepath);
std::vector<ModelTriangle> readObj(std::string filepath,std::map<std::string,Colour> mtlmap,float scaling);

}