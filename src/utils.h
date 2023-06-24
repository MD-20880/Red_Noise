#include <vector>

#include <Colour.h>
#include <glm/glm.hpp>


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


/*Global Function*/
uint32_t fromColour(Colour colour);



}