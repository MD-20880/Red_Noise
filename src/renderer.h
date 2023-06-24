#include <CanvasPoint.h>
#include <CanvasTriangle.h>
#include <Colour.h>
#include <DrawingWindow.h>

#include "utils.h"


namespace Render
{


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

    void drawTriangle(CanvasTriangle triangle, Colour colour);

    void drawFilledTriangle(CanvasTriangle triangle, Colour colour);

    void drawFilledTriangle(CanvasTriangle triangle);

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

private:
    DrawingWindow *window_;
    std::vector<std::vector<uint32_t>> pbuffer_;
    size_t height_;
    size_t width_;

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
};

}



