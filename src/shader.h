#include "scene.h"
#include "camera.h"


namespace Shader{
    CanvasPoint calculateCanvasPos(int height,int width,glm::vec3 point,Camera::BasicCamera* camera);

    void LineShader(Scene::BasicScene* scene, Camera::BasicCamera* camera);
    void RasterizeShader(Scene::BasicScene* scene, Camera::BasicCamera* camera);
    void RaytracingShader(Scene::BasicScene* scene, Camera::BasicCamera* camera);
    void GourandShader(Scene::BasicScene* scene, Camera::BasicCamera* camera);
    void PhongShader(Scene::BasicScene* scene, Camera::BasicCamera* camera);
}