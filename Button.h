#pragma once
#include "UIElement.h"
#include <functional>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct Button : public UIElement {
    Button(short int x, short int y, unsigned short int width, unsigned short int height, const std::string& text);
    void setOnClick(std::function<void()> callback);
    void setTooltip(const std::string& tooltipText);
    void draw(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event& event) override;

    private:
        short int x;
        short int y;
        unsigned short int width;
        unsigned short int height;
        std::string text;
        std::string tooltipText;
        bool isHovered;
        Uint32 hoverStartTime;
        bool showTooltip;
        TTF_Font* font;
        std::function<void()> onClick;
};
