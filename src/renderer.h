#pragma once

#include <CanvasPoint.h>
#include <CanvasTriangle.h>
#include <Colour.h>
#include <DrawingWindow.h>
#include <TextureMap.h>
#include <glm/glm.hpp>

#include "utils.h"


namespace Render
{

glm::mat3x3 transfer(CanvasTriangle triangle1, CanvasTriangle triangle2);


class Renderer{
public:

    /**
     * @brief Construct a new Renderer object
     * 
     * @param window 
     */
    Renderer(DrawingWindow *window,int height, int width);

    /**
     * @brief Destroy the Renderer object
     * 
     */
    ~Renderer();


    /**
     * @brief Draw line from one CanvasPoint to another CanvasPoint
     * 
     * @param from Starting point
     * @param to Ending point
     * @param colour Colour of the line 
     * @param window DrawingWindow
     */
    void drawLine(CanvasPoint from, CanvasPoint to, Colour colour);
    void drawLine(CanvasPoint from, CanvasPoint to, TextureMap* texture, glm::mat3x3 transfer);
    void drawTriangle(CanvasTriangle triangle, Colour colour);
    void drawFilledTriangle(CanvasTriangle triangle, Colour colour);
    void drawFilledTriangle(CanvasTriangle triangle, TextureMap* texture);

    std::vector<std::vector<uint32_t>>* getpbuffer();
    std::vector<std::vector<float>>* getdbuffer() ;
    
    void setpbuffer(int y, int x, uint32_t value);
    void setdbuffer(int y, int x, float value);

    /**
     * @brief Display Contents in pbuffer_
     * 
     */
    void refresh();

    /**
     * @brief Clean pbuffer_
     * 
     */
    void clearPixelBuffer();

    int getHeight(){
        return height_;
    }
    int getWidth(){
        return width_;
    }

private:
    DrawingWindow *window_;
    std::vector<std::vector<uint32_t>> pbuffer_;
    std::vector<std::vector<float>> dbuffer_ ;
    int height_;
    int width_;

    

    /**
     * @brief Create empty depth Buffer
     * 
     * @return std::vector<std::vector<float>> 
     */
    std::vector<std::vector<float>> makedBuffer();

    /**
     * @brief Create empty pixel buffer
     * 
     * @return std::vector<std::vector<uint32_t>> 
     */
    std::vector<std::vector<uint32_t>> makepBuffer();

    void drawFlatFilledTriangle(CanvasPoint bottom1, CanvasPoint bottom2, CanvasPoint top, Colour colour);
    void drawFlatFilledTriangle(CanvasPoint bottom1, CanvasPoint bottom2, CanvasPoint top, TextureMap* texture, glm::mat3x3 transfer);
};

}



