#pragma once
#include "../Core/Application.h"

class ColorGradingApp : public ColorGradingTool::Application
{
public:
    ColorGradingApp(const ColorGradingTool::ApplicationProperties& props);
    ~ColorGradingApp();

    void Run() override;
};