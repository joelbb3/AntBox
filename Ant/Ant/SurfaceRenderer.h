#ifndef __SandboxSim__SurfaceRenderer__
#define __SandboxSim__SurfaceRenderer__

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>

struct DepthFrame{
    std::shared_ptr<float> depthData;
    int width;
    int height;
    DepthFrame(std::shared_ptr<float> depthData, int width, int height) : depthData(depthData), width(width), height(height){
    };
};

class SurfaceRenderer{
    static sf::Texture frameTexture;
    static sf::Sprite frameSprite;
    static std::vector<std::shared_ptr<DepthFrame>> frameList;
    static std::shared_ptr<DepthFrame> currentFrame;
public:
    static void drawFrame(sf::RenderWindow& window);
    static void loadFrame(std::string pathToFrameData);
    static void setFrame(int frameIndex);
    static void renderFrame();
    static double getDepth(int x, int y);
    static int getCurrentWidth();
    static int getCurrentHeight();
};

#endif /* defined(__SandboxSim__SurfaceRenderer__) */
