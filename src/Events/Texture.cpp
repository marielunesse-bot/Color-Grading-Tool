#include "Texture.h"
#include "../../Core/Logger.h"

Texture::Texture()
    : mTextureID(0), mWidth(0), mHeight(0), mChannels(0)
{
    glGenTextures(1, &mTextureID);
}

Texture::~Texture()
{
    if (mTextureID)
        glDeleteTextures(1, &mTextureID);
}

bool Texture::LoadFromMat(const cv::Mat& image)
{
    if (image.empty())
    {
        Logger::Error("Texture: empty image");
        return false;
    }

    mWidth = image.cols;
    mHeight = image.rows;
    mChannels = image.channels();

    GLenum format = GL_RGB;
    if (mChannels == 1) format = GL_RED;
    else if (mChannels == 3) format = GL_BGR;
    else if (mChannels == 4) format = GL_BGRA;

    glBindTexture(GL_TEXTURE_2D, mTextureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, image.data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    Logger::Info("Texture loaded successfully.");
    return true;
}

void Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, mTextureID);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}