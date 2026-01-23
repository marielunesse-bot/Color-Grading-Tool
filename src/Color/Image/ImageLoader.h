#pragma once

#include <string>
#include <opencv2/opencv.hpp>

class ImageLoader
{
public:
    ImageLoader();
    ~ImageLoader();

    bool LoadFromFile(const std::string& filePath);

    int GetWidth() const;
    int GetHeight() const;
    int GetChannels() const;

    const cv::Mat& GetImage() const;

private:
    cv::Mat mImage;
};