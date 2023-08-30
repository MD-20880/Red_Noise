#include "renderer.h"


namespace Render{

glm::mat3x3 transfer(CanvasTriangle triangle1, CanvasTriangle triangle2){
	// glm::mat3x3 mat1(triangle1.v0().x,triangle1.v0().y,1,triangle1.v1().x,triangle1.v1().y,1,triangle1.v2().x,triangle1.v2().y,1);
	// glm::mat3x3 mat2(triangle2.v0().x,triangle2.v0().y,1,triangle2.v1().x,triangle2.v1().y,1,triangle2.v2().x,triangle2.v2().y,1);
	glm::mat3x3 mat1(triangle1.v0().x,triangle1.v1().x,triangle1.v2().x,triangle1.v0().y,triangle1.v1().y,triangle1.v2().y,1,1,1);
	glm::mat3x3 mat2(triangle2.v0().x,triangle2.v1().x,triangle2.v2().x,triangle2.v0().y,triangle2.v1().y,triangle2.v2().y,1,1,1);
	glm::mat3x3 result = glm::inverse(mat1)*mat2;
	return result;
}

Renderer::Renderer(DrawingWindow *window, int height, int width){
    window_ = window;
    height_ = height;
    width_ = width;
    pbuffer_ = makepBuffer();
	dbuffer_ = makedBuffer();
}

Renderer::~Renderer(){
}

 

void Renderer::drawLine(CanvasPoint from, CanvasPoint to, Colour colour){
    float xdiff = to.x - from.x;
    float ydiff = to.y - from.y;
    float step = std::max(abs(xdiff),abs(ydiff));
    float xStepSize = xdiff/step;
    float yStepSize = ydiff/step;
	std::vector<float> depths = Utils::interpolate(from.depth,to.depth,step);

    for (int i = 0; i < step; i++){
        float x = from.x+( xStepSize * i );
        float y = from.y + ( yStepSize * i );
		if ((round(x) < width_) && (round(y) < height_) && (x >= 0) && (y >= 0)){
			pbuffer_[round(y)][round(x)] = Utils::fromColour(colour);
		}else{
			continue;;
		}

    }
}


void Renderer::drawLine(CanvasPoint from, CanvasPoint to, TextureMap* texture, glm::mat3x3 transfer){
	float xdiff = to.x - from.x;
	float ydiff = to.y - from.y;
	float step = std::max(abs(xdiff),abs(ydiff));
	float xStepSize = xdiff/step;
	float yStepSize = ydiff/step;
	std::vector<float> depths = Utils::interpolate(from.depth,to.depth,step);

	for (int i = 0; i < step; i++){
		float x = from.x+( xStepSize * i );
		float y = from.y + ( yStepSize * i );
		glm::vec3 p = glm::vec3(x,y,1)*transfer ;
		TexturePoint point(p.x,p.y);
		uint32_t c = (*texture).pixels[int(p.y)*(*texture).width+int(p.x)];
		if ((round(x) < width_) && (round(y) < height_) && (x >= 0) && (y >= 0)){
			pbuffer_[round(y)][round(x)] = c;
		}else{
			continue;
		}
	}
}

void Renderer::drawTriangle(CanvasTriangle triangle, Colour colour){
	drawLine(triangle.v0(),triangle.v1(),colour);
	drawLine(triangle.v1(),triangle.v2(),colour);
	drawLine(triangle.v2(),triangle.v0(),colour);
}



void Renderer::drawFlatFilledTriangle(CanvasPoint bottom1, CanvasPoint bottom2, CanvasPoint top, Colour colour){
	float direction = top.y > bottom1.y ? 1 : -1;
	float slope1 = (top.x - bottom1.x)/(top.y-bottom1.y);
	float slope2 = (top.x - bottom2.x)/(top.y-bottom2.y);
	for (int i = 0; i < abs(top.y - bottom1.y); i++){
		CanvasPoint b1 = CanvasPoint(
		bottom1.x+ slope1*(i*direction),
		//x      + (      dx          /      dy        )*(    dy     )
		bottom1.y+(i*direction)
		);
		CanvasPoint b2 = CanvasPoint(
		bottom2.x+ slope2*(i*direction),
		bottom2.y+(i*direction)
		);
		drawLine(b1,b2,colour);
	}
}

void Renderer::drawFlatFilledTriangle(CanvasPoint bottom1, CanvasPoint bottom2, CanvasPoint top, TextureMap* texture, glm::mat3x3 transfer){
	//detect drawing direction
	float directiony = top.y >= bottom1.y ? 1 : -1;

	float slope1 = (top.x - bottom1.x)/(top.y-bottom1.y);
	float slope2 = (top.x - bottom2.x)/(top.y-bottom2.y);
	int height = abs(top.y - bottom1.y);
	for (int i = 0; i < height; i++){
		float factor = i*directiony;
		CanvasPoint b1 = CanvasPoint(
		bottom1.x+ slope1*factor,
		//x      + (      dx          /      dy        )*(    dy     )
		bottom1.y+factor
		);
		CanvasPoint b2 = CanvasPoint(
			bottom2.x+slope2*factor,
			bottom2.y+factor
		);
		drawLine(b1,b2,texture,transfer);
	}
}



void Renderer::drawFilledTriangle(CanvasTriangle triangle, TextureMap* texture){
	CanvasTriangle textureTriangle = CanvasTriangle(CanvasPoint(triangle.v0().texturePoint.x,triangle.v0().texturePoint.y),
											CanvasPoint(triangle.v1().texturePoint.x,triangle.v1().texturePoint.y),
											CanvasPoint(triangle.v2().texturePoint.x,triangle.v2().texturePoint.y)
											);
	glm::mat3x3 transfer = Render::transfer(triangle,textureTriangle);

	

	CanvasPoint top,middle,bottom,split;
	std::array<CanvasPoint,3> points= triangle.vertices;
	std::sort(points.begin(),points.end(),[](CanvasPoint a, CanvasPoint b){return a.y<b.y;});

	top= points[0];
	middle= points[1];
	bottom=points[2];
	drawTriangle(triangle,Colour(255,255,255));
	//Assertions
	assert(middle.y >= top.y);
    assert(bottom.y >= middle.y);

	//Calculate Split Point
	float splitx = top.x + (bottom.x - top.x)/(bottom.y-top.y)*(middle.y-top.y);
	float splity = middle.y;
	glm::vec3 tPoint (glm::vec3(splitx,splity,1)*transfer);
	TexturePoint texturePoint = TexturePoint(tPoint.x,tPoint.y);
	split = CanvasPoint(
		splitx,
		splity,
		top.depth + 
		(bottom.x - top.x)/(bottom.depth-top.depth)*(middle.depth-top.depth) + 
		(bottom.y - top.y)/(bottom.depth-top.depth)*(middle.depth-top.depth)
	);
	split.texturePoint = texturePoint;
	drawFlatFilledTriangle(middle,split,top,texture,transfer);
	drawFlatFilledTriangle(middle,split,bottom,texture,transfer);
	
}



void Renderer::drawFilledTriangle(CanvasTriangle triangle, Colour colour){
	auto v0 = triangle.v0();
	auto v1 = triangle.v1();
	auto v2 = triangle.v2();
	int startx, starty, endx, endy;
	startx = std::min({v0.x,v1.x,v2.x});
	starty = std::min({v0.y,v1.y,v2.y});
	endx = std::max({v0.x,v1.x,v2.x});
	endy = std::max({v0.y,v1.y,v2.y});

	// std::cout << "BEFORE LOOP" << std::endl;

	for (int y = round(starty); y<round(endy); y++){

		for (int x = round(startx); x<round(endx); x++){
			if (!(x >= width_ || x < 0 || y >= height_ || y < 0)){
				glm::vec3 barycord = Utils::GetBarycentricCoord(v0,v1,v2,CanvasPoint(x,y));
				if (barycord.x >= 0 && barycord.x <= 1 && barycord.y >= 0 && barycord.y <= 1 && barycord.z >= 0 && barycord.z <= 1){
					// float depth = 1/(barycord*glm::mat3x3(v0.x,v0.y,v0.depth,v1.x,v1.y,v1.depth,v2.x,v2.y,v2.depth)).z;
					float depth = 1.0/(barycord*glm::mat3x3(v0.x,v1.x,v2.x,v0.y,v1.y,v2.y,v0.depth,v1.depth,v2.depth)).z;
					// float depth = 0.5;

				if (depth >= dbuffer_[y][x]){
					// if (depth < dbuffer[0][0]){
					dbuffer_[y][x] = depth;
					pbuffer_[y][x] = Utils::fromColour(colour);
					//window.setPixelColour(x,y,fromColour(colour));
				}
			}
			}

		}
	}

	
}

std::vector<std::vector<uint32_t>>* Renderer::getpbuffer(){
	return &pbuffer_;
}
std::vector<std::vector<float>>* Renderer::getdbuffer(){
	return &dbuffer_;
}
    








void Renderer::refresh(){
    for (size_t y = 0; y < height_; y++){
        for (size_t x = 0; x < width_ ; x++){
            (*window_).setPixelColour(x,y,pbuffer_[y][x]);
        }
    }
    (*window_).renderFrame();
}

void Renderer::clearPixelBuffer(){
    pbuffer_ = makepBuffer();
	dbuffer_ = makedBuffer();
}



std::vector<std::vector<float>> Renderer::makedBuffer(){
	std::vector<std::vector<float>> result;
	for (size_t i = 0; i < height_; i++)
	{
		std::vector<float> row;
		for (size_t j = 0; j < width_; j++)
		{
			row.push_back(FLT_MIN);
		}
		result.push_back(row);
	}
	return result;
}



std::vector<std::vector<uint32_t>> Renderer::makepBuffer(){
	std::vector<std::vector<uint32_t>> result;
	for (size_t i = 0; i < height_; i++)
	{
		std::vector<uint32_t> row;
		for (size_t j = 0; j < width_; j++)
		{
			row.push_back(Utils::fromColour(Colour(0,0,0)));
		}
		result.push_back(row);
	}
	return result;
}

}