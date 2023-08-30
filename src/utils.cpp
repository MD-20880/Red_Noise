#include "utils.h"

#include <fstream>

#include <Utils.h>
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

	Colour getColour(uint32_t colour){
		int r = (colour >> 16) & 0xff; // red
		int g = (colour >> 8) & 0xff; // green
		int b = colour  & 0xff; // blue
		return Colour(r,g,b);
	}

	glm::vec3 GetBarycentricCoord(CanvasPoint P1, CanvasPoint P2, CanvasPoint P3, CanvasPoint P)
	{
		float u = ((P2.y - P3.y) * P.x + (P3.x - P2.x) * P.y + (P2.x * P3.y - P3.x * P2.y)) / ((P2.y - P3.y) * P1.x + (P3.x - P2.x) * P1.y + (P2.x * P3.y - P3.x * P2.y));
		float v = ((P1.y - P3.y) * P.x + (P3.x - P1.x) * P.y + (P1.x * P3.y - P3.x * P1.y)) / ((P1.y - P3.y) * P2.x + (P3.x - P1.x) * P2.y + (P1.x * P3.y - P3.x * P1.y));
		//float w = ((P1.y - P2.y) * P.x + (P2.x - P1.x) * P.y + (P1.x * P2.y - P2.x * P1.y)) / ((P1.y - P2.y) * P3.x + (P2.x - P1.x) * P3.y + (P1.x * P2.y - P2.x * P1.y));
		float w = 1 - u - v;
		// std::cout <<"SB" <<std::endl;
		return glm::vec3(u, v, w);
		// return glm::vec3(1, 1, 1);
	}

	/* readObj Helpers BEGIN */
	glm::vec3 readVertex(std::vector<std::string> line, std::vector<glm::vec3> vertices){
		return glm::vec3(std::stof(line[1]),std::stof(line[2]),std::stof(line[3]));
	}


	ModelTriangle formTriangle(std::vector<std::string> line, std::vector<glm::vec3> vertices,Colour mtl){

		for (size_t i = 1; i < line.size(); i++){
			line[i].pop_back();
		}

		int i1, i2, i3;
		i1 = std::stoi(line[1])-0.1;
		i2 = std::stoi(line[2])-0.1;
		i3 = std::stoi(line[3])-0.1;


		return ModelTriangle(vertices[i1],vertices[i2],vertices[i3],mtl);
	}
	/* readObj Helpers END */


	std::vector<ModelTriangle> readObj(std::string filepath,std::map<std::string,Colour> mtlmap,float scaling){
		std::string receiver;
		std::ifstream readFile(filepath);

		std::vector<glm::vec3> vertices;
		std::vector<ModelTriangle> triangles;

		Colour currentmtl;

		while(getline(readFile,receiver)){
			std::vector<std::string> elements = split(receiver,' ');
			if (elements[0] == "v"){
				vertices.push_back(readVertex(elements,vertices)*scaling);
			}else if(elements[0] == "f"){
				triangles.push_back(formTriangle(elements,vertices,currentmtl));
			}else if (elements[0] == "usemtl"){
				currentmtl = mtlmap[elements[1]];
			}
		}
		readFile.close();
		printf("Read %d Triangles\n",triangles.size());
		return triangles;
	}


	/* ReadMtl Helpers BEGIN */
	void addmtl(std::vector<std::string> line, std::vector<std::string> elements, std::map<std::string,Colour> &map){
		map[line[1]] = Colour(line[1],std::stof(elements[1])*255,std::stof(elements[2])*255,std::stof(elements[3])*255);
	}
	/* ReadMtl Helpers END */

	/**
	 * @brief Read .mtl file
	 * 
	 * @param filepath 
	 * @return std::map<std::string,Colour> 
	 */
	std::map<std::string,Colour> readMtl(std::string filepath){
		std::string receiver;
		std::ifstream readFile(filepath);
		std::map<std::string,Colour> mtlmap;

		while(getline(readFile,receiver)){
			std::vector<std::string> line = split(receiver,' ');
			if(line[0] == "newmtl"){
				// std::cout << receiver << std::endl;
				getline(readFile,receiver);
				// std::cout << receiver << std::endl;
				std::vector<std::string> nextline = split(receiver,' ');
				addmtl(line,nextline,mtlmap);
			}
		}
		printf("Read %d MTLs\n", mtlmap.size());

		return mtlmap;
	}




}