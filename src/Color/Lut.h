#pragma once

#include <vector>
#include <string>
#include <GL/gl.h>
#include "../../Core/Logger.h"

class Lut
{
public:
    Lut();
    ~Lut();

    bool LoadFromCubeFile(const std::string& filePath);

    void Bind() const;
    void Unbind() const;

    int GetSize() const { return mSize; }

private:
    GLuint mTextureID;
    int mSize; // taille du LUT (ex: 32 pour 32x32x32)
    std::vector<float> mData; // RGB float

    void UploadToGPU();
};