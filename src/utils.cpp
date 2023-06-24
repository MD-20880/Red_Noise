#include "utils.h"

namespace Utils{
    std::vector<float> interpolate(float start, float end, int steps){
		std::vector<float> result;
		float step = (end - start) / float(steps-1);
		for (int i = 0; i < steps; i++) {
			result.push_back(start + i * step);
		}

		return result;
	}


    std::vector<glm::vec3> interpolate(glm::vec3 start, glm::vec3 end, int steps){
		std::vector<glm::vec3> result;
		glm::vec3 step = (end - start) / float(steps-1);
		for (int i = 0; i < steps; i++) {
			result.push_back(start + float(i) * step);
		}
		return result;
	}

uint32_t fromColour(Colour colour){
	uint32_t colour_in_uint32 = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
	return colour_in_uint32;
}


}