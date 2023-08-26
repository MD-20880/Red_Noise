#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Colour.h>
#include <Utils.h>
#include <fstream>
#include <vector>


#include "utils.h"
#include "renderer.h"
#include "camera.h"
#include "shader.h"

#define WIDTH 320
#define HEIGHT 240



size_t mode = 0x05;
bool doRotation = false;
/* 0x01  Blank    */
/* 0x02  WireFrame*/

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

glm::mat3x3 lookAt(Camera::BasicCamera camera,glm::vec3 point){
	glm::vec3 fakeup(0,1,0);
	glm::vec3 forward = glm::normalize(-point + camera.campos_);
	glm::vec3 right = glm::normalize(glm::cross(fakeup,forward));
	glm::vec3 up = glm::normalize(glm::cross(right,forward));
	return glm::mat3(right,up,forward);
}


void handleEvent(SDL_Event event, DrawingWindow &window, Render::Renderer &renderer, Camera::BasicCamera* cameras) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_l) {
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
			CanvasTriangle triangle = CanvasTriangle(
                    CanvasPoint(rand()%WIDTH,rand()%HEIGHT,rand()%HEIGHT),
                    CanvasPoint(rand()%WIDTH,rand()%HEIGHT,rand()%HEIGHT),
                    CanvasPoint(rand()%WIDTH,rand()%HEIGHT,rand()%HEIGHT));
			printf("top:\t(%f,%f)\nleft:\t(%f,%f)\nright:\t(%f,%f)\n",triangle.v0().x,triangle.v0().y,
			                                                triangle.v1().x,triangle.v1().y,
			                                                triangle.v2().x,triangle.v2().y);
			renderer.drawFilledTriangle(triangle,Colour(rand()%256,rand()%256,rand()%256));
		}
		else if (event.key.keysym.sym == SDLK_h){
			TextureMap tmap = TextureMap("../src/texture.ppm");
			CanvasPoint point1(160, 10);
            CanvasPoint point2(300, 230);
            CanvasPoint point3(10, 150);
			point1.texturePoint.x = 195; point1.texturePoint.y = 5;
            point2.texturePoint.x = 395; point2.texturePoint.y = 380;
            point3.texturePoint.x = 65; point3.texturePoint.y = 330;
			

			renderer.drawFilledTriangle(CanvasTriangle(
				point1,
				point2,
				point3
			),&tmap);
		}
		else if (event.key.keysym.sym == SDLK_j){
			if (mode != 0x02){
				mode = 0x02;
			}else{
				mode = mode;
			}
		}
		else if (event.key.keysym.sym == SDLK_k){
			if (mode != 0x03){
				mode = 0x03;
			}else{
				mode = mode;
			}
		}else if (event.key.keysym.sym == SDLK_m){
			if (mode != 0x04){
				mode = 0x04;
			}else{
				mode = mode;
			}
		}else if (event.key.keysym.sym == SDLK_n){
			if (mode != 0x05){
				mode = 0x05;
			}else{
				mode = mode;
			}
		}
		else if (event.key.keysym.sym == SDLK_r){
			doRotation = !doRotation;
		}


		



		/*Orbit Code*/
		else if(event.key.keysym.sym == SDLK_UP)cameras->rotate(ROTATE_X(0.1));
		else if(event.key.keysym.sym == SDLK_DOWN)cameras->rotate(ROTATE_X(-0.1));
		else if(event.key.keysym.sym == SDLK_RIGHT)cameras->rotate(ROTATE_Y(0.1));
		else if(event.key.keysym.sym == SDLK_LEFT)cameras->rotate(ROTATE_Y(-0.1));

		/*Move Code*/
		else if(event.key.keysym.sym == SDLK_w)cameras->setPosition(cameras->campos_+(glm::vec3(0,0,-0.1)*cameras->camrot_));
		else if(event.key.keysym.sym == SDLK_s)cameras->setPosition(cameras->campos_+(glm::vec3(0,0,0.1)*cameras->camrot_));
		else if(event.key.keysym.sym == SDLK_d)cameras->setPosition(cameras->campos_+(glm::vec3(0.1,0,0)*cameras->camrot_));
		else if(event.key.keysym.sym == SDLK_a)cameras->setPosition(cameras->campos_+(glm::vec3(-0.1,0,0)*cameras->camrot_));
		
	} else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	} 
}

int main(int argc, char *argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;
	Render::Renderer renderer(&window,HEIGHT,WIDTH);
	Camera::BasicCamera camera1(&renderer);
	Scene::BasicScene scene1("../src/cornell-box.obj","../src/cornell-box.mtl",0.35);
	Scene::BasicScene scene2("../src/ball.obj","../src/cornell-box.mtl",0.35);
	camera1.setPosition(glm::vec3(0.000000, 0.800000, 2.200002));
	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window,renderer,&camera1);
		if(mode == 0x02){
			renderer.clearPixelBuffer();
			// camera1.setPosition(glm::vec3(0.000000, 0.0,4));
			void (*render)(Scene::BasicScene*,Camera::BasicCamera*) = Shader::RaytracingShader;
			camera1.render(scene1, render);
		}
		else if(mode == 0x03){
			renderer.clearPixelBuffer();
			// camera1.setPosition(glm::vec3(0.000000, 0.0,4));
			void (*render)(Scene::BasicScene*,Camera::BasicCamera*) = Shader::RasterizeShader;
			camera1.render(scene1, render);
		}else if(mode == 0x04){
			renderer.clearPixelBuffer();
			// camera1.setPosition(glm::vec3(0.000000, 0.800000, 2.200002));
			scene2.pointLights_ = {glm::vec3(0,0.8,1)};
			void (*render)(Scene::BasicScene*,Camera::BasicCamera*) = Shader::GourandShader;
			camera1.render(scene2, render);
		}else if(mode == 0x05){
			renderer.clearPixelBuffer();
			// camera1.setPosition(glm::vec3(0.000000, 0.800000, 2.200002));
			scene2.pointLights_ = {glm::vec3(0,0.8,1)};
			void (*render)(Scene::BasicScene*,Camera::BasicCamera*) = Shader::PhongShader;
			camera1.render(scene2, render);
		}
		
		if (doRotation){
			camera1.setPosition(camera1.campos_*ROTATE_Y(0.001));
			camera1.setOrientation(lookAt(camera1,glm::vec3(0,0,0)));
		}
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		// renderer.clearPixelBuffer();
		renderer.refresh();
	}
}
