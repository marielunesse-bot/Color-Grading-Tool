#include "ImageLoader.h"
#include <iostream>

ImageLoader::ImageLoader()
{
}

ImageLoader::~ImageLoader()
{
}

bool ImageLoader::LoadFromFile(const std::string& filePath)
{
    mImage = cv::imread(filePath, cv::IMREAD_COLOR);

    if (mImage.empty())
    {
        std::cerr << "Failed to load image: " << filePath << std::endl;
        return false;
    }

    std::cout << "Image loaded successfully: " << filePath << std::endl;
    return true;
}

int ImageLoader::GetWidth() const
{
    return mImage.cols;
}

int ImageLoader::GetHeight() const
{
    return mImage.rows;
}

int ImageLoader::GetChannels() const
{
    return mImage.channels();
}

const cv::Mat& ImageLoader::GetImage() const
{
    return mImage;
}