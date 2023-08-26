#pragma once

#include <glm/glm.hpp>

#include "renderer.h"
#include "scene.h"



#define ROTATE_X(angle) glm::mat3x3(1,0,0,0,cos(angle),-sin(angle),0,sin(angle),cos(angle))
#define ROTATE_Y(angle) glm::mat3x3(cos(angle),0,sin(angle),0,1,0,-sin(angle),0,cos(angle))
namespace Camera{

    class BasicCamera {
    public:
	    glm::vec3 campos_;
	    glm::mat3 camrot_;
	    float focal_;
        Render::Renderer* renderer_;
    

        BasicCamera(Render::Renderer* renderer);
        ~BasicCamera();
        void render(Scene::BasicScene scene, void (*render)(Scene::BasicScene*,Camera::BasicCamera*));
        void rotate (glm::mat3x3 rotation);
        void setOrientation(glm::mat3x3 camrot);
        void setPosition(glm::vec3 position);
    };


}