#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Colour.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include "utils.h"
#include "renderer.h"

#define WIDTH 320
#define HEIGHT 240

void draw(DrawingWindow &window) {
	window.clearPixels();
	for (size_t y = 0; y < window.height; y++) {
		for (size_t x = 0; x < window.width; x++) {
			float red = rand() % 256;
			float green = 0.0;
			float blue = 0.0;
			uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
			window.setPixelColour(x, y, colour);
		}
	}
}

void drawGray(DrawingWindow &window) {
	window.clearPixels();
    std::vector<float> values = Utils::interpolate(255.0f , 0.0f , window.width);
	for (size_t y = 0; y < window.height; y++) {
		for (size_t x = 0; x < window.width; x++) {
			float red = values[x];
			float green = values[x];
			float blue = values[x];
			uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
			window.setPixelColour(x, y, colour);
		}
	}
}

void drawColor(DrawingWindow &window){
    window.clearPixels();

    glm::vec3 topLeft(255, 0, 0);        // red
    glm::vec3 topRight(0, 0, 255);       // blue
    glm::vec3 bottomRight(0, 255, 0);    // green
    glm::vec3 bottomLeft(255, 255, 0);   // yellow

    std::vector<glm::vec3> firstRow = Utils::interpolate(topLeft,bottomLeft,window.height);
    std::vector<glm::vec3> lastRow = Utils::interpolate(topRight,bottomRight,window.height);


    for (size_t y = 0; y < window.height; y++) {
        std::vector<glm::vec3> values = Utils::interpolate( firstRow[y] , lastRow[y] , window.width );
        for (size_t x = 0; x < window.width; x++) {
            float red = values[x][0];
            float green = values[x][1];
            float blue = values[x][2];
            uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
            window.setPixelColour(x, y, colour);
        }
    }
}


void handleEvent(SDL_Event event, DrawingWindow &window, Render::Renderer &renderer) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
		else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
		else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
		else if (event.key.keysym.sym == SDLK_d) {
			CanvasPoint point1(rand()%WIDTH,rand()%HEIGHT);
			CanvasPoint point2(rand()%WIDTH,rand()%HEIGHT);
			Colour colour(rand()%256,rand()%256,rand()%256);
			renderer.drawLine(point1,point2,colour);
		}
		else if (event.key.keysym.sym == SDLK_f){
			renderer.drawTriangle(CanvasTriangle(
                    CanvasPoint(rand()%WIDTH,rand()%HEIGHT),
                    CanvasPoint(rand()%WIDTH,rand()%HEIGHT),
                    CanvasPoint(rand()%WIDTH,rand()%HEIGHT)),
					Colour(rand()%256,rand()%256,rand()%256));
		}
		else if (event.key.keysym.sym == SDLK_g){
			renderer.drawFilledTriangle(CanvasTriangle(
                    CanvasPoint(rand()%WIDTH,rand()%HEIGHT),
                    CanvasPoint(rand()%WIDTH,rand()%HEIGHT),
                    CanvasPoint(rand()%WIDTH,rand()%HEIGHT)),
					Colour(rand()%256,rand()%256,rand()%256));
		}
	} else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	} 
}

int main(int argc, char *argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;
	Render::Renderer renderer(&window,HEIGHT,WIDTH);
	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window,renderer);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		renderer.refresh();
	}
}
