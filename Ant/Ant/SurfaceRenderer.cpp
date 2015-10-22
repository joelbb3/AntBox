#include "SurfaceRenderer.h"
#include <fstream>
#include <cmath>
#include <iostream>

sf::Texture SurfaceRenderer::frameTexture;
sf::Sprite SurfaceRenderer::frameSprite;
std::vector<std::shared_ptr<DepthFrame>> SurfaceRenderer::frameList;
std::shared_ptr<DepthFrame> SurfaceRenderer::currentFrame;

void SurfaceRenderer::loadFrame(std::string pathToFrameData){
    float input;
    std::ifstream infile;
    infile.open(pathToFrameData);
    if (!infile)
    {
        std::cerr << "Error: could not open file.\n";
    }
    int y, x;
    infile >> y >> x;
    std::shared_ptr<float> data{new float[x*y]};
    int i = 0;
    while (infile >> input)
    {
        data.get()[x * y - x * (i / x + 1) + i % x] = input;
        i++;
    }
    infile.close();
    frameList.push_back(std::shared_ptr<DepthFrame>{new DepthFrame(data, x, y)});
}

void SurfaceRenderer::setFrame(int frameIndex){
    currentFrame = frameList[frameIndex];
    SurfaceRenderer::renderFrame();
}

void SurfaceRenderer::renderFrame(){
    float min, max, dif;
    min = 745;
    max = 808;
    dif = max - min;
    
    typedef unsigned char Uint8;
    Uint8* RGBA;
    RGBA = new Uint8[currentFrame->width * currentFrame->height * 4];
    
    for (int i = 0; i < currentFrame->width * currentFrame->height; ++i) {
        float hue;
        if (currentFrame->depthData.get()[i] < min) {
            hue = 0;
        }
        else if (currentFrame->depthData.get()[i] > max) {
            hue = 240;
        }
        else {
            hue = ((currentFrame->depthData.get()[i] - min) / dif) * 240;
        }
        
        //Convert from hue to RGB: http://www.rapidtables.com/convert/color/hsl-to-rgb.htm
        float C, X, m;
        C = (1 - fabs(2 * 0.5 - 1)) * 1;
        X = C*(1 - fabs(fmod(hue / 60.0, 2) - 1));
        m = 0.5 - C / 2.0;
        
        float mods[3];
        if (hue >= 0 && hue < 60) {
            mods[0] = C;
            mods[1] = X;
            mods[2] = 0;
        }
        else if (hue >= 60 && hue < 120) {
            mods[0] = X;
            mods[1] = C;
            mods[2] = 0;
        }
        else if (hue >= 120 && hue < 180) {
            mods[0] = 0;
            mods[1] = C;
            mods[2] = X;
        }
        else if (hue >= 180 && hue < 240) {
            mods[0] = 0;
            mods[1] = X;
            mods[2] = C;
        }
        else if (hue >= 240 && hue < 300) {
            mods[0] = X;
            mods[1] = 0;
            mods[2] = C;
        }
        else {
            std::cerr << "what the hell."; //Something went horribly wrong.
        }
        RGBA[i * 4] = (mods[0] + m)*255;
        RGBA[i * 4 + 1] = (mods[1] + m)*255;
        RGBA[i * 4 + 2] = (mods[2] + m)*255;
        RGBA[i * 4 + 3] = 255;
    }
    frameTexture = sf::Texture();
    frameTexture.create(currentFrame->width, currentFrame->height);
    frameTexture.update(RGBA);
    frameTexture.setSmooth(true);
    frameSprite.setTexture(frameTexture);
    //frameSprite.setScale(3, 3);
}

void SurfaceRenderer::drawFrame(sf::RenderWindow& window){
    window.draw(frameSprite);
}

double SurfaceRenderer::getDepth(int x, int y){
    return currentFrame->depthData.get()[y * currentFrame->width + x];
}

int SurfaceRenderer::getCurrentWidth(){
    return currentFrame->width;
}

int SurfaceRenderer::getCurrentHeight(){
    return currentFrame->height;
}