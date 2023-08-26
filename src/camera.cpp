#include "camera.h"


namespace Camera{

	    BasicCamera::BasicCamera(Render::Renderer* renderer){
            campos_ = glm::vec3(0, 0, 4);
            camrot_ = glm::mat3(1,0,0,0,-1,0,0,0,1);
            focal_ = 2.0f;
            renderer_ = renderer;
        }


        BasicCamera::~BasicCamera(){

        }


        void BasicCamera::render(Scene::BasicScene scene, void (*render)(Scene::BasicScene*,Camera::BasicCamera*) ){
            render(&scene,this);
        }

        void BasicCamera::rotate (glm::mat3x3 rotation){
            camrot_ = camrot_*rotation;
        }
        
        void BasicCamera::setOrientation(glm::mat3x3 camrot){
            camrot_ = camrot;
        }


        void BasicCamera::setPosition(glm::vec3 position){
            campos_ = position;
        }

}


