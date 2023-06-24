#include "renderer.h"


namespace Render{


Renderer::Renderer(DrawingWindow *window, int height, int width){
    window_ = window;
    height_ = height;
    width_ = width;
    pbuffer_ = makepBuffer();
}

Renderer::~Renderer(){
}

void Renderer::drawLine(CanvasPoint from, CanvasPoint to, Colour colour){
    float xdiff = to.x - from.x;
    float ydiff = to.y - from.y;
    float step = std::max(abs(xdiff),abs(ydiff));
    float xStepSize = xdiff/step;
    float yStepSize = ydiff/step;

    for (int i = 0; i < step; i++){
        float x = from.x+( xStepSize * i );
        float y = from.y + ( yStepSize * i );
        pbuffer_[y][x] = Utils::fromColour(colour);
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

void Renderer::drawFilledTriangle(CanvasTriangle triangle, Colour colour){
	//Sort Points From top to Bottom
	CanvasPoint top,middle,bottom,split;
	std::array<CanvasPoint,3> points= triangle.vertices;
	std::sort(points.begin(),points.end(),[](CanvasPoint a, CanvasPoint b){return a.y<b.y;});

	top= points[0];
	middle= points[1];
	bottom=points[2];
	//Assertions
	assert(middle.y >= top.y);
    assert(bottom.y >= middle.y);

	//Calculate Split Point
	split = CanvasPoint(
		top.x + (bottom.x - top.x)/(bottom.y-top.y)*(middle.y-top.y),
		middle.y
	);
	drawFlatFilledTriangle(middle,split,top,colour);
	drawFlatFilledTriangle(middle,split,bottom,colour);
	
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