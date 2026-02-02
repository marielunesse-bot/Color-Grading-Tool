#pragma once

#include <GL/gl.h>
#include <opencv2/opencv.hpp>
#include <string>

class Texture
{
public:
    Texture();
    ~Texture();

    bool LoadFromMat(const cv::Mat& image);
    void Bind() const;
    void Unbind() const;

    int GetWidth() const { return mWidth; }
    int GetHeight() const { return mHeight; }

private:
    GLuint mTextureID;
    int mWidth;
    int mHeight;
    int mChannels;
};