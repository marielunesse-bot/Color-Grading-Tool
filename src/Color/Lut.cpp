#include "Lut.h"
#include <fstream>
#include <sstream>
#include <iostream>

Lut::Lut()
    : mTextureID(0), mSize(0)
{
    glGenTextures(1, &mTextureID);
}

Lut::~Lut()
{
    if (mTextureID)
        glDeleteTextures(1, &mTextureID);
}

bool Lut::LoadFromCubeFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        Logger::Error("Failed to open LUT file: " + filePath);
        return false;
    }

    std::string line;
    mData.clear();
    mSize = 0;

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#' || line.find("TITLE") != std::string::npos)
            continue;

        if (line.find("LUT_3D_SIZE") != std::string::npos)
        {
            std::istringstream ss(line);
            std::string tmp;
            ss >> tmp >> tmp >> mSize;
            continue;
        }

        float r, g, b;
        std::istringstream ss(line);
        if (!(ss >> r >> g >> b)) continue;

        mData.push_back(r);
        mData.push_back(g);
        mData.push_back(b);
    }

    file.close();

    if (mData.empty() || mSize == 0)
    {
        Logger::Error("Invalid LUT file: " + filePath);
        return false;
    }

    UploadToGPU();
    Logger::Info("LUT loaded successfully: " + filePath);
    return true;
}

void Lut::UploadToGPU()
{
    glBindTexture(GL_TEXTURE_3D, mTextureID);

    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB32F,
                 mSize, mSize, mSize, 0,
                 GL_RGB, GL_FLOAT, mData.data());

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_3D, 0);
}

void Lut::Bind() const
{
    glBindTexture(GL_TEXTURE_3D, mTextureID);
}

void Lut::Unbind() const
{
    glBindTexture(GL_TEXTURE_3D, 0);
}